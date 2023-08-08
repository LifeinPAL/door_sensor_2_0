/**@file HAL_flash.h
 *
 * @brief gpio用户硬件抽象层头文件
 */
 
#ifndef __HAL_FLASH_H__
#define __HAL_FLASH_H__

/* private include */

#include "fds.h"
#include "common_abbr.h"
#include "model_data_trs.h"

/* private typedef */


/* private define */

/* export macro */

/* export function prototype */

/**@brief 初始化flash操作模块
 *
 * @detail
 *
 * @param[in]	
 *
 * @retval
 */
ret_code_t HAL_flash_init(void);

/**@brief 获取某文件中的记录数量
 * 
 * @param[in] file_id 文件标识号
 * 
 * @retval count 文件中的记录数量
*/
u8 record_inflie_countget(u16 file_id);

/**@brief 向某文件中存储一个记录
 * 
 * @param[in] file_id 文件标识号
 * 
 * @retval NRF_STATUS
*/
ret_code_t doordata_record_push(u16 file_id, door_data_t* p_data, u32 size);


/**@brief 从文件中按记录号读取
 * 
 * @param[in] file_id 读取的文件id号
 * @param[in] record_key 读取的记录号
 * @param[out] p_data 存储读取的数据
 * @param[in] size 读取的数据大小
*/
ret_code_t doordata_record_read(u16 file_id, u16 record_key, door_data_t* p_data, size_t size);

/**@brief 删除某文件中的全部记录 
 * 
 * @param[in] file_id 文件号
*/
void HAL_file_del(const u16 file_id);

/**@brief 文件中移动记录
 * 
 * @param[in] file_id 文件ID号
 * @param[in] key_des 目标记录号
 * @param[in] key_src 源记录号
*/
void doordata_record_move(u16 file_id, u16 key_des, u16 key_src);

/**@brief 删除某文件中的全部记录 
 * 
 * @param[in] file_id 文件号
 * @param[in] record_key 记录号
*/
void HAL_record_del(const u16 file_id, const u16 record_key);

#endif // __HAL_FLASH_H__


