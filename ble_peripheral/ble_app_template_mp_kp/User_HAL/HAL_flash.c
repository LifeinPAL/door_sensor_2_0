/**@file	HAL_flash.c
 * 
 * @brief	本文件为flash读写操作等功能的硬件抽象层文件
 *			调用fds模块实现
 *
 * @detail	本文件包含application使用fds相关功能的接口函数。
 *			同时作为nrf5 SDK 的上层封装
 */
 
/**< private include. */
#include "HAL_flash.h"
#include "power.h"
#include "user_hardware.h"
#include "model_log.h"

/* private typedef */
typedef struct fds_status_s{
	u8	init 		:1;
	u8	write		:1;
	u8	record_del	:1;
	u8	file_del	:1;
	u8	gc			:1;
	u8	update		:1;
}fds_status_t;

/* static variable */
static const char* fds_event_type[6] = {
	"FDS_EVT_INIT",
    "FDS_EVT_WRITE",
    "FDS_EVT_UPDATE",
    "FDS_EVT_DEL_RECORD",
    "FDS_EVT_DEL_FILE",
    "FDS_EVT_GC"
};

/**< 使用时需要减去 NRF_ERROR_FDS_ERR_BASE */
static const char* fds_error_type[15] = {
	"FDS_ERR_OPERATION_TIMEOUT",
	"FDS_ERR_NOT_INITIALIZED",
	"FDS_ERR_UNALIGNED_ADDR",
	"FDS_ERR_INVALID_ARG",
	"FDS_ERR_NULL_ARG",
	"FDS_ERR_NO_OPEN_RECORDS",
	"FDS_ERR_NO_SPACE_IN_FLASH",
	"FDS_ERR_NO_SPACE_IN_QUEUES",
	"FDS_ERR_RECORD_TOO_LARGE",
	"FDS_ERR_NOT_FOUND",
	"FDS_ERR_NO_PAGES",
	"FDS_ERR_USER_LIMIT_REACHED",
	"FDS_ERR_CRC_CHECK_FAILED",
	"FDS_ERR_BUSY",
	"FDS_ERR_INTERNAL"
};

static fds_status_t m_fds_status = {
	.init 		= false,
	.write		= false,
	.record_del	= false,
	.file_del	= false,
	.gc 		= false,
	.update		= false
};

//static	fds_record_t	m_record = {0};
//static	fds_record_desc_t m_record_desc = {0};
//static	fds_find_token_t m_ftok = {0};
//static	fds_flash_record_t m_fds_record = {0};

static void fds_evt_handler(fds_evt_t const * p_evt)
{
	LOGINFO("fds event id %d: %s", p_evt->id, fds_event_type[p_evt->id]);
	
	switch (p_evt->id)
    {
    	case FDS_EVT_INIT:
		{
			if (p_evt->result == NRF_SUCCESS)
			{
				m_fds_status.init = true;
				LOGINFO("fds init success.");
			}
			else
			{
				LOGINFO("fds error type %d: %s", p_evt->result - NRF_ERROR_FDS_ERR_BASE, fds_error_type[p_evt->result - NRF_ERROR_FDS_ERR_BASE]);
			}
		}break;
    	case FDS_EVT_WRITE:
		{
			if (p_evt->result == NRF_SUCCESS)
			{
				m_fds_status.write = true;
			}
			else
			{
				LOGINFO("fds error type %d: %s", p_evt->result - NRF_ERROR_FDS_ERR_BASE, fds_error_type[p_evt->result - NRF_ERROR_FDS_ERR_BASE]);
			}
    	}break;
		case FDS_EVT_DEL_RECORD:
		{
			if (p_evt->result == NRF_SUCCESS)
			{
				m_fds_status.record_del = true;
			}
			else
			{
				LOGINFO("fds error type %d: %s", p_evt->result - NRF_ERROR_FDS_ERR_BASE, fds_error_type[p_evt->result - NRF_ERROR_FDS_ERR_BASE]);
			}
		}break;
		case FDS_EVT_DEL_FILE:
		{
			if (p_evt->result == NRF_SUCCESS)
			{
				m_fds_status.file_del = true;
			}
			else
			{
				LOGINFO("fds error type %d: %s", p_evt->result - NRF_ERROR_FDS_ERR_BASE, fds_error_type[p_evt->result - NRF_ERROR_FDS_ERR_BASE]);
			}
		}break;
		case FDS_EVT_GC:
		{
			if (p_evt->result == NRF_SUCCESS)
            {
            	m_fds_status.gc = true;
            }
			else
			{
				LOGINFO("fds error type %d: %s", p_evt->result - NRF_ERROR_FDS_ERR_BASE, fds_error_type[p_evt->result - NRF_ERROR_FDS_ERR_BASE]);
			}
		}break;
		case FDS_EVT_UPDATE:
		{
			if (p_evt->result == NRF_SUCCESS)
            {
            	m_fds_status.update = true;
            }
			else
			{
				LOGINFO("fds error type %d: %s", p_evt->result - NRF_ERROR_FDS_ERR_BASE, fds_error_type[p_evt->result - NRF_ERROR_FDS_ERR_BASE]);
			}
		}
    	default:
    		break;
    }
}

