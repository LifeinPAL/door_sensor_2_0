/**@file model_bond.h
 *
 * @brief 日志模块sdk API接口封装
 */
 
#ifndef __MODEL_BOND_H__
#define __MODEL_BOND_H__

/**< private include */
#include "common_abbr.h"
#include "main.h"
#include "app_error.h"

/**< export macro */


/**< private define */
#define SEC_PARAM_BOND              1                       /**< Perform bonding. */
#define SEC_PARAM_MITM              0                       /**< Man in the middle protection not required. */
#define SEC_PARAM_LESC              0                       /**< LE secure connections not enabled. */
#define SEC_PARAM_KEYPRESS          0                       /**< Keypress notifications not evabled. */
#define SEC_PARAM_IO_CAPABILITIES   BLE_GAP_IO_CAPS_NONE    /**< No I/O capabilities. */
#define SEC_PARAM_OOB               0                       /**< Out of band data not available. */
#define SEC_PARAM_MIN_KEY_SIZE      7                       /**< Minimum encryption key size. */
#define SEC_PARAM_MAX_KEY_SIZE      16                      /**< Maximum encryption key size. */
#define SEC_PARAM_KDIST_OWN_ENC     1                       /**< 分发自己的LTK */
#define SEC_PARAM_KDIST_OWN_ID      1                       /**< 分发自己的IRK */
#define SEC_PARAM_KDIST_PEER_ENC    1                       /**< 要求对方分发LTK */    
#define SEC_PARAM_KDIST_PEER_ID     1                       /**< 要求对方分发IRK */

/* export variable */
extern U_FUN_STATUS fun_bond_status;


/**< export function prototype */

/**@brief Function for initialize peer manager module.
*/
void peer_manager_init(void);

/**@brief 返回当前是否有绑定的设备
 * 
 * @retval true or false
*/
bool is_bonded(void);

/**@brief 从存储中删除绑定信息
 */
void delete_bonds(void);

/**@brief 获取本地设备地址
 * 
 * @param[out]	p_local_addr 带填充设备地址指针
 * 
 * @retval NRF_STATUS	
*/
ret_code_t local_addr_get(u8* p_local_addr);

/**@brief 获取绑定的对端设备地址
 * 
 * @param[out] p_addr 存储设备地址的数组指针
 * @param[in] conn_handle 要查询设备的连接句柄
*/
ret_code_t bond_addr_get(u8* p_addr, const u16 conn_handle);

#endif // __MODEL_BOND_H__

