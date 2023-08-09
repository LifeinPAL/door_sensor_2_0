/**@file model_cmd_act.c
 *
 * @brief 用于处理与蓝牙主机交互的数据包的发送，等待应答，存储
 * 
 * @version 2.0.0.alpha.20230717
 *
 */
 
/**< Private include */
#include "model_cmd_act.h"
#include "model_data_trs.h"
#include "model_log.h"
#include "main.h"
#include "model_ble.h"
#include "model_bond.h"
#include "data_transfer.h"
#include "HAL_timer.h"
#include "power.h"
#include "user_hardware.h"
//#include "HAL_gpio.h"
#include "board_door_sensor.h"
#include "HAL_flash.h"
#include "event_id.h"
#include "model_calendar.h"

/* private define */


/* private declair */



void door_query_handler(comm_param_t* p_context);
void door_read_handler(comm_param_t* p_context);
void bond_del_req_handler(comm_param_t* p_context);
void bond_addr_query_handler(comm_param_t* p_context);
void remote_reset_handler(comm_param_t* p_context);
void storage_clear_handler(comm_param_t* p_context);
void force_clear_handler(comm_param_t* p_context);
void local_addr_query_handler(comm_param_t* p_context);
void calendar_update_handler(comm_param_t* p_context);
void enter_dfu_handler(comm_param_t* p_context);

/* 以下是一些调试输出指令 */
void gpio_init_handler(comm_param_t* p_context);
void glt_handler(comm_param_t* p_context);

/* global variable */
u8 g_doordata_read_count = 0;



/* static variable */
NRF_QUEUE_DEF(comm_param_t, m_cmd_queue, CMD_QUEUE_SIZE, NRF_QUEUE_MODE_NO_OVERFLOW);
NRF_QUEUE_INTERFACE_DEF(comm_param_t, cmd_queue, &m_cmd_queue);
NRF_QUEUE_DEF(comm_param_t, m_rsp_queue, RSP_QUEUE_SIZE, NRF_QUEUE_MODE_NO_OVERFLOW);
NRF_QUEUE_INTERFACE_DEF(comm_param_t, rsp_queue, &m_rsp_queue);


//static u8 crd_buffer[PAYLOAD_SIZE_MAX] = {0};

//static const char* rsp_list[RSP_LIST_SIZE] = {
//    "data",
//    "auth"
//};

//static const rsp_fun rsp_fun_list[RSP_LIST_SIZE] = {
//    (rsp_fun)&rsp_data_handler,
//    (rsp_fun)&rsp_auth_handler,
//};

action_table_t rsp_list[RSP_LIST_SIZE] = {
	{"data",rsp_data_handler},
	{"auth_request",rsp_auth_handler}
};

action_table_t cmd_list[CMD_LIST_SIZE] = {
	{"DOOR_QUERY", door_query_handler},
	{"DOOR_READ", door_read_handler},
	{"STORAGE_CLEAR", storage_clear_handler},
	{"BOND_DEL_REQ", bond_del_req_handler},
	{"BOND_ADDR_QUERY", bond_addr_query_handler},
	{"LOCAL_ADDR_QUERY", local_addr_query_handler},
	{"REMOTE_RESET", remote_reset_handler},
	{"FORCE_CLEAR", force_clear_handler},
	{"CALENDAR_UPDATE", calendar_update_handler},
	{"ENTER_DFU", enter_dfu_handler},
	{"GPIO_INIT", gpio_init_handler},
	{"GLT", glt_handler}
};

/* export function */


/**@brief 获取处理函数在函数表的位置
 * 
 * @param[in] p_rsp_data 
 * 
 * @retval 
*/
static u8 handler_idget(const u8* p_payloard, const action_table_t* p_list, const size_t list_size)
{
    u8 id = 0xFF;
    for (size_t i = 0; i < list_size; i++)
    {
        if (!(strncmp((const char*)p_payloard, p_list[i].p_name, strlen(p_list[i].p_name))))
        {
            id = i;
            break;
        }        
    }
    
    return id;
}

