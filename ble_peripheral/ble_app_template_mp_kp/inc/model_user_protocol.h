/**@file model_user_protocol.h
 *
 * @brief 传感器采集数据的发送，接收（应答），存储
 */
 
#ifndef __USER_PROTOCOL_H__
#define __USER_PROTOCOL_H__

/**< private include */
#include "common_abbr.h"
#include "sdk_config.h"
#include <stdbool.h>

/* export define */

#define PROTOCOL_FRAME_HEAD_SIZE    2
#define PROTOCOL_PRORPERTY_SIZE     1
#define PROTOCOL_ORIGINAL_ID_SIZE   1
#define PROTOCOL_DESTINATION_ID_SIZE    1
#define PROTOCOL_PAYLOAD_LENGTH_SIZE    1
#define PROTOCOL_FRAME_END_SIZE     2

#define PROTOCOL_FRAME_HEAD_OFFSET 0
#define PROTOCOL_PRORPERTY_OFFSET       (PROTOCOL_FRAME_HEAD_OFFSET +       \
                                        PROTOCOL_FRAME_HEAD_SIZE)
#define PROTOCOL_ORIGINAL_ID_OFFSET     (PROTOCOL_PRORPERTY_OFFSET +        \
                                        PROTOCOL_PRORPERTY_SIZE)
#define PROTOCOL_DESTINATION_ID_OFFSET  (PROTOCOL_ORIGINAL_ID_OFFSET +      \
                                        PROTOCOL_ORIGINAL_ID_SIZE)
#define PROTOCOL_PAYLOAD_LENGTH_OFFSET  (PROTOCOL_DESTINATION_ID_OFFSET +   \
                                        PROTOCOL_DESTINATION_ID_SIZE)
#define PROTOCOL_PAYLOAD_OFFSET         (PROTOCOL_PAYLOAD_LENGTH_OFFSET +   \
                                        PROTOCOL_PAYLOAD_LENGTH_SIZE)
#define PROTOCOL_TOTAL_SIZE_MAX     NRF_SDH_BLE_GATT_MAX_MTU_SIZE - 3

#define PACKET_FRAME_HEAD	0xA4B5
#define PACKET_FRAME_END	0xAAED

#define PROPERTY_COMMOND    0xCC
#define PROPERTY_DATA       0xDD
#define PROPERTY_RESPONCE   0xAA

/* export typedef */

/* */
typedef struct protocol_ctrl_s
{
    u8 type;
    u8 org_id;
    u8 des_id;
    u8 payload_len;
    u8 payload[PROTOCOL_TOTAL_SIZE_MAX - 
              PROTOCOL_PAYLOAD_OFFSET - 
              PROTOCOL_FRAME_END_SIZE];
}protocol_ctrl_t;



/* export define */






/**< export macro */



/**< export function prototype */

/**@brief 用户协议层解包
* 
* @param[in] p_rx_buffer 接收数据缓冲
* @pqram[out] p_payload_buffer 解析数据放入的缓冲区
* @param[out] size 解析数据数据长度
* 
* @retval 
*/
bool user_protocol_unpack(const u8* p_rx_buffer, protocol_ctrl_t* p_pro_ctrl);

/**@brief 用户协议层组包
 * 
 * @param[in] p_packet 待发送数据包
 * @pqram[in] type 数据包类型
 * @param[in] des 数据包目的地址标识
 * @parma[in] payload_size 有效数据长度
 * 
 * @retval 
*/
void user_protocol_pack(u8* p_packet, u8 type, u8 des, u8 payload_size);



#endif // __USER_PROTOCOL_H__

