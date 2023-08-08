/**@file model_data_trs.c
 *
 * @brief ���ڴ����������������������ݰ��ķ��ͣ��ȴ�Ӧ�𣬴洢
 * 
 * @version 2.0.0.alpha.20230717
 *
 */
 
/**< Private include */
#include "data_transfer.h"
#include <string.h>


/* private define */


/* global variable */




/* static variable */





/**@brief �����ģʽ���
 * 
 * @param[in] data Ҫ����ı���
 * @param[out] p_pack �����洢��λ��
 * 
 * @retval NONE
*/
void pack_bigendian16(const u16 data, u8* p_pack)
{
    *p_pack = (data >> 8) & 0xFF;
    *(p_pack + 1) = data & 0xFF;
}

void pack_bigendian32(const u32 data, u8* p_pack)
{
    for (size_t i = 0; i < 4; i++)
    {
        *(p_pack + i) = (u8)((data >> ((3 - i) * 8)) & 0xFF);
    }
    
}

/**@brief �����ģʽ���
 * 
 * @param[in] p_pack �����洢��λ��
 * 
 * @retval data ����ı���
*/
u16 unpack_bigendian16(const u8* p_pack)
{
    u16 ret = 0;
    ret = (*p_pack << 8) | *(p_pack + 1);
    return ret;
}

u32 unpack_bigendian32(const u8* p_pack)
{
    u32 ret = 0;
    for (size_t i = 0; i < 4; i++)
    {
        ret = (ret << 8) | *(p_pack + i);
    }    
    return ret;
}


/**@brief ���ַ����еĴ�д��ĸת����Сд��ĸ 
 * 
 * @note �˺�����c�⺯��tolower�ķ�װ
*/
void stringtolower(char* p_des, const char* p_src, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        *(p_des + i) = tolower(*(p_src + i));
    }    
}

