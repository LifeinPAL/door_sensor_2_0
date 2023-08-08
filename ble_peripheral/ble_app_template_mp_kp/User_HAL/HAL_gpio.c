/**@file	HAL_gpio.c
 * 
 * @brief	本文件为gpio相关功能的硬件抽象层文件
 *			led配置同属gpio相关功能
 *
 * @detail	本文件包含application使用gpio相关功能的接口函数。
 *			同时作为nrf5 SDK 的上层封装
 */
 
/**< private include. */
#include "main.h"
#include "HAL_gpio.h"

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
					   bool is_int_enable)
{
	ret_code_t err_code = NRF_SUCCESS;
	
	err_code = nrf_drv_gpiote_init();
    APP_ERROR_CHECK(err_code);
	
	if (p_config != NULL)
    {
    	UNUSED_PARAMETER(default_config);
		err_code = nrf_drv_gpiote_in_init(pin, p_config, gpiote_evt_handler);
		APP_ERROR_CHECK(err_code);
    }
	else
	{
		switch (default_config)
        {			
        	case SENSE_LOWTOHIGH:
			{
				nrf_drv_gpiote_in_config_t _in_config = GPIOTE_CONFIG_IN_SENSE_LOTOHI(false);
				err_code = nrf_drv_gpiote_in_init(pin, &_in_config, gpiote_evt_handler);				
			}break;
        	case SENSE_HIGHTOLOW:
			{
				nrf_drv_gpiote_in_config_t _in_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(false);
				err_code = nrf_drv_gpiote_in_init(pin, &_in_config, gpiote_evt_handler);
			}break;
			case SENSE_TOGGLE:
			{
				nrf_drv_gpiote_in_config_t _in_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(false);
				err_code = nrf_drv_gpiote_in_init(pin, &_in_config, gpiote_evt_handler);
			}break;
        }
		APP_ERROR_CHECK(err_code);
	}
	nrf_drv_gpiote_in_event_enable(pin, is_int_enable);
	
	return err_code;
}

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
void hal_gpio_std_output_init(nrfx_gpiote_pin_t pin_num)
{
	nrf_gpio_cfg_output(pin_num);
}

/**@brief	gpiote反初始化
 * 
 * @detail	 
 *
 * @param[in]	pin_num	引脚号
 *
 * @retval	NONE
 */
void hal_gpiote_uninit(nrfx_gpiote_pin_t pin)
{
	nrfx_gpiote_in_uninit(pin);
}

/**@brief 判断gpio模块是否已经初始化
 *
 * @detail
 *
 * @retval	true
 * 			false
 */
bool is_gpiote_init(void)
{
	return nrfx_gpiote_is_init();
}







