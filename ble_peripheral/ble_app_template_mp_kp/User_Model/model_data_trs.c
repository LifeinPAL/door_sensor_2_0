/**@file model_data_trs.c
 *
 * @brief ���ڴ����������������������ݰ��ķ��ͣ��ȴ�Ӧ�𣬴洢
 * 
 * @version 2.0.0.alpha.20230717
 *
 */
 
/**< Private include */
#include "model_data_trs.h"
#include "main.h"
#include "model_ble.h"
#include "HAL_timer.h"
#include "data_transfer.h"
#include "model_user_protocol.h"
#include "model_log.h"
#include "HAL_flash.h"


/* private define */
#define DOOR_DATA_QUEUE_SIZE 10



/* global variable */
bool g_flag_acktimer_status = false;



/* static variable */
NRF_QUEUE_DEF(door_data_t, m_door_data_queue, DOOR_DATA_QUEUE_SIZE, NRF_QUEUE_MODE_OVERFLOW);
NRF_QUEUE_INTERFACE_DEF(door_data_t, door_data_queue, &m_door_data_queue);
NRF_QUEUE_DEF(door_data_t, m_door_sent_queue, DOOR_DATA_QUEUE_SIZE, NRF_QUEUE_MODE_OVERFLOW);
NRF_QUEUE_INTERFACE_DEF(door_data_t, door_sent_queue, &m_door_sent_queue);

APP_TIMER_DEF(m_app_timer_ack_timeout);

/**@brief ���ݰ�Ӧ��ʱ�¼�����
*/
static void app_timer_handler_ack_timeout(void* p_context)
{
    door_data_t door_data_temp;
	LOGINFO("ack timeout!!");
    while (!door_sent_queue_is_empty())
    {
		LOGINFO("store a data.");
        door_sent_queue_pop(&door_data_temp);
        doordata_record_push(FILE_ID_DATA, &door_data_temp, sizeof(door_data_temp));
    }
    g_flag_acktimer_status = false;
}

/**@brief model_data_trs�����ʱ����ʼ��
*/
void app_timer_init_data_trs(void)
{
    hal_app_timer_create(&m_app_timer_ack_timeout, APP_TIMER_MODE_SINGLE_SHOT, app_timer_handler_ack_timeout);
}


/**@brief �����Ϳ��������ݰ����
 * 
 * @param[out] �����Ŀ��������ݰ�
 * @param[in]  ������������Ϣ
*/
void data_payload_fill(u8* p_packet, const door_data_t* p_door_data)
{
    *(p_packet + DATA_DOORSTATE_OFFSET) = p_door_data->door_state;
    pack_bigendian16(p_door_data->battery_voltage, p_packet + DATA_BATTERYVOLTAGE_OFFSET);
    *(p_packet + DATA_WAKEUPSOURCE_OFFSET) = (u8)p_door_data->wkp_src;
    pack_bigendian32(p_door_data->global_number, p_packet + DATA_GLOBALNUMBER_OFFSET);

    if (g_lts != LTS_NO_BOND)
    {
        pack_bigendian16(p_door_data->calendar.year, p_packet + DATA_CALENDAR_YEAR_OFFSET);
        *(p_packet + DATA_CALENDAR_MONTH_OFFSET) = p_door_data->calendar.month;
        *(p_packet + DATA_CALENDAR_DAY_OFFSET) = p_door_data->calendar.day;
        *(p_packet + DATA_CALENDAR_HOUR_OFFSET) = p_door_data->calendar.hour;
        *(p_packet + DATA_CALENDAR_MINUTE_OFFSET) = p_door_data->calendar.minute;
        *(p_packet + DATA_CALENDAR_SECOND_OFFSET) = p_door_data->calendar.second;
        *(p_packet + DATA_FLAGSTORAGE_OFFSET) = record_inflie_countget(FILE_ID_DATA);
    }
}