/**@brief 解析有效数据
 *
 * @detail 根据指令（响应）表，组成需要传递的指令（响应）参数
 *
 * @param[in] p_ctrl	控制字段数据，取用src_id, des_id, payload_len, payload
 *
 * @param[out] p_comm_param	需要传递的参数，包括id, src, context 需要压入队列
 *
 * @retval true or false
*/
bool payload_resolve(protocol_ctrl_t* p_ctrl, comm_param_t* p_comm_param)
{
	if (PROPERTY_COMMOND == p_ctrl->type)
	{
		p_comm_param->id = handler_idget(p_ctrl->payload, cmd_list, CMD_LIST_SIZE);
		if (p_comm_param->id != 0xFF)
		{
			memcpy(p_comm_param->context, p_ctrl->payload + strlen(cmd_list[p_comm_param->id].p_name), p_ctrl->payload_len - strlen(cmd_list[p_comm_param->id].p_name));
		}
		else
		{
			LOGINFO("no command in list.");
			return false;
		}
		
	}
	else if (PROPERTY_RESPONCE == p_ctrl->type)
	{
		p_comm_param->id = handler_idget(p_ctrl->payload, rsp_list, RSP_LIST_SIZE);
		if (p_comm_param->id != 0xFF)
		{
			memcpy(p_comm_param->context, p_ctrl->payload + strlen(rsp_list[p_comm_param->id].p_name), p_ctrl->payload_len - strlen(rsp_list[p_comm_param->id].p_name));
		}
		else
		{
			LOGINFO("no response in list.");
			return false;
		}
	}
	return true;
}

/**@brief 响应处理函数跳转
 *
 * @param[in] p_p_comm_param
 *
 * @retval NONE
*/
void rsp_jump(comm_param_t* p_comm_param)
{
//	LOGINFO("rsp jump ready.");
	rsp_list[p_comm_param->id].handler(p_comm_param);
	LOGINFO("rsp jump over.");
}

/**@brief 指令处理函数跳转
 *
 * @param[in] p_p_comm_param
 *
 * @retval NONE
*/
void cmd_jump(comm_param_t* p_comm_param)
{
//	LOGINFO("cmd jump ready.");
	cmd_list[p_comm_param->id].handler(p_comm_param);
	LOGINFO("cmd jump over.");
}

///**@brief 将有效数据从串口缓冲复制到处理命令、响应的待处理缓冲区
// * 
// * @param[in] p_data 待复制的数据
// * @param[in] size   待复制的数据大小
// * 
// * @retval
//*/
//void crd_copy(const u8* p_data, u8 size)
//{
//    memcpy(crd_buffer, p_data, size);
//}


/**@brief 命令的通用格式响应
 * 
 * @param[in] p_cmd_param 命令参数结构体
 * @param[in] p_gen_rsp 响应字符串，一般为
 *                      "success" "failed" "timeout" "busy" ...
 * @param[in] len   响应字符串长度
 * 
 * @retval NONE
*/
void cmd_general_rsp(const comm_param_t* p_cmd_param, const char* p_gen_rsp, const u8 len)
{
    u8 packet_genrsp[36];
	
	memset(packet_genrsp, 0xFF, 36);
    stringtolower((char*)packet_genrsp + PROTOCOL_PAYLOAD_OFFSET, cmd_list[p_cmd_param->id].p_name, strlen(cmd_list[p_cmd_param->id].p_name));
    strncpy((char*)(packet_genrsp + PROTOCOL_PAYLOAD_OFFSET + strlen(cmd_list[p_cmd_param->id].p_name)), p_gen_rsp, len);
    user_protocol_pack(packet_genrsp, PROPERTY_RESPONCE, p_cmd_param->src, strlen(cmd_list[p_cmd_param->id].p_name) + len);
    ble_data_send(packet_genrsp, (u16)(PROTOCOL_PAYLOAD_OFFSET + strlen(cmd_list[p_cmd_param->id].p_name) + len + PROTOCOL_FRAME_END_SIZE), p_cmd_param->conn_handle);
}

