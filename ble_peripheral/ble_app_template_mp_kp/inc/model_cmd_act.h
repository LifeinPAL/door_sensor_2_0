/**@file model_cmd_act.h
 *
 * @brief �Զ�������ģ�����ݶ�����API�ӿ�
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
#define CMD_PARAM_BUFFER_SIZE 32 /* �����ȸ���һ�������ݴ�������������飬�����СΪ�̶�32�ֽڣ����Ǻ����Ƿ�������ڴ�ض�̬���� */
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

/**@brief ������Ч����
 *
 * @detail ����ָ���Ӧ���������Ҫ���ݵ�ָ���Ӧ������
 *
 * @param[in] p_ctrl	�����ֶ����ݣ�ȡ��src_id, des_id, payload_len, payload
 *
 * @param[out] p_comm_param	��Ҫ���ݵĲ���������id, src, context ��Ҫѹ�����
 *
 * @retval true or false
*/
bool payload_resolve(protocol_ctrl_t* p_ctrl, comm_param_t* p_comm_param);

/**@brief ��Ӧ��������ת
 *
 * @param[in] p_p_comm_param
 *
 * @retval NONE
*/
void rsp_jump(comm_param_t* p_comm_param);

/**@brief ָ�������ת
 *
 * @param[in] p_p_comm_param
 *
 * @retval NONE
*/
void cmd_jump(comm_param_t* p_comm_param);

///**@brief ����Ч���ݴӴ��ڻ��帴�Ƶ����������Ӧ�Ĵ���������
// * 
// * @param[in] p_data �����Ƶ�����
// * @param[in] size   �����Ƶ����ݴ�С
// * 
// * @retval
//*/
//void crd_copy(const u8* p_data, u8 size);

///**@brief ������Ӧ���͵�����
// * 
//*/
//void rsp_process(u8* p_rsp_data);

///**brief ���������ȡ����
// * 
// * param[in] p_ctrl_data
// * param[out] p_cmd_param
// * 
// * retval NONE
//*/
//void cmd_resove(const protocol_ctrl_t* p_ctrl_data, cmd_comm_param_t* p_cmd_param);

/**@brief �����ͨ�ø�ʽ��Ӧ
 * 
 * @param[in] p_cmd_param ��������ṹ��
 * @param[in] p_gen_rsp ��Ӧ�ַ�����һ��Ϊ
 *                      "success" "failed" "timeout" "busy" ...
 * @param[in] len   ��Ӧ�ַ�������
 * 
 * @retval NONE
*/
void cmd_general_rsp(const comm_param_t* p_cmd_param, const char* p_gen_rsp, const u8 len);

///**@brief ����صĳ�ʱ��ʱ����ʼ��
// * 
//*/
//void app_timer_init_bond(void);

#endif // __MODEL_CMDACT_H__