/**@brief ���ݷ��ʹ�������
 * 
 * @param[in] p_door_data ��Ҫ���͵Ŀ��������ݣ���Ϊ�մӿ������¼������л�ȡ
 * @param[in] des ���ݷ��͵ķ��򣬸ò������������������Դ��ʶ������ȱʡֵΪ0xFF
*/
void data_sending_procedure(door_data_t* p_door_data,  u8 des)
{
    u8 packet_send[PROTOCOL_PAYLOAD_OFFSET + DATA_PAYLOAD_SIZE + PROTOCOL_FRAME_END_SIZE];
    door_data_t door_data_temp;
    u8 packet_size = PROTOCOL_PAYLOAD_OFFSET + DATA_PAYLOAD_SIZE + PROTOCOL_FRAME_END_SIZE;

    if (p_door_data == NULL)
    {
        door_data_queue_pop(&door_data_temp);
        p_door_data = &door_data_temp;
    }
    memset(packet_send, 0xFF, packet_size);
    data_payload_fill(packet_send + PROTOCOL_PAYLOAD_OFFSET, p_door_data);
    /* ��δ��״̬��Ӧ�����Թ����豸�Ĳ�ѯ����ʱ */
    if ((LTS_NO_BOND == g_lts) || (0x04 == des))
    {
        user_protocol_pack(packet_send, PROPERTY_DATA, des, DATA_PAYLOAD_SIZE);
        ble_data_send(packet_send, packet_size, g_manager_conn_handle);
    }
    else if ((0x01 == des) || (0x03 == des))
    {
        user_protocol_pack(packet_send, PROPERTY_DATA, des, DATA_PAYLOAD_SIZE);
		NRF_LOG_HEXDUMP_INFO(packet_send, packet_size);
		LOGINFO("central conn handle = 0x%x", g_central_conn_handle);
        ble_data_send(packet_send, packet_size, g_central_conn_handle);
    }
    else if (LTS_BOND_UNCONN == g_lts)
    {
		LOGINFO("no central connect, storage data.");
        doordata_record_push(FILE_ID_DATA, &door_data_temp, sizeof(door_data_temp));
    }
    else
    {
        if (g_manager_conn_handle != BLE_CONN_HANDLE_INVALID)
        {
			LOGINFO("send door data to manager.");
            user_protocol_pack(packet_send, PROPERTY_DATA, 0x04, DATA_PAYLOAD_SIZE);
            ble_data_send(packet_send, packet_size, g_manager_conn_handle);
        }
        else
        {
			LOGINFO("send door data to central.");
            user_protocol_pack(packet_send, PROPERTY_DATA, 0x03, DATA_PAYLOAD_SIZE);
			NRF_LOG_HEXDUMP_INFO(packet_send, packet_size);
            ble_data_send(packet_send, packet_size, g_central_conn_handle);
            door_sent_queue_push(&door_data_temp);
#if 1 == MANUAL_SIMULATION
            hal_app_timer_start(m_app_timer_ack_timeout, 15000, NULL);
#else
			hal_app_timer_start(m_app_timer_ack_timeout, 6000, NULL);
#endif
			LOGINFO("ack timoust timer start.");
            g_flag_acktimer_status = true;
        }
    }
}

/**@brief ���յ�data��rsp���ݰ��Ĵ�����
 * 
 * @param[in] ָ��rsp���ݰ���data��Ӧ����Ӧ����
*/
void rsp_data_handler(comm_param_t* p_context)
{
	const u8* p_ack_num = p_context->context;
    u32 number = unpack_bigendian32(p_ack_num);
    door_data_t temp;
    door_sent_queue_pop(&temp);
	LOGINFO("rsp num %d & queue num %d", number, temp.global_number);
    if (number != temp.global_number)
    {
        door_sent_queue_push(&temp);
    }
}

/**@brief ���door_data_t����������־ 
 *
 * @param[in] p_door_data ָ��door_data_t�������͵�ָ��
 *
 * @retval NONE
*/
void doordata_log(door_data_t* p_door_data)
{
	LOGINFO("door state: %d,", p_door_data->door_state);
	LOGINFO("wakeup source: %d,", p_door_data->wkp_src);
	LOGINFO("battery voltage: %d,", p_door_data->battery_voltage);
	LOGINFO("event number: %d,", p_door_data->global_number);
	LOGINFO("current day: %d year, %d month, %d day, %d hour, %d minute, %d second.",p_door_data->calendar.year, p_door_data->calendar.month, p_door_data->calendar.day, p_door_data->calendar.hour, p_door_data->calendar.minute, p_door_data->calendar.second);
}