/**@brief 命令的带参数格式响应
 * 
 * @param[in] p_cmd_param 命令参数结构体
 * @param[in] p_rsp 
 * @param[in] len   响应字符串长度
 * 
 * @retval NONE
*/
void cmd_rsp(const comm_param_t* p_cmd_param, const u8* p_rsp, const u8 len)
{
    u8 packet_rsp[36];
	
	memset(packet_rsp, 0xFF, 36);
    stringtolower((char*)packet_rsp + PROTOCOL_PAYLOAD_OFFSET, cmd_list[p_cmd_param->id].p_name, strlen(cmd_list[p_cmd_param->id].p_name));
    memcpy((u8*)(packet_rsp + PROTOCOL_PAYLOAD_OFFSET + strlen(cmd_list[p_cmd_param->id].p_name)), p_rsp, len);
	user_protocol_pack(packet_rsp, PROPERTY_RESPONCE, p_cmd_param->src, strlen(cmd_list[p_cmd_param->id].p_name) + len);
    ble_data_send(packet_rsp, (u16)(PROTOCOL_PAYLOAD_OFFSET + strlen(cmd_list[p_cmd_param->id].p_name) + len + PROTOCOL_FRAME_END_SIZE), p_cmd_param->conn_handle);
}

///* 注册一个指示绑定相关事件超时的定时器 */
APP_TIMER_DEF(m_bond_timer);
/**@brief 数据包应答超时事件处理
*/
static void app_timer_handler_bond_timeout(void* p_context)
{
	fun_bond_status = USER_TIMEOUT;
}

/**@brief 绑定相关的超时计时器初始化
 * 
*/
void app_timer_init_bond(void)
{
    hal_app_timer_create(&m_bond_timer, APP_TIMER_MODE_SINGLE_SHOT, app_timer_handler_bond_timeout);
}

/**@brief 删除绑定信息函数
 *
 * @detail 删除从机本地的绑定信息，并将执行结果反馈
 * @param[in] p_context
*/
void bond_del_req_handler(comm_param_t* p_context)
{
    fun_bond_status = USER_INVALID;
    delete_bonds();
    hal_app_timer_start(m_bond_timer, 3000, NULL);
    while (USER_INVALID == fun_bond_status)
    {
        IDLE_ENTER();
    }
    hal_app_timer_stop(m_bond_timer);
    cmd_general_rsp(p_context, g_string_status[fun_bond_status], strlen(g_string_status[fun_bond_status]));
}

/**@brief 查询门状态
 *
 * @param[in] p_context
*/
void door_query_handler(comm_param_t* p_context)
{
	door_data_t door_data = DOOR_DATA_DEFAULT;
	
	LOGINFO("door quering.");
    door_data.door_state = HAL_GPIO_READ(USER_MAGNET_PIN);
    door_data.battery_voltage = battery_voltage_mearment();
    door_data.wkp_src = WKP_SRC_BLUE;
    if (g_lts != LTS_NO_BOND)
    {        
        door_data.global_number = ++g_door_event_number;
        calendar_get(&door_data.calendar);
    }
    data_sending_procedure(&door_data, p_context->src);
}

