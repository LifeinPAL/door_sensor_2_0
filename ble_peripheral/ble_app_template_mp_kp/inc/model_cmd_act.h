/**@file model_cmd_act.h
 *
 * @brief 自定义日历模块数据定义与API接口
 */
 
#ifndef __MODEL_CMDACT_H__
#define __MODEL_CMDACT_H__

/**< private include */
#include "nrf_queue.h"
#include "common_abbr.h"
#include "model_user_protocol.h"

/* private typedef */

/* private define */
#define CMD_QUEUE_SIZE 5
#define RSP_QUEUE_SIZE 5
#define CMD_PARAM_BUFFER_SIZE 32 /* 这里先给了一个用于暂存命令参数的数组，数组大小为固定32字节，考虑后期是否可以用内存池动态分配 */
#define ACK_PARAM_BUFFER_SIZE 32
#define CMD_LIST_SIZE 12
#define RSP_LIST_SIZE 2
/* export typedef */
//typedef struct cmd_comm_param_s
//{
//	u8 cmd_id;
//	u16 cmd_conn_handle;
//    u8 cmd_src;
//    u8 cmd_param[CMD_PARAM_BUFFER_SIZE];
//}cmd_comm_param_t;

//typedef struct rsp_comm_param_s
//{
//	u8	rsp_id;
//	u16	rsp_conn_handle;
//	u8 	rsp_src;
//	u8	rsp_param[ACK_PARAM_BUFFER_SIZE];
//}rsp_comm_param_t;

typedef struct comm_param_s
{
	u8	id;
	u16	conn_handle;
	u8 	src;
	u8	context[ACK_PARAM_BUFFER_SIZE];
}comm_param_t;

typedef void (*gen_handler) (comm_param_t*);

//typedef void (*cmd_handle) (cmd_comm_param_t*);

typedef struct action_table_s
{
	const char* p_name;
	const gen_handler	handler;
}action_table_t;

///* export variable */
//extern const cmd_handle cmd_handle_list[CMD_LIST_SIZE];

///**< export macro */
//#define PAYLOAD_SIZE_MAX    200


/**< export function prototype */
NRF_QUEUE_INTERFACE_DEC(comm_param_t, cmd_queue);
NRF_QUEUE_INTERFACE_DEC(comm_param_t, rsp_queue);

/**@brief 解析有效数据
 *
 * @detail 根据指令（响应）表，组成需要传递的指令（响应）参数
 *
 * @param[in] p_ctrl	控制字段数据，取用src_id, des_id, payload_len, payload
 *
 * @param[out] p_comm_param	需要传递的参数，包括id, src, context 需要压入队列
 *
 * @retval true or false
*/
bool payload_resolve(protocol_ctrl_t* p_ctrl, comm_param_t* p_comm_param);

/**@brief 响应处理函数跳转
 *
 * @param[in] p_p_comm_param
 *
 * @retval NONE
*/
void rsp_jump(comm_param_t* p_comm_param);

/**@brief 指令处理函数跳转
 *
 * @param[in] p_p_comm_param
 *
 * @retval NONE
*/
void cmd_jump(comm_param_t* p_comm_param);

///**@brief 将有效数据从串口缓冲复制到处理命令、响应的待处理缓冲区
// * 
// * @param[in] p_data 待复制的数据
// * @param[in] size   待复制的数据大小
// * 
// * @retval
//*/
//void crd_copy(const u8* p_data, u8 size);

///**@brief 解析响应类型的数据
// * 
//*/
//void rsp_process(u8* p_rsp_data);

///**brief 解析命令获取参数
// * 
// * param[in] p_ctrl_data
// * param[out] p_cmd_param
// * 
// * retval NONE
//*/
//void cmd_resove(const protocol_ctrl_t* p_ctrl_data, cmd_comm_param_t* p_cmd_param);

/**@brief 命令的通用格式响应
 * 
 * @param[in] p_cmd_param 命令参数结构体
 * @param[in] p_gen_rsp 响应字符串，一般为
 *                      "success" "failed" "timeout" "busy" ...
 * @param[in] len   响应字符串长度
 * 
 * @retval NONE
*/
void cmd_general_rsp(const comm_param_t* p_cmd_param, const char* p_gen_rsp, const u8 len);

///**@brief 绑定相关的超时计时器初始化
// * 
//*/
//void app_timer_init_bond(void);

#endif // __MODEL_CMDACT_H__

