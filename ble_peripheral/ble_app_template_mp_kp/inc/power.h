/**@file power.h
 *
 * @brief ��־ģ��sdk API�ӿڷ�װ
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

/**@brief �ػ���ִ�е�Դָ��
 *
 * @param[in]	nrf_pwr_mgmt_shutdown_t shutdown_type
 *
 * @retval NONE
 */
void SHUTDOWN_TO(nrf_pwr_mgmt_shutdown_t shutdown_type);

/**@brief �������״̬
 *
 * @note ϵͳ��Ȼ��system-on״̬
 *
 * @retval NONE
 */
void IDLE_ENTER(void);

/**@brief ��ʼ����Դ����
*/
void power_management_init(void);

/**@brief ��ȡ���θ�λԭ�򣬲����
 * 
 * @retval reset_reason	��λԭ��
*/
u32 power_reset_reason_get(void);

#endif // __POWER_H__