/**@brief 读取存储的开关门数据
 * 
 * @param[in] p_context
*/
void door_read_handler(comm_param_t* p_context)
{
    u16 door_rest = record_inflie_countget(FILE_ID_DATA);
	
	LOGINFO("door rest %d.", door_rest);
	if (door_rest)
	{
		door_data_t door_data;
		u8 packet_buffer[PROTOCOL_TOTAL_SIZE_MAX];
		memset(packet_buffer, 0xFF, PROTOCOL_TOTAL_SIZE_MAX);
		u8* p_packet_buffer = packet_buffer;
		u8 count = 0;
		g_doordata_read_count = 0;
		
		p_packet_buffer += PROTOCOL_PAYLOAD_OFFSET;
		stringtolower((char*)p_packet_buffer, cmd_list[p_context->id].p_name, strlen(cmd_list[p_context->id].p_name));
		p_packet_buffer += strlen(cmd_list[p_context->id].p_name);

		for (u16 i = 1; i <= door_rest; i++)
		{
			file_record_read(FILE_ID_DATA, i, &door_data, sizeof(door_data_t));
			data_payload_fill(p_packet_buffer, &door_data);
			p_packet_buffer += DATA_PAYLOAD_SIZE;
			count++;
			g_doordata_read_count++;
			if (3 == count)
			{
				const u8 payload_size = strlen(cmd_list[p_context->id].p_name) + count * DATA_PAYLOAD_SIZE;
				user_protocol_pack(packet_buffer, PROPERTY_RESPONCE, p_context->src, payload_size);
				NRF_LOG_HEXDUMP_INFO(packet_buffer, (PROTOCOL_PAYLOAD_OFFSET + payload_size + PROTOCOL_FRAME_END_SIZE));
				ble_data_send(packet_buffer, (u16)(PROTOCOL_PAYLOAD_OFFSET + payload_size + PROTOCOL_FRAME_END_SIZE), p_context->conn_handle);
				count = 0;
				p_packet_buffer = packet_buffer + PROTOCOL_PAYLOAD_OFFSET + strlen(cmd_list[p_context->id].p_name);
			}        
		}
		if (count)
		{
			const u8 payload_size = strlen(cmd_list[p_context->id].p_name) + count * DATA_PAYLOAD_SIZE;
			user_protocol_pack(packet_buffer, PROPERTY_RESPONCE, p_context->src, payload_size);
			NRF_LOG_HEXDUMP_INFO(packet_buffer, (PROTOCOL_PAYLOAD_OFFSET + payload_size + PROTOCOL_FRAME_END_SIZE));
			ble_data_send(packet_buffer, (u16)(PROTOCOL_PAYLOAD_OFFSET + payload_size + PROTOCOL_FRAME_END_SIZE), p_context->conn_handle);
			count = 0;
			p_packet_buffer = packet_buffer + PROTOCOL_PAYLOAD_OFFSET + strlen(cmd_list[p_context->id].p_name);
		} 
	}
	else
	{
		LOGINFO("no record storaged.");
	}
   
}

/**@brief 清除存储的开关门数据
 * 
 * @param[in] p_context
*/
void storage_clear_handler(comm_param_t* p_context)
{
    u16 door_rest = record_inflie_countget(FILE_ID_DATA);

    UNUSED_PARAMETER(p_context);
	if (door_rest)
	{
		if (g_doordata_read_count)  
		{	
			/* 如果应答超时定时器在工作，先等待本次应答存储操作完成后在清除 */
			while (g_flag_acktimer_status)
			{
				IDLE_ENTER();
			}
			LOGINFO("rest %d and read %d.", door_rest, g_doordata_read_count);
			if (door_rest > g_doordata_read_count)
			{
				u16 i,j;
				for (i = 1; i <= (door_rest-g_doordata_read_count); i++)
				{
					doordata_record_move(FILE_ID_DATA, i, i+g_doordata_read_count);
				}
				for (j = i; j <= g_doordata_read_count; j++)
				{
					HAL_record_del(FILE_ID_DATA, j);
				}        
			}
			else
			{
				HAL_file_del(FILE_ID_DATA);
			}
			cmd_general_rsp(p_context, "success", strlen("success"));
		}
		else
		{
			LOGINFO("record is not read.");
			cmd_general_rsp(p_context, "unread", strlen("unread"));
		}
	}
	else
	{
		LOGINFO("record is empty.");
		cmd_general_rsp(p_context, "empty", strlen("empty"));
	}
}

