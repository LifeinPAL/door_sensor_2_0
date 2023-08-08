/**@file HAL_watchdog.h
 *
 * @brief ��ʱ���û�Ӳ�������ͷ�ļ�
 */
 
#ifndef __HAL_WATCHDOG_H__
#define __HAL_WATCHDOG_H__

/* private include */

#include "nrf_drv_wdt.h"


/* private define */

/* export typedef */





/* export macro */

		


/* export variable */


/* export function prototype. */

/**@brief ��ͨ��ι��
 * 
 * @param[in] _channel_id ι��ͨ����
 * 
 * @retval NONE
*/
void watchdog_feed(nrf_drv_wdt_channel_id _channel_id);


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
					   bool is_enable);


#endif // __HAL_WATCHDOG_H__


