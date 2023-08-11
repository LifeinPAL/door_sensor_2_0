/**@file	HAL_watchdog.c
 * 
 * @brief	本文件为看门狗相关功能的硬件抽象层文件
 *			led配置同属gpio相关功能
 *
 * @detail	本文件包含application使用gpio相关功能的接口函数。
 *			同时作为nrf5 SDK 的上层封装
 */
 
/**< private include. */
#include "HAL_watchdog.h"

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
					   bool is_enable)
{
	ret_code_t err_code = NRF_SUCCESS;
	nrf_drv_wdt_config_t config_wdt = NRF_DRV_WDT_DEAFULT_CONFIG;

    err_code = nrf_drv_wdt_init(&config_wdt, wdt_evt_handler);
	APP_ERROR_CHECK(err_code);
	err_code = nrf_drv_wdt_channel_alloc(p_channel_id);
	APP_ERROR_CHECK(err_code);
	if (is_enable)
	{
		nrf_drv_wdt_enable();
	}
	
	return err_code;
}

/**@brief 按通道喂狗
 * 
 * @param[in] _channel_id 喂狗通道号
 * 
 * @retval NONE
*/
void watchdog_feed(nrf_drv_wdt_channel_id _channel_id)
{
    nrf_drv_wdt_channel_feed(_channel_id);
}




/**/
