/**@file HAL_flash.h
 *
 * @brief gpio�û�Ӳ�������ͷ�ļ�
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

/**@brief ��ʼ��flash����ģ��
 *
 * @detail
 *
 * @param[in]	
 *
 * @retval
 */
ret_code_t HAL_flash_init(void);

/**@brief ��ȡĳ�ļ��еļ�¼����
 * 
 * @param[in] file_id �ļ���ʶ��
 * 
 * @retval count �ļ��еļ�¼����
*/
u8 record_inflie_countget(u16 file_id);

/**@brief ��ĳ�ļ��д洢һ����¼
 * 
 * @param[in] file_id �ļ���ʶ��
 * 
 * @retval NRF_STATUS
*/
ret_code_t doordata_record_push(u16 file_id, door_data_t* p_data, u32 size);


/**@brief ���ļ��а���¼�Ŷ�ȡ
 * 
 * @param[in] file_id ��ȡ���ļ�id��
 * @param[in] record_key ��ȡ�ļ�¼��
 * @param[out] p_data �洢��ȡ������
 * @param[in] size ��ȡ�����ݴ�С
*/
ret_code_t doordata_record_read(u16 file_id, u16 record_key, door_data_t* p_data, size_t size);

/**@brief ɾ��ĳ�ļ��е�ȫ����¼ 
 * 
 * @param[in] file_id �ļ���
*/
void HAL_file_del(const u16 file_id);

/**@brief �ļ����ƶ���¼
 * 
 * @param[in] file_id �ļ�ID��
 * @param[in] key_des Ŀ���¼��
 * @param[in] key_src Դ��¼��
*/
void doordata_record_move(u16 file_id, u16 key_des, u16 key_src);

/**@brief ɾ��ĳ�ļ��е�ȫ����¼ 
 * 
 * @param[in] file_id �ļ���
 * @param[in] record_key ��¼��
*/
void HAL_record_del(const u16 file_id, const u16 record_key);

#endif // __HAL_FLASH_H__


