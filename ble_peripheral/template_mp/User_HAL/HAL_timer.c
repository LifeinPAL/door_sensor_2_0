/**@file	HAL_timer.c
 * 
 * @brief	���ļ�Ϊtimer��ع��ܵ�Ӳ��������ļ�
 *
 * @detail	���ļ�����applicationʹ��timer��ع��ܵĽӿں�����
 *			ͬʱ��Ϊnrf5 SDK ���ϲ��װ
*/

/* private include */
#include "HAL_timer.h"
#include "app_error.h"


/* function defination */

/**@brief apptimer �����ʼ������
*/
void apptimer_init(void)
{
	ret_code_t err_code;
	err_code = app_timer_init();
	APP_ERROR_CHECK(err_code);
}

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
					  nrfx_timer_event_handler_t timer_evt_handler)
{
	ret_code_t err_code = NRF_SUCCESS;

	err_code = nrf_drv_timer_init(p_timer_instance, &p_init->timer_config, timer_evt_handler);
	APP_ERROR_CHECK(err_code);

	nrf_drv_timer_extended_compare(p_timer_instance,
								   p_init->cc_channel,
								   p_init->cc_value,
								   p_init->short_mask,
								   p_init->enable_init);
	
	return err_code;
}

/**@brief ��ʼ��ʱ��
 * 
 * @param[in] p_timer_instance
 * 
 * @retval NONE
*/
void timer_start(const nrfx_timer_t* const p_timer_instance)
{
    nrf_drv_timer_enable(p_timer_instance);
}


/**@brief ֹͣ��ʱ��
 * 
 * @param[in] p_timer_instance
 * 
 * @retval NONE
*/
void timer_stop(const nrfx_timer_t* const p_timer_instance)
{
    nrf_drv_timer_disable(p_timer_instance);
}

/**@brief ��鶨ʱ���Ƿ�����
 * 
 * @param[in] p_timer_instance
 * 
 * @retval true
 *         false
*/
bool is_timer_start(const nrfx_timer_t* const p_timer_instance)
{
    return nrf_drv_timer_is_enabled(p_timer_instance);
}

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
								app_timer_timeout_handler_t	timeout_handler)
{
	return app_timer_create(p_timer_id, mode, timeout_handler);
}

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
								void*			p_context)
{
	return app_timer_start(timer_id, APP_TIMER_TICKS(timeout_ms), p_context);
}

/**@brief ֹͣ�����ʱ��
 * 
 * @param[in] timer_id 			�����ʱ��ʵ��
 * 
 * @retval NRF_STATUS
*/
ret_code_t hal_app_timer_stop(app_timer_id_t	timer_id)
{
	return app_timer_stop(timer_id);
}

/**/
