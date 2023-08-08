/**@file HAL_timer.h
 *
 * @brief ��ʱ���û�Ӳ�������ͷ�ļ�
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



/**@brief ��ʼ��ʱ��
 * 
 * @param[in] p_timer_instance
 * 
 * @retval NONE
*/
void timer_start(const nrfx_timer_t* const p_timer_instance);

/**@brief ֹͣ��ʱ��
 * 
 * @param[in] p_timer_instance
 * 
 * @retval NONE
*/
void timer_stop(const nrfx_timer_t* const p_timer_instance);

/**@brief ��鶨ʱ���Ƿ�����
 * 
 * @param[in] p_timer_instance
 * 
 * @retval true
 *         false
*/
bool is_timer_start(const nrfx_timer_t* const p_timer_instance);

/**@brief ��ʼ��Ӳ����ʱ��
 * 
 * @param[in] p_timer_instance ��Ҫ��ʼ���Ķ�ʱ��ʵ��ָ��
 *			  p_init ��ʱ����ʼ�������ṹ��ָ��
 *            timer_evt_handler ��ʱ���¼������� 
 * 
 * @retval	NRF_SUCCESS;
*/
ret_code_t timer_init(const nrfx_timer_t * const p_timer_instance, 
					  const HAL_timer_init_t* p_init, 
					  nrfx_timer_event_handler_t timer_evt_handler);
					  
					  
/**@brief apptimer �����ʼ������
*/
void apptimer_init(void);

/**@brief ���������ʱ��
 * 
 * @param[in] p_timer_id 		�����ʱ��ʵ��ָ��
 * @param[in] mode		 		�����ʱ��ģʽ
 * @param[in] timeout_handler	�����ʱ����ʱ������
 * 
 * @retval NRF_STATUS
*/
ret_code_t hal_app_timer_create(const app_timer_id_t* 		p_timer_id,
								app_timer_mode_t			mode,
								app_timer_timeout_handler_t	timeout_handler);

/**@brief ���������ʱ��
 * 
 * @param[in] timer_id 			�����ʱ��ʵ��
 * @param[in] timeout_ms		��ʱʱ�䣨ms��
 * @param[in] p_context			��Ҫ���ݵĲ�����û����NULL
 * 
 * @retval NRF_STATUS
*/
ret_code_t hal_app_timer_start(app_timer_id_t	timer_id,
								u32				timeout_ms,
								void*			p_context);

/**@brief ֹͣ�����ʱ��
 * 
 * @param[in] timer_id 			�����ʱ��ʵ��
 * 
 * @retval NRF_STATUS
*/
ret_code_t hal_app_timer_stop(app_timer_id_t	timer_id);

#endif // __HAL_TIMER_H__

/* */
