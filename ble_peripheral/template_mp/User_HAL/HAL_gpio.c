/**@file	HAL_gpio.c
 * 
 * @brief	���ļ�Ϊgpio��ع��ܵ�Ӳ��������ļ�
 *			led����ͬ��gpio��ع���
 *
 * @detail	���ļ�����applicationʹ��gpio��ع��ܵĽӿں�����
 *			ͬʱ��Ϊnrf5 SDK ���ϲ��װ
 */
 
/**< private include. */
#include "main.h"
#include "HAL_gpio.h"

/**@brief ��ʼ��gpiote�����⹦��
 *
 * @detail
 *
 * @param[in]	pin					��Ҫ��ʼ�������ź�
 *				p_config			gpiote���ýṹ��ָ��
 *				default_config		Ĭ��gpiote���ã���p_configΪ��ʱִ��
 *				gpiote_evt_handler	gpiote�¼�������
 *   			is_int_enable		�Ƿ���gpiote�ж�
 *
 * @retval	NRF_SUCCESS				ִ�гɹ�
 *			NRF_ERROR_INVALID_PARAM	Ĭ�����ò�������
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

/**@brief	��ʼ��IO����Ϊ��׼ģʽ���
 * 
 * @detail	����״̬��
 *			DIR 	--> OUTPUT
 *			INPUT 	--> DISCONNECT
 *			PULL 	--> NO PULL
 * 			DRIVE 	--> STANDARD 0, STANDARD 1
 *			SENSE 	--> NO SENSE 
 *
 *@param[in]	pin_num	���ź�
 *
 *@retval	NONE
 */
void hal_gpio_std_output_init(nrfx_gpiote_pin_t pin_num)
{
	nrf_gpio_cfg_output(pin_num);
}

/**@brief	gpiote����ʼ��
 * 
 * @detail	 
 *
 * @param[in]	pin_num	���ź�
 *
 * @retval	NONE
 */
void hal_gpiote_uninit(nrfx_gpiote_pin_t pin)
{
	nrfx_gpiote_in_uninit(pin);
}

/**@brief �ж�gpioģ���Ƿ��Ѿ���ʼ��
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







