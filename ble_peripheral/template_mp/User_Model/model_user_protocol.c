/**@file model_user_protocol.c
 *
 * @brief ������ϵͳ��ͨ��Э��
 * 
 * @version 2.0.0.alpha.20230717
 *
 */
 
/**< Private include */

#include "model_user_protocol.h"
#include "data_transfer.h"


/* private define */




/* global variable */

/* static function */


/* export function define */

/**@brief �û�Э�����
 * 
 * @param[in] p_rx_buffer �������ݻ���
 * @pqram[out] p_payload_buffer �������ݷ���Ļ�����
 * @param[out] size �����������ݳ���
 * 
 * @retval 
*/
bool user_protocol_unpack(const u8* p_rx_buffer, protocol_ctrl_t* p_pro_ctrl)
{
    u8 fh_h = (u8)(PACKET_FRAME_HEAD >> 8);			/* ���ݰ�֡ͷ��8λ */
	u8 fh_l = (u8)PACKET_FRAME_HEAD;				/* ���ݰ�֡ͷ��8λ */ 
    const u8* p_protocol = p_rx_buffer; 
          
    /* ��ȡ֡ͷ��ַ */
    for (size_t i = 0; i < PROTOCOL_TOTAL_SIZE_MAX - 1; i++)
    {
        if ((*p_protocol == fh_h) && (*(p_protocol + 1) == fh_l))
        {
            break;
        }
        p_protocol++;
    }

     /* δ�ҵ�֡ͷʱ���ش���0 */
     if (p_protocol == p_rx_buffer + PROTOCOL_TOTAL_SIZE_MAX -1)
     {
        return false;
     }
    p_pro_ctrl->type = *(p_rx_buffer + PROTOCOL_PRORPERTY_OFFSET);
    p_pro_ctrl->org_id = *(p_rx_buffer + PROTOCOL_ORIGINAL_ID_OFFSET);
    p_pro_ctrl->des_id = *(p_rx_buffer + PROTOCOL_DESTINATION_ID_OFFSET);
    p_pro_ctrl->payload_len = *(p_rx_buffer + PROTOCOL_PAYLOAD_LENGTH_OFFSET);
    memcpy(p_pro_ctrl->payload, p_rx_buffer + PROTOCOL_PAYLOAD_OFFSET, p_pro_ctrl->payload_len);

    return true; 
}

/**@brief �û�Э������
 * 
 * @param[in] p_packet ���������ݰ�
 * @pqram[in] type ���ݰ�����
 * @param[in] des ���ݰ�Ŀ�ĵ�ַ��ʶ
 * @parma[in] payload_size ��Ч���ݳ���
 * 
 * @retval 
*/
void user_protocol_pack(u8* p_packet, u8 type, u8 des, u8 payload_size)
{
    pack_bigendian16(PACKET_FRAME_HEAD, p_packet);
    *(p_packet + PROTOCOL_PRORPERTY_OFFSET) = type;
    *(p_packet + PROTOCOL_ORIGINAL_ID_OFFSET) = 0x02;
    *(p_packet + PROTOCOL_DESTINATION_ID_OFFSET) = des;
    *(p_packet + PROTOCOL_PAYLOAD_LENGTH_OFFSET) = payload_size;
    pack_bigendian16(PACKET_FRAME_END, p_packet + PROTOCOL_PAYLOAD_OFFSET + payload_size);
}


