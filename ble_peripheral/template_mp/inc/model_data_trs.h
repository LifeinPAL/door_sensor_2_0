/**@file model_data_trs.h
 *
 * @brief �������ɼ����ݵķ��ͣ����գ�Ӧ�𣩣��洢
 */
 
#ifndef __MODEL_DATATRS_H__
#define __MODEL_DATATRS_H__

/**< private include */
#include "common_abbr.h"
#include "nrf_queue.h"
#include "model_calendar.h"
#include "model_cmd_act.h"


/* export typedef */

typedef enum{
	WKP_SRC_BLUE = 'E',
	WKP_SRC_DOOR = 'U',
	WKP_SRC_VALID = 0xFF
}wakeup_source_t;

typedef struct door_data_s
{
	u8 door_state;
	i16 battery_voltage;
	wakeup_source_t wkp_src;
	u32 global_number;
	calendar_data_t calendar;
	u8 error_code;
}door_data_t;

/* export variable */
extern bool g_flag_acktimer_status;

/* export define */

#define DOOR_DATA_DEFAULT				\
{										\
	.door_state = 0xFF,					\
	.battery_voltage = 0xFFFF,			\
	.wkp_src = WKP_SRC_VALID,			\
	.global_number = 0xFFFFFFFF,		\
	.calendar = CALENDAR_DEFAULT,		\
	.error_code = 0xFF					\
}


/* FILE_ID restriction 0x0000~0xBFFF
   RECORD_KEY restriction 0x0001~0xBFFF
 */

#define FILE_ID_DATA 0x0020


#define DATA_DOORSTATE_SIZE           1 
#define DATA_BATTERYVOLTAGE_SIZE      2
#define DATA_WAKEUPSOURCE_SIZE        1
#define DATA_GLOBALNUMBER_SIZE        4 
#define DATA_CALENDAR_YEAR_SIZE       2
#define DATA_CALENDAR_MONTH_SIZE      1
#define DATA_CALENDAR_DAY_SIZE        1
#define DATA_CALENDAR_HOUR_SIZE       1
#define DATA_CALENDAR_MINUTE_SIZE     1
#define DATA_CALENDAR_SECOND_SIZE     1
#define DATA_FLAGSTORAGE_SIZE         1
#define DATA_ERRORCODE_SIZE           1
#define DATA_RSSI_SIZE                1

#define DATA_PAYLOAD_SIZE (DATA_DOORSTATE_SIZE + \
                           DATA_BATTERYVOLTAGE_SIZE + \
                           DATA_WAKEUPSOURCE_SIZE + \
                           DATA_GLOBALNUMBER_SIZE + \
                           DATA_CALENDAR_YEAR_SIZE + \
                           DATA_CALENDAR_MONTH_SIZE + \
                           DATA_CALENDAR_DAY_SIZE + \
                           DATA_CALENDAR_HOUR_SIZE + \
                           DATA_CALENDAR_MINUTE_SIZE + \
                           DATA_CALENDAR_SECOND_SIZE + \
                           DATA_FLAGSTORAGE_SIZE + \
                           DATA_ERRORCODE_SIZE + \
                           DATA_RSSI_SIZE)


#define DATA_DOORSTATE_OFFSET 0
#define DATA_BATTERYVOLTAGE_OFFSET  (DATA_DOORSTATE_OFFSET + DATA_DOORSTATE_SIZE)
#define DATA_WAKEUPSOURCE_OFFSET    (DATA_BATTERYVOLTAGE_OFFSET + DATA_BATTERYVOLTAGE_SIZE)
#define DATA_GLOBALNUMBER_OFFSET    (DATA_WAKEUPSOURCE_OFFSET + DATA_WAKEUPSOURCE_SIZE)
#define DATA_CALENDAR_YEAR_OFFSET   (DATA_GLOBALNUMBER_OFFSET + DATA_GLOBALNUMBER_SIZE)
#define DATA_CALENDAR_MONTH_OFFSET  (DATA_CALENDAR_YEAR_OFFSET + DATA_CALENDAR_YEAR_SIZE)
#define DATA_CALENDAR_DAY_OFFSET    (DATA_CALENDAR_MONTH_OFFSET + DATA_CALENDAR_MONTH_SIZE)
#define DATA_CALENDAR_HOUR_OFFSET   (DATA_CALENDAR_DAY_OFFSET + DATA_CALENDAR_DAY_SIZE)
#define DATA_CALENDAR_MINUTE_OFFSET (DATA_CALENDAR_HOUR_OFFSET + DATA_CALENDAR_HOUR_SIZE)
#define DATA_CALENDAR_SECOND_OFFSET (DATA_CALENDAR_MINUTE_OFFSET + DATA_CALENDAR_MINUTE_SIZE)
#define DATA_FLAGSTORAGE_OFFSET     (DATA_CALENDAR_SECOND_OFFSET + DATA_CALENDAR_SECOND_SIZE)
#define DATA_ERRORCODE_OFFSET       (DATA_FLAGSTORAGE_OFFSET + DATA_FLAGSTORAGE_SIZE)
#define DATA_RSSI_OFFSET            (DATA_ERRORCODE_OFFSET + DATA_ERRORCODE_SIZE)


/**< export function prototype */
NRF_QUEUE_INTERFACE_DEC(door_data_t, door_data_queue);
NRF_QUEUE_INTERFACE_DEC(door_data_t, door_sent_queue);

/**@brief model_data_trs�����ʱ����ʼ��
*/
void app_timer_init_data_trs(void);

/**@brief ���յ�data��rsp���ݰ��Ĵ�����
 * 
 * @param[in] ָ��rsp���ݰ���data��Ӧ����Ӧ����
*/
void rsp_data_handler(comm_param_t* p_context);

/**@brief ���ݷ��ʹ�������
 * 
 * @param[in] p_door_data ��Ҫ���͵Ŀ��������ݣ���Ϊ�մӿ������¼������л�ȡ
 * @param[in] des ���ݷ��͵ķ��򣬸ò������������������Դ��ʶ������ȱʡֵΪ0xFF
*/
void data_sending_procedure(door_data_t* p_door_data,  u8 des);

/**@brief �����Ϳ��������ݰ����
 * 
 * @param[out] �����Ŀ��������ݰ�
 * @param[in]  ������������Ϣ
*/
void data_payload_fill(u8* p_packet, const door_data_t* p_door_data);


/**@brief ���door_data_t����������־ 
 *
 * @param[in] p_door_data ָ��door_data_t�������͵�ָ��
 *
 * @retval NONE
*/
void doordata_log(door_data_t* p_door_data);


#endif // __MODEL_DATATRS_H__

