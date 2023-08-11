/**@file	HAL_saadc.c
 * 
 * @brief	���ļ�Ϊsaadc��ع��ܵ�Ӳ��������ļ�
 *
 * @detail	���ļ�����applicationʹ��saadc��ع��ܵĽӿں�����
 *			ͬʱ��Ϊnrf5 SDK ���ϲ��װ
 */
 
/**< private include. */
#include "HAL_saadc.h"

 
/**@brief ��ʼ��SAADC
 * 
 * @param[in] p_init ��ʼ��saadc�����ṹ��ָ��
 *            saadc_evt_handler ��ʱ���¼������� 
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

/**@brief ִ���������������ز���ƽ��ֵ
 * 
 * @param[in] num ��������
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
