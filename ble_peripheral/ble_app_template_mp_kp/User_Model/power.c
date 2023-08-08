/**@file power.c
 *
 * @brief 电源管理
 * 
 * @version 2.0.0.alpha.20230717
 *
 */
 
/**< Private include */

#include "model_log.h"
#include "power.h"



/**< static variable*/


/**@brief 初始化电源管理
*/
void power_management_init(void)
{
	ret_code_t err_code;

	err_code = nrf_pwr_mgmt_init();
	APP_ERROR_CHECK(err_code);
}

/**@brief 关机并执行电源指令
 *
 * @param[in]	nrf_pwr_mgmt_shutdown_t shutdown_type
 *
 * @retval NONE
 */
void SHUTDOWN_TO(nrf_pwr_mgmt_shutdown_t shutdown_type)
{
	nrf_pwr_mgmt_shutdown(shutdown_type);
}

/**@brief 进入空闲状态
 *
 * @note 系统依然在system-on状态
 *
 * @retval NONE
 */
void IDLE_ENTER(void)
{
	nrf_pwr_mgmt_run();
}

/**@brief 获取本次复位原因，并清除
 * 
 * @retval reset_reason	复位原因
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

