/**@file HAL_timer.h
 *
 * @brief 定时器用户硬件抽象层头文件
 */
 
#ifndef __HAL_TIMER_H__
#define __HAL_TIMER_H__

/* private include */
#include "app_timer.h"
#include "nrf_drv_timer.h"
#include "common_abbr.h"

/* private define */

/* export typedef */

typedef struct timer_init_s
{
    nrf_drv_timer_config_t timer_config;
    nrf_timer_cc_channel_t cc_channel;
    uint32_t cc_value;
    nrf_timer_short_mask_t short_mask;
    bool enable_init;
}HAL_timer_init_t;




/* export macro */
#define HAL_timer_ms_to_tick    nrf_drv_timer_ms_to_ticks
#define HAL_timer_us_to_tick    nrf_drv_timer_us_to_ticks

		


/* export variable */


/* export function prototype. */



/**@brief 开始定时器
 * 
 * @param[in] p_timer_instance
 * 
 * @retval NONE
*/
void timer_start(const nrfx_timer_t* const p_timer_instance);

/**@brief 停止定时器
 * 
 * @param[in] p_timer_instance
 * 
 * @retval NONE
*/
void timer_stop(const nrfx_timer_t* const p_timer_instance);

/**@brief 检查定时器是否运行
 * 
 * @param[in] p_timer_instance
 * 
 * @retval true
 *         false
*/
bool is_timer_start(const nrfx_timer_t* const p_timer_instance);

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
					  nrfx_timer_event_handler_t timer_evt_handler);
					  
					  
/**@brief apptimer 总体初始化函数
*/
void apptimer_init(void);

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
								app_timer_timeout_handler_t	timeout_handler);

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
								void*			p_context);

/**@brief 停止软件定时器
 * 
 * @param[in] timer_id 			软件定时器实例
 * 
 * @retval NRF_STATUS
*/
ret_code_t hal_app_timer_stop(app_timer_id_t	timer_id);

#endif // __HAL_TIMER_H__

/* */
