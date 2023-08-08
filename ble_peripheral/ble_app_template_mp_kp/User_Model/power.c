/**@file power.c
 *
 * @brief ��Դ����
 * 
 * @version 2.0.0.alpha.20230717
 *
 */
 
/**< Private include */

#include "model_log.h"
#include "power.h"



/**< static variable*/


/**@brief ��ʼ����Դ����
*/
void power_management_init(void)
{
	ret_code_t err_code;

	err_code = nrf_pwr_mgmt_init();
	APP_ERROR_CHECK(err_code);
}

/**@brief �ػ���ִ�е�Դָ��
 *
 * @param[in]	nrf_pwr_mgmt_shutdown_t shutdown_type
 *
 * @retval NONE
 */
void SHUTDOWN_TO(nrf_pwr_mgmt_shutdown_t shutdown_type)
{
	nrf_pwr_mgmt_shutdown(shutdown_type);
}

/**@brief �������״̬
 *
 * @note ϵͳ��Ȼ��system-on״̬
 *
 * @retval NONE
 */
void IDLE_ENTER(void)
{
	nrf_pwr_mgmt_run();
}

/**@brief ��ȡ���θ�λԭ�򣬲����
 * 
 * @retval reset_reason	��λԭ��
*/
u32 power_reset_reason_get(void)
{
	ret_code_t err_code;
	u32 reset_reason;
	err_code = sd_power_reset_reason_get(&reset_reason);
	APP_ERROR_CHECK(err_code);
//	LOGINFO("pwr_rst_rsn: 0x%x", reset_reason);

	err_code = sd_power_reset_reason_clr(reset_reason);
	APP_ERROR_CHECK(err_code);
	return reset_reason;
}