/**@brief 初始化flash操作模块
 *
 * @detail
 *
 * @param[in]	
 *
 * @retval
 */
ret_code_t HAL_flash_init(void)
{
	ret_code_t err_code = NRF_SUCCESS;

	/* 注册fds事件处理函数 */
	err_code = fds_register(fds_evt_handler);
	APP_ERROR_CHECK(err_code);

	/* 初始化fds模块 */
	m_fds_status.init = false;
	err_code = fds_init();
	APP_ERROR_CHECK(err_code);

	while (!m_fds_status.init)
	{
		IDLE_ENTER();
	}

	m_fds_status.gc = false;
	err_code = fds_gc();
	
	while (!m_fds_status.gc)
	{
		IDLE_ENTER();
	}
	
	return err_code;
}

/**@brief 获取某文件中的记录数量
 * 
 * @param[in] file_id 文件标识号
 * 
 * @retval count 文件中的记录数量
*/
u8 record_inflie_countget(u16 file_id)
{
	u8 count = 0;
	fds_record_desc_t desc = {0};
	fds_find_token_t ftok = {0};
	while (NRF_SUCCESS == fds_record_find_in_file(file_id, &desc, &ftok))
	{
		count++;
	}

	return count;
}

/**@brief 向某文件中存储一个记录
 * 
 * @param[in] file_id 文件标识号
 * 
 * @retval NRF_STATUS
*/
ret_code_t doordata_record_push(u16 file_id, door_data_t* p_data, u32 size)
{
	ret_code_t err_code;
	fds_record_t	record = {0};
	fds_record_desc_t desc = {0};
	record.file_id = file_id;
	record.key = record_inflie_countget(file_id) + 1;
	record.data.p_data = p_data;
	record.data.length_words = (size + 3) / 4;
	
	m_fds_status.write = false;
	err_code = fds_record_write(&desc, &record);
	if (err_code != NRF_SUCCESS)
	{
		LOGINFO("write %d result: 0x%x, %s.", err_code, fds_error_type[err_code - NRF_ERROR_FDS_ERR_BASE]);
	}
	else
	{	
		while (!m_fds_status.write)
		{
			IDLE_ENTER();
		}
	}
	return err_code;
}



/**@brief 从文件中按记录号读取
 * 
 * @param[in] file_id 读取的文件id号
 * @param[in] record_key 读取的记录号
 * @param[out] p_data 存储读取的数据
 * @param[in] size 读取的数据大小
*/
ret_code_t doordata_record_read(u16 file_id, u16 record_key, door_data_t* p_data, size_t size)
{
	ret_code_t err_code;
	fds_flash_record_t flash_record;
	fds_record_desc_t desc = {0};
	fds_find_token_t ftok = {0};
	
	LOGINFO("file id %d, key src %d.", file_id, record_key);
	err_code = fds_record_find(file_id, record_key, &desc, &ftok);
	if (err_code != NRF_SUCCESS)
	{
		LOGINFO("find result: 0x%x, %s.", err_code, fds_error_type[err_code - NRF_ERROR_FDS_ERR_BASE]);
	}
	err_code = fds_record_open(&desc, &flash_record);
	if (err_code != NRF_SUCCESS)
	{
		LOGINFO("open result: 0x%x, %s.", err_code, fds_error_type[err_code - NRF_ERROR_FDS_ERR_BASE]);
	}	
	memcpy(p_data, flash_record.p_data, size);

	err_code = fds_record_close(&desc);
	if (err_code != NRF_SUCCESS)
	{
		LOGINFO("close result: 0x%x, %s.", err_code, fds_error_type[err_code - NRF_ERROR_FDS_ERR_BASE]);
	}	
	return err_code;
}

