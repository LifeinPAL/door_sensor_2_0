/**@file power.h
 *
 * @brief 日志模块sdk API接口封装
 */
 
#ifndef __POWER_H__
#define __POWER_H__

/**< private include */
#include "common_abbr.h"
#include "nrf_pwr_mgmt.h"

/* export typedef */
typedef enum{
    RESETREAS_PIN = 0x01,
    RESETREAS_DOG = 0x02,
    RESETREAS_SOFT = 0x04,
    RESETREAS_LOCKUP = 0x08
}reset_reason_t;

/**< export macro */


/**< export function prototype */

/**@brief 关机并执行电源指令
 *
 * @param[in]	nrf_pwr_mgmt_shutdown_t shutdown_type
 *
 * @retval NONE
 */
void SHUTDOWN_TO(nrf_pwr_mgmt_shutdown_t shutdown_type);

/**@brief 进入空闲状态
 *
 * @note 系统依然在system-on状态
 *
 * @retval NONE
 */
void IDLE_ENTER(void);

/**@brief 初始化电源管理
*/
void power_management_init(void);

/**@brief 获取本次复位原因，并清除
 * 
 * @retval reset_reason	复位原因
*/
u32 power_reset_reason_get(void);

#endif // __POWER_H__

