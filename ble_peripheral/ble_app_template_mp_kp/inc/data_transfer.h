/**@file model_data_trs.h
 *
 * @brief �������ɼ����ݵķ��ͣ����գ�Ӧ�𣩣��洢
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

/**@brief �����ģʽ���
 * 
 * @param[in] data Ҫ����ı���
 * @param[out] p_pack �����洢��λ��
 * 
 * @retval NONE
*/
void pack_bigendian16(const u16 data, u8* p_pack);

void pack_bigendian32(const u32 data, u8* p_pack);

/**@brief �����ģʽ���
 * 
 * @param[in] p_pack �����洢��λ��
 * 
 * @retval data ����ı���
*/
u16 unpack_bigendian16(const u8* p_pack);

u32 unpack_bigendian32(const u8* p_pack);

/**@brief ���ַ����еĴ�д��ĸת����Сд��ĸ 
 * 
 * @note �˺�����c�⺯��tolower�ķ�װ
*/
void stringtolower(char* p_des, const char* p_src, size_t size);




#endif // __DATA_TRANSFER_H__

