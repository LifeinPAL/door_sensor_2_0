/**@file model_ble.h
 *
 * @brief 蓝牙协议栈应用头文件
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

/**@brief 封装nus服务的数据发送函数
 * 
 * @param[in] p_data	将要发送的数据
 * @param[in] length	将要发送的数据长度
 * @param[in] conn_handle 数据发送的方向
 * 
 * @retval NONE
*/
void ble_data_send(u8* p_data, u16 length, u16 conn_handle);

/**@brief 接收到auth的rsp数据包的处理函数
 * 
 * @param[in] 指向rsp数据包中auth响应的响应参数
*/
void rsp_auth_handler(comm_param_t* p_context);



#endif // __MODEL_BLE_H__





