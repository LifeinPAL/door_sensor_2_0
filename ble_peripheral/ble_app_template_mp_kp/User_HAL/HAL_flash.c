/**@file	HAL_flash.c
 * 
 * @brief	���ļ�Ϊflash��д�����ȹ��ܵ�Ӳ��������ļ�
 *			����fdsģ��ʵ��
 *
 * @detail	���ļ�����applicationʹ��fds��ع��ܵĽӿں�����
 *			ͬʱ��Ϊnrf5 SDK ���ϲ��װ
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

/**< ʹ��ʱ��Ҫ��ȥ NRF_ERROR_FDS_ERR_BASE */
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

/**@brief ��ʼ��flash����ģ��
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

	/* ע��fds�¼������� */
	err_code = fds_register(fds_evt_handler);
	APP_ERROR_CHECK(err_code);

	/* ��ʼ��fdsģ�� */
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

/**@brief ��ȡĳ�ļ��еļ�¼����
 * 
 * @param[in] file_id �ļ���ʶ��
 * 
 * @retval count �ļ��еļ�¼����
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

/**@brief ��ĳ�ļ��д洢һ����¼
 * 
 * @param[in] file_id �ļ���ʶ��
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



/**@brief ���ļ��а���¼�Ŷ�ȡ
 * 
 * @param[in] file_id ��ȡ���ļ�id��
 * @param[in] record_key ��ȡ�ļ�¼��
 * @param[out] p_data �洢��ȡ������
 * @param[in] size ��ȡ�����ݴ�С
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

/**@brief �ļ����ƶ���¼
 * 
 * @param[in] file_id �ļ�ID��
 * @param[in] key_des Ŀ���¼��
 * @param[in] key_src Դ��¼��
*/
void doordata_record_move(u16 file_id, u16 key_des, u16 key_src)
{
	ret_code_t err_code;
	fds_record_desc_t record_desc = {0};
	fds_find_token_t f_tok = {0};
	fds_flash_record_t flash_record = {0};
	fds_record_t record = {0};
	
	LOGINFO("file id %d, key src %d, key des %d.", file_id, key_src, key_des);

	/* ��ȡԴ��¼ */
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

	/* �����¼�¼ */
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
	
	/* ɾ��Դ��¼ */
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


/**@brief ɾ��ĳ�ļ��е�ȫ����¼ 
 * 
 * @param[in] file_id �ļ���
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

/**@brief ɾ��ĳ�ļ��е�ĳ����¼ 
 * 
 * @param[in] file_id �ļ���
 * @param[in] record_key ��¼��
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









