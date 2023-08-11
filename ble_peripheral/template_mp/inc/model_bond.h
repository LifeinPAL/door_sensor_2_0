/**@file model_bond.h
 *
 * @brief ��־ģ��sdk API�ӿڷ�װ
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
#define SEC_PARAM_KDIST_OWN_ENC     1                       /**< �ַ��Լ���LTK */
#define SEC_PARAM_KDIST_OWN_ID      1                       /**< �ַ��Լ���IRK */
#define SEC_PARAM_KDIST_PEER_ENC    1                       /**< Ҫ��Է��ַ�LTK */    
#define SEC_PARAM_KDIST_PEER_ID     1                       /**< Ҫ��Է��ַ�IRK */

/* export variable */
extern U_FUN_STATUS fun_bond_status;


/**< export function prototype */

/**@brief Function for initialize peer manager module.
*/
void peer_manager_init(void);

/**@brief ���ص�ǰ�Ƿ��а󶨵��豸
 * 
 * @retval true or false
*/
bool is_bonded(void);

/**@brief �Ӵ洢��ɾ������Ϣ
 */
void delete_bonds(void);

/**@brief ��ȡ�����豸��ַ
 * 
 * @param[out]	p_local_addr ������豸��ַָ��
 * 
 * @retval NRF_STATUS	
*/
ret_code_t local_addr_get(u8* p_local_addr);

/**@brief ��ȡ�󶨵ĶԶ��豸��ַ
 * 
 * @param[out] p_addr �洢�豸��ַ������ָ��
 * @param[in] conn_handle Ҫ��ѯ�豸�����Ӿ��
*/
ret_code_t bond_addr_get(u8* p_addr, const u16 conn_handle);

#endif // __MODEL_BOND_H__

