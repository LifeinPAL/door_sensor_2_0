/**@file HAL_gpio.h
 *
 * @brief gpio用户硬件抽象层头文件
 */
 
#ifndef __HAL_GPIO_H__
#define __HAL_GPIO_H__

/* private include */
#include "nrf_drv_gpiote.h"

/* private typedef */
typedef enum {
	SENSE_LOWTOHIGH,
	SENSE_HIGHTOLOW,
	SENSE_TOGGLE
}gpiote_config_default_in_t;

/* private define */
#define h_pin_num_t	uint32_t

/* export macro */

/** @brief Macro for mapping port and pin numbers to values understandable for SDK. */
#define HAL_PIN_MAP(_port, _pin)	\
		NRF_GPIO_PIN_MAP(_port, _pin)

/**@brief	Macro for set pin to high.
 *
 * @param[in]	_pin_num	Number of pin to be set.
 */
#define HAL_GPIO_HIGH(_pin_num)	\
		nrf_gpio_pin_set(_pin_num)
		
/**@brief	Macro for set pin to low.
 *
 * @param[in]	_pin_num	Number of pin to be set.
 */
#define HAL_GPIO_LOW(_pin_num)		\
		nrf_gpio_pin_clear(_pin_num)
		
/**@brief	Macro for set pin to toggle.
 *
 * @param[in]	_pin_num	Number of pin to be set.
 */
#define HAL_GPIO_TOGGLE(_pin_num)	\
		nrf_gpio_pin_toggle(_pin_num)

/**@brief	Macro for get pin input level.
 *
 * @param[in]	_pin_num	Number of pin to get.
 */
#define HAL_GPIO_READ(_pin_num)	\
		nrf_gpio_pin_read(_pin_num)
		
/**@brief	初始化IO引脚为标准模式输出
 * 
 * @detail	引脚状态：
 *			DIR 	--> OUTPUT
 *			INPUT 	--> DISCONNECT
 *			PULL 	--> NO PULL
 * 			DRIVE 	--> STANDARD 0, STANDARD 1
 *			SENSE 	--> NO SENSE 
 *
 *@param[in]	pin_num	引脚号
 *
 *@retval	NONE
 */
void hal_gpio_std_output_init(nrfx_gpiote_pin_t pin_num);

/**@brief	gpiote反初始化
 * 
 * @detail	 
 *
 * @param[in]	pin_num	引脚号
 *
 * @retval	NONE
 */
void hal_gpiote_uninit(nrfx_gpiote_pin_t pin);

/**@brief 判断gpio模块是否已经初始化
 *
 * @detail
 *
 * @retval	true
 * 			false
 */
bool is_gpiote_init(void);

/* export variable */


/* export function prototype. */

/**@brief 初始化gpiote输入检测功能
 *
 * @detail
 *
 * @param[in]	pin					将要初始化的引脚号
 *				p_config			gpiote配置结构体指针
 *				default_config		默认gpiote配置，当p_config为空时执行
 *				gpiote_evt_handler	gpiote事件处理函数
 *   			is_int_enable		是否开启gpiote中断
 *
 * @retval	NRF_SUCCESS				执行成功
 *			NRF_ERROR_INVALID_PARAM	默认配置参数错误
 */
ret_code_t gpiote_init(nrfx_gpiote_pin_t pin,
					   nrf_drv_gpiote_in_config_t* p_config,
					   gpiote_config_default_in_t default_config,
					   nrfx_gpiote_evt_handler_t gpiote_evt_handler,
					   bool is_int_enable);


#endif // __HAL_GPIO_H__


