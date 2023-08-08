/**@file HAL_watchdog.h
 *
 * @brief 定时器用户硬件抽象层头文件
 */
 
#ifndef __HAL_WATCHDOG_H__
#define __HAL_WATCHDOG_H__

/* private include */

#include "nrf_drv_wdt.h"


/* private define */

/* export typedef */





/* export macro */

		


/* export variable */


/* export function prototype. */

/**@brief 按通道喂狗
 * 
 * @param[in] _channel_id 喂狗通道号
 * 
 * @retval NONE
*/
void watchdog_feed(nrf_drv_wdt_channel_id _channel_id);


/**@brief 初始化看门狗模块
 *
 * @detail
 *
 * @param[in]	p_channel_id				看门狗通道号指针
 *				nrf_wdt_event_handler_t		看门狗中断处理函数
 *   			is_enable					是否开启看门狗
 *
 * @retval	NRF_SUCCESS				执行成功
 */
ret_code_t HAL_watchdog_init(nrf_drv_wdt_channel_id* p_channel_id,
					   nrf_wdt_event_handler_t wdt_evt_handler,
					   bool is_enable);


#endif // __HAL_WATCHDOG_H__


