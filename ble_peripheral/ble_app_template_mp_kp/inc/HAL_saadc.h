/**@file HAL_saadc.h
 *
 * @brief 定时器用户硬件抽象层头文件
 */
 
#ifndef __HAL_SAADC_H__
#define __HAL_SAADC_H__

/* private include */
#include "nrf_drv_saadc.h"


/* private define */

/* export typedef */
typedef struct saadc_init_s
{
    nrf_saadc_input_t SAADC_INPUT;
    uint8_t saadc_channel;
}HAL_ssadc_init_t;




/* export macro */
#define REFERANCE_VOLTAGE_IN_MILIVOLTS 600
#define SAADC_GAIN_RECIPROCAL 6
#define SAADC_RESOLUTION 10
		


/* export variable */


/* export function prototype. */

/**@brief 阻塞模式执行一次ADC采样
 * 
 * @param[in] saadc_channel
 * @param[out] p_saadc_value
 * 
 * @retval NRF_SUCCESSS
*/
static inline ret_code_t saadc_sample_once_blocking(uint8_t saadc_channel, nrf_saadc_value_t* p_saadc_value)
{
    return nrfx_saadc_sample_convert(saadc_channel, p_saadc_value);
}

/**@brief 初始化SAADC
 * 
 * @param[in] p_init 初始化saadc参数结构体指针
 *            saadc_evt_handler 定时器事件处理函数 
 * 
 * @retval	NRF_SUCCESS;
*/
ret_code_t saadc_init(const HAL_ssadc_init_t*  p_init,  
					  nrf_drv_saadc_event_handler_t saadc_evt_handler);

/**@brief 执行阻塞采样并返回采样平均值
 * 
 * @param[in] num 采样次数
 * 
 * @retval	adc_value_mean;
*/
int32_t saadc_samples_blocking_mean(uint8_t num);

#endif // __HAL_SAADC_H__


