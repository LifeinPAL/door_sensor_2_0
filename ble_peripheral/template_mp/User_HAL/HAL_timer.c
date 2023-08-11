/**@file	HAL_timer.c
 * 
 * @brief	本文件为timer相关功能的硬件抽象层文件
 *
 * @detail	本文件包含application使用timer相关功能的接口函数。
 *			同时作为nrf5 SDK 的上层封装
*/

/* private include */
#include "HAL_timer.h"
#include "app_error.h"


/* function defination */

/**@brief apptimer 总体初始化函数
*/
void apptimer_init(void)
{
	ret_code_t err_code;
	err_code = app_timer_init();
	APP_ERROR_CHECK(err_code);
}

/**@brief 初始化硬件定时器
 * 
 * @param[in] p_timer_instance 需要初始化的定时器实例指针
 *			  p_init 定时器初始化参数结构体指针
 *            timer_evt_handler 定时器事件处理函数 
 * 
 * @retval	NRF_SUCCESS;
*/
ret_code_t timer_init(const nrfx_timer_t * const p_timer_instance, 
					  const HAL_timer_init_t* p_init, 
					  nrfx_timer_event_handler_t timer_evt_handler)
{
	ret_code_t err_code = NRF_SUCCESS;

	err_code = nrf_drv_timer_init(p_timer_instance, &p_init->timer_config, timer_evt_handler);
	APP_ERROR_CHECK(err_code);

	nrf_drv_timer_extended_compare(p_timer_instance,
								   p_init->cc_channel,
								   p_init->cc_value,
								   p_init->short_mask,
								   p_init->enable_init);
	
	return err_code;
}

/**@brief 开始定时器
 * 
 * @param[in] p_timer_instance
 * 
 * @retval NONE
*/
void timer_start(const nrfx_timer_t* const p_timer_instance)
{
    nrf_drv_timer_enable(p_timer_instance);
}


/**@brief 停止定时器
 * 
 * @param[in] p_timer_instance
 * 
 * @retval NONE
*/
void timer_stop(const nrfx_timer_t* const p_timer_instance)
{
    nrf_drv_timer_disable(p_timer_instance);
}

/**@brief 检查定时器是否运行
 * 
 * @param[in] p_timer_instance
 * 
 * @retval true
 *         false
*/
bool is_timer_start(const nrfx_timer_t* const p_timer_instance)
{
    return nrf_drv_timer_is_enabled(p_timer_instance);
}

/**@brief 创建软件定时器
 * 
 * @param[in] p_timer_id 		软件定时器实例指针
 * @param[in] mode		 		软件定时器模式
 * @param[in] timeout_handler	软件定时器超时处理函数
 * 
 * @retval NRF_STATUS
*/
ret_code_t hal_app_timer_create(const app_timer_id_t* 		p_timer_id,
								app_timer_mode_t			mode,
								app_timer_timeout_handler_t	timeout_handler)
{
	return app_timer_create(p_timer_id, mode, timeout_handler);
}

/**@brief 启动软件定时器
 * 
 * @param[in] timer_id 			软件定时器实例
 * @param[in] timeout_ms		定时时间（ms）
 * @param[in] p_context			需要传递的参数，没有用NULL
 * 
 * @retval NRF_STATUS
*/
ret_code_t hal_app_timer_start(app_timer_id_t	timer_id,
								u32				timeout_ms,
								void*			p_context)
{
	return app_timer_start(timer_id, APP_TIMER_TICKS(timeout_ms), p_context);
}

/**@brief 停止软件定时器
 * 
 * @param[in] timer_id 			软件定时器实例
 * 
 * @retval NRF_STATUS
*/
ret_code_t hal_app_timer_stop(app_timer_id_t	timer_id)
{
	return app_timer_stop(timer_id);
}

/**/