/**@brief 强制清除存储数据
 * 
 * @param[in] p_context
*/
void force_clear_handler(comm_param_t* p_context)
{
	HAL_file_del(FILE_ID_DATA);
	LOGINFO("file clear over.");
}


/**@brief 查询绑定设备对端地址
 * 
 * @param[in] p_context
*/
void bond_addr_query_handler(comm_param_t* p_context)
{
    ret_code_t err_code;
	u8 peer_addr[6] = {0xFF};
    if (NRF_SUCCESS == (err_code = bond_addr_get(peer_addr, g_central_conn_handle)))
	{	
		LOGINFO("peer address: %x %x %x %x %x %x",
		peer_addr[5],
		peer_addr[4],
		peer_addr[3],
		peer_addr[2],
		peer_addr[1],
		peer_addr[0]);
		cmd_rsp(p_context, (const u8*)peer_addr, 6);
	}
	else
	{
		LOGINFO("error: %s.", nrf_error_type[err_code]);
	}
}

/**@brief 查询设备本地地址
 * 
 * @param[in] p_context
*/
void local_addr_query_handler(comm_param_t* p_context)
{
    u8 local_addr[6] = {0xFF};
	
	memset(local_addr, 0xFF, 6);
    local_addr_get(local_addr);
	LOGINFO("local address: %x %x %x %x %x %x",
			local_addr[5],
			local_addr[4],
			local_addr[3],
			local_addr[2],
			local_addr[1],
			local_addr[0]);
    cmd_rsp(p_context, (const u8*)local_addr, 6);
}

/**@brief 设备复位
 * 
 * @param[in] p_context
*/
void remote_reset_handler(comm_param_t* p_context)
{
    UNUSED_PARAMETER(p_context);
//	global_number_write(FILE_ID_GLOBAL_NO, RECORD_KEY_GLOBAL_NO, &g_door_event_number);
	LOGINFO("soft reset now!");
    SHUTDOWN_TO(NRF_PWR_MGMT_SHUTDOWN_RESET);
}

/**@brief 日历时间更新
 * 
 * @param[in] p_context
*/
void calendar_update_handler(comm_param_t* p_context)
{
	calendar_data_t new_calendar;
	u8* p_value = p_context->context;
	
	new_calendar.year = unpack_bigendian16(p_value);
	p_value += 2;
	new_calendar.month 	= *(p_value++);
	new_calendar.day 	= *(p_value++);
	new_calendar.hour 	= *(p_value++);
	new_calendar.minute = *(p_value++);
	new_calendar.second = *(p_value++);
	if (calendar_update(&new_calendar))
	{
		LOGINFO("update time: %d year, %d month, %d day, %d hour, %d minute, %d second.",new_calendar.year, new_calendar.month,new_calendar.day, new_calendar.hour, new_calendar.minute, new_calendar.second);
		cmd_general_rsp(p_context, "success", strlen("success"));
	}
	else
	{
		cmd_general_rsp(p_context, "failed", strlen("failed"));
	}
}

/**@brief 进入DFU模式
 * 
 * @param[in]
 *
*/
void enter_dfu_handler(comm_param_t* p_context)
{
	UNUSED_PARAMETER(p_context);
	LOGINFO("Enter DFU mode.");
	/* 进入DFU前的配置 */
	
//	SHUTDOWN_TO(NRF_PWR_MGMT_SHUTDOWN_GOTO_DFU);
}


/* 以下指令只在调试过程中使用 */
void gpio_init_handler(comm_param_t* p_context)
{
	bool init = is_gpiote_init();
	UNUSED_PARAMETER(p_context);
	LOGINFO("gpio is init: %d", (u8)init);
}
void glt_handler(comm_param_t* p_context)
{
	UNUSED_PARAMETER(p_context);
	LOGINFO("glt 0x%x", g_lts);
}