/**@brief 文件中移动记录
 * 
 * @param[in] file_id 文件ID号
 * @param[in] key_des 目标记录号
 * @param[in] key_src 源记录号
*/
void doordata_record_move(u16 file_id, u16 key_des, u16 key_src)
{
	ret_code_t err_code;
	fds_record_desc_t record_desc = {0};
	fds_find_token_t f_tok = {0};
	fds_flash_record_t flash_record = {0};
	fds_record_t record = {0};
	
	LOGINFO("file id %d, key src %d, key des %d.", file_id, key_src, key_des);

	/* 获取源记录 */
	err_code = fds_record_find(file_id, key_src, &record_desc, &f_tok);
	if (err_code != NRF_SUCCESS)
	{
		LOGINFO("find result: 0x%x, %s.", err_code, fds_error_type[err_code - NRF_ERROR_FDS_ERR_BASE]);
	}	
	err_code = fds_record_open(&record_desc, &flash_record);
	if (err_code != NRF_SUCCESS)
	{
		LOGINFO("open result: 0x%x, %s.", err_code, fds_error_type[err_code - NRF_ERROR_FDS_ERR_BASE]);
	}

	record.file_id = file_id;
	record.key = key_des;
	record.data.p_data = flash_record.p_data;
	record.data.length_words = (sizeof(door_data_t) + 3) / 4;

	/* 更新新记录 */
	memset(&f_tok, 0, sizeof(fds_find_token_t));
	memset(&record_desc, 0, sizeof(fds_record_desc_t));
	err_code = fds_record_find(file_id, key_des, &record_desc, &f_tok);
	if (err_code != NRF_SUCCESS)
	{
		LOGINFO("find result: 0x%x, %s.", err_code, fds_error_type[err_code - NRF_ERROR_FDS_ERR_BASE]);
	}	

	m_fds_status.update = false;
	err_code = fds_record_update(&record_desc, &record);
	if (err_code != NRF_SUCCESS)
	{
		LOGINFO("update result: 0x%x, %s.", err_code, fds_error_type[err_code - NRF_ERROR_FDS_ERR_BASE]);
	}	
	while (!m_fds_status.update)
	{
		IDLE_ENTER();
	}
	
	/* 删除源记录 */
	memset(&f_tok, 0, sizeof(fds_find_token_t));
	memset(&record_desc, 0, sizeof(fds_record_desc_t));
	err_code = fds_record_find(file_id, key_src, &record_desc, &f_tok);
	if (err_code != NRF_SUCCESS)
	{
		LOGINFO("find result: 0x%x, %s.", err_code, fds_error_type[err_code - NRF_ERROR_FDS_ERR_BASE]);
	}

	m_fds_status.record_del = false;
	err_code = fds_record_delete(&record_desc);
	if (err_code != NRF_SUCCESS)
	{
		LOGINFO("record delete result: 0x%x, %s.", err_code, fds_error_type[err_code - NRF_ERROR_FDS_ERR_BASE]);
	}
	while (!m_fds_status.record_del)
	{
		IDLE_ENTER();
	}
}


/**@brief 删除某文件中的全部记录 
 * 
 * @param[in] file_id 文件号
*/
void HAL_file_del(const u16 file_id)
{
	ret_code_t err_code;

	m_fds_status.file_del = false;
	err_code = fds_file_delete(file_id);
	if (err_code != NRF_SUCCESS)
	{
		LOGINFO("file delete result: 0x%x, %s.", err_code, fds_error_type[err_code - NRF_ERROR_FDS_ERR_BASE]);
	}
	while (!m_fds_status.file_del)
	{
		IDLE_ENTER();
	}	
	m_fds_status.gc = false;
	err_code = fds_gc();
	
	while (!m_fds_status.gc)
	{
		IDLE_ENTER();
	}
}

/**@brief 删除某文件中的某个记录 
 * 
 * @param[in] file_id 文件号
 * @param[in] record_key 记录号
*/
void HAL_record_del(const u16 file_id, const u16 record_key)
{
	ret_code_t err_code;
	fds_record_desc_t record_desc = {0};
	fds_find_token_t f_tok = {0};

	err_code = fds_record_find(file_id, record_key, &record_desc, &f_tok);
	if (err_code != NRF_SUCCESS)
	{
		LOGINFO("find result: 0x%x, %s.", err_code, fds_error_type[err_code - NRF_ERROR_FDS_ERR_BASE]);
	}

	m_fds_status.record_del = false;
	err_code = fds_record_delete(&record_desc);
	if (err_code != NRF_SUCCESS)
	{
		LOGINFO("record delete result: 0x%x, %s.", err_code, fds_error_type[err_code - NRF_ERROR_FDS_ERR_BASE]);
	}
	while (!m_fds_status.record_del)
	{
		IDLE_ENTER();
	}
}









