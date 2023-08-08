/**@file model_data_trs.c
 *
 * @brief 用于处理与蓝牙主机交互的数据包的发送，等待应答，存储
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





/**@brief 按大端模式组包
 * 
 * @param[in] data 要组包的变量
 * @param[out] p_pack 变量存储的位置
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

/**@brief 按大端模式解包
 * 
 * @param[in] p_pack 变量存储的位置
 * 
 * @retval data 解包的变量
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


/**@brief 将字符串中的大写字母转换成小写字母 
 * 
 * @note 此函数是c库函数tolower的封装
*/
void stringtolower(char* p_des, const char* p_src, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        *(p_des + i) = tolower(*(p_src + i));
    }    
}

