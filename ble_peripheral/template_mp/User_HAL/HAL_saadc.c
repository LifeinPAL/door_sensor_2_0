/**@file	HAL_saadc.c
 * 
 * @brief	本文件为saadc相关功能的硬件抽象层文件
 *
 * @detail	本文件包含application使用saadc相关功能的接口函数。
 *			同时作为nrf5 SDK 的上层封装
 */
 
/**< private include. */
#include "HAL_saadc.h"

 
/**@brief 初始化SAADC
 * 
 * @param[in] p_init 初始化saadc参数结构体指针
 *            saadc_evt_handler 定时器事件处理函数 
 * 
 * @retval	NRF_SUCCESS;
*/
ret_code_t saadc_init(const HAL_ssadc_init_t*  p_init,  
					  nrf_drv_saadc_event_handler_t saadc_evt_handler)
{
	ret_code_t err_code = NRF_SUCCESS;

	nrf_saadc_channel_config_t channel_config = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(p_init->SAADC_INPUT);
	err_code = nrf_drv_saadc_init(NULL, saadc_evt_handler);
	APP_ERROR_CHECK(err_code);

	err_code = nrf_drv_saadc_channel_init(p_init->saadc_channel, &channel_config);
	APP_ERROR_CHECK(err_code);

	return err_code;
}

/**@brief 执行阻塞采样并返回采样平均值
 * 
 * @param[in] num 采样次数
 * 
 * @retval	adc_value_mean;
*/
int32_t saadc_samples_blocking_mean(uint8_t num)
{
	ret_code_t err_code = NRF_SUCCESS;
	nrf_saadc_value_t	adc_value = 0;
	int32_t	adc_value_mean = 0;

	for (size_t i = 0; i < num; i++)
		{
			err_code = nrfx_saadc_sample_convert(0, &adc_value);
			APP_ERROR_CHECK(err_code);
			adc_value_mean += (int32_t)adc_value;
		}
	
	/**< VOLTAGE(P) - VOLTAGE(N) = RESULT * (REFERENCE / GAIN) / 2^RESOLUTION */
	return ((adc_value_mean / num) * (REFERANCE_VOLTAGE_IN_MILIVOLTS * SAADC_GAIN_RECIPROCAL)) >> SAADC_RESOLUTION; 
}



/**/
