/**@file	HAL_watchdog.c
 * 
 * @brief	���ļ�Ϊ���Ź���ع��ܵ�Ӳ��������ļ�
 *			led����ͬ��gpio��ع���
 *
 * @detail	���ļ�����applicationʹ��gpio��ع��ܵĽӿں�����
 *			ͬʱ��Ϊnrf5 SDK ���ϲ��װ
 */
 
/**< private include. */
#include "HAL_watchdog.h"

/**@brief ��ʼ�����Ź�ģ��
 *
 * @detail
 *
 * @param[in]	p_channel_id				���Ź�ͨ����ָ��
 *				nrf_wdt_event_handler_t		���Ź��жϴ�����
 *   			is_enable					�Ƿ������Ź�
 *
 * @retval	NRF_SUCCESS				ִ�гɹ�
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

/**@brief ��ͨ��ι��
 * 
 * @param[in] _channel_id ι��ͨ����
 * 
 * @retval NONE
*/
void watchdog_feed(nrf_drv_wdt_channel_id _channel_id)
{
    nrf_drv_wdt_channel_feed(_channel_id);
}




/**/
