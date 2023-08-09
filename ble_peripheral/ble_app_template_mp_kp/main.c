/**@file main.c
 *	
 * @brief 应用程序业务主程序
*/

/* private include */

#include "main.h"
#include "model_log.h"
#include "model_ble.h"
#include "HAL_timer.h"
#include "power.h"
#include "user_hardware.h"
#include "model_data_trs.h"
#include "model_bond.h"


/*测试用*/
#include "model_calendar.h"
#include "HAL_flash.h"
#include "nrf_delay.h"
#include "board_door_sensor.h"

/*global variable*/
long_term_status_t 	g_lts 	= LTS_BOND_CONN;
status_machine_t 	g_sm	= SM_IDLE_READY;

u8 wdt_test_num = 0;

const char* g_string_status[4] = {
	"success",
	"failed",
	"timeout",
	"invalid"
};


int main(void)
{
	ret_code_t err_code;
	/* 初始化日志模块 */
	log_init();
	LOGINFO("log module init over.");
	
	/* 用户软件定时器初始化 */
	apptimer_init();
	
	app_timer_init_calendar();
	app_timer_init_data_trs();
	
	/* 电源管理初始化 */
	power_management_init();
	LOGINFO("power module init.");
	
	/* 初始化看门狗功能模块 */
	watchdog_module_init();
	
	/* 蓝牙协议栈初始化 */
	ble_init();
	
	switch (power_reset_reason_get())
	{
		case RESETREAS_SOFT:
		{
			LOGINFO("soft reset.");
//			file_record_read(FILE_ID_GLOBAL_NO, RECORD_KEY_GLOBAL_NO, &g_door_event_number, sizeof(u32));
			LOGINFO("global number: %d.", g_door_event_number);
		}break;
		case RESETREAS_DOG:
		{
			LOGINFO("watchdog reset.")

		}break;
		default:
		{
			LOGINFO("other reset.")
			g_door_event_number = 0;
			g_sm = SM_IDLE_READY;

		}break;
	}
	
	u8 local_addr_test[6] = {0xFF};
	
	memset(local_addr_test, 0xFF, 6);
    local_addr_get(local_addr_test);
	LOGINFO("local address: %x %x %x %x %x %x",
			local_addr_test[5],
			local_addr_test[4],
			local_addr_test[3],
			local_addr_test[2],
			local_addr_test[1],
			local_addr_test[0]);
	
	/*判断传感器是否已经被绑定*/
	if (is_bonded())
	{
		g_lts = LTS_BOND_UNCONN;
		LOGINFO("LTS_BOND_UNCONN : 0x%x.", g_lts);
		app_timer_start_calendar();
	}
	else
	{
		g_lts = LTS_NO_BOND;
		LOGINFO("LTS_NO_BOND : 0x%x.", g_lts);
	}
	
	/* 硬件资源初始化 */
	peri_on_board_init(g_lts);
	
	
	
	/* 开始广播 */
	advertising_start();
	USER_LED_ON(LED_CENTRAL_CONN);
	
	
	g_sm = SM_IDLE_READY;

	while(1)
	{
		switch (g_sm)
		{
			case SM_IDLE_READY:
			{
//				LOGINFO("sm idle ready.");
				IDLE_ENTER();
			}break;
			
			case SM_MISSION_COMPLETE:
			{

				LOGINFO("sm mission complete.");
				g_sm = SM_IDLE_READY;
			}break;
			
			case SM_AUTHENTICATION:
			{
				LOGINFO("sm authentication.");
			}break;
			
			case SM_COMMAND_EXECUTION:
			{
				LOGINFO("sm command execution.");
				comm_param_t cmd_param = {0};
				cmd_queue_pop(&cmd_param);
				LOGINFO("cmd id: %d", cmd_param.id);
				LOGINFO("cmd conn handle: %x", cmd_param.conn_handle);
				LOGINFO("cmd src: %d", cmd_param.src);
				NRF_LOG_HEXDUMP_INFO(cmd_param.context, 32);
				
				cmd_jump(&cmd_param);
				g_sm = SM_IDLE_READY;
			}break;
			
			case SM_RESPONSE_HANDLING:
			{
				LOGINFO("sm response handling.");
				comm_param_t rsp_param = {0};
				rsp_queue_pop(&rsp_param);
				LOGINFO("rsp id: %d", rsp_param.id);
				LOGINFO("rsp conn handle: %x", rsp_param.conn_handle);
				LOGINFO("rsp src: %d", rsp_param.src);
				NRF_LOG_HEXDUMP_INFO(rsp_param.context, 32);
				
				rsp_jump(&rsp_param);
				g_sm = SM_IDLE_READY;
				
			}break;
			case SM_DATA_SENDING:
			{
				LOGINFO("sm data sending.");
				data_sending_procedure(NULL, 0xFF);
				g_sm = SM_MISSION_COMPLETE;
			}break;
			default:
			{
				LOGINFO("unexpected sm status. soft reset now.");
				SHUTDOWN_TO(NRF_PWR_MGMT_SHUTDOWN_RESET);
			}break;
		}
	}
}


/* */ 

