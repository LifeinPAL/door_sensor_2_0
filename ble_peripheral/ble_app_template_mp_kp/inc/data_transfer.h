/**@file model_data_trs.h
 *
 * @brief 传感器采集数据的发送，接收（应答），存储
 */
 
#ifndef __DATA_TRANSFER_H__
#define __DATA_TRANSFER_H__

/**< private include */
#include "common_abbr.h"
#include <string.h>
#include <ctype.h>


/* export typedef */

/* export define */





/**< export macro */



/**< export function prototype */

/**@brief 按大端模式组包
 * 
 * @param[in] data 要组包的变量
 * @param[out] p_pack 变量存储的位置
 * 
 * @retval NONE
*/
void pack_bigendian16(const u16 data, u8* p_pack);

void pack_bigendian32(const u32 data, u8* p_pack);

/**@brief 按大端模式解包
 * 
 * @param[in] p_pack 变量存储的位置
 * 
 * @retval data 解包的变量
*/
u16 unpack_bigendian16(const u8* p_pack);

u32 unpack_bigendian32(const u8* p_pack);

/**@brief 将字符串中的大写字母转换成小写字母 
 * 
 * @note 此函数是c库函数tolower的封装
*/
void stringtolower(char* p_des, const char* p_src, size_t size);




#endif // __DATA_TRANSFER_H__

