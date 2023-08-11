/**@file model_ble.h
 *
 * @brief ����Э��ջӦ��ͷ�ļ�
 */
 
#ifndef __MODEL_BLE_H__
#define __MODEL_BLE_H__

/* private include */
#include "common_abbr.h"
#include "ble_conn_state.h"
#include "model_cmd_act.h"

/* export variable */
extern u16 g_central_conn_handle;
extern u16 g_manager_conn_handle;


/* export function prototype */

void ble_init(void);

/**@brief Function for starting advertising.
 */
void advertising_start(void);

/**@brief ��װnus��������ݷ��ͺ���
 * 
 * @param[in] p_data	��Ҫ���͵�����
 * @param[in] length	��Ҫ���͵����ݳ���
 * @param[in] conn_handle ���ݷ��͵ķ���
 * 
 * @retval NONE
*/
void ble_data_send(u8* p_data, u16 length, u16 conn_handle);

/**@brief ���յ�auth��rsp���ݰ��Ĵ�����
 * 
 * @param[in] ָ��rsp���ݰ���auth��Ӧ����Ӧ����
*/
void rsp_auth_handler(comm_param_t* p_context);



#endif // __MODEL_BLE_H__





