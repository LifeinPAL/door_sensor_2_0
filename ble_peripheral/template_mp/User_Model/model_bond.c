/**@file model_bond.c
 *
 * @brief �����ص�ҵ������
 * 
 * @version 2.0.0.alpha.20230717
 *
 */
 
/**< Private include */
#include "model_bond.h"
#include "model_ble.h"
#include "fds.h"
#include "peer_manager.h"
#include "peer_manager_handler.h"
#include "HAL_timer.h"
#include "model_log.h"
#include "event_id.h"
#include "model_calendar.h"
#include "user_hardware.h"

U_FUN_STATUS fun_bond_status = USER_INVALID;


/**@brief Function for handling Peer Manager events.
 *
 * @param[in] p_evt  Peer Manager event.
 */
static void pm_evt_handler(pm_evt_t const * p_evt)
{
	LOGINFO("PM event id = %d: %s", p_evt->evt_id, pm_evt_id[p_evt->evt_id]);
	/*��ӡpeer_manager��־
	 *����������Ѿ��󶨵�peer������������
	 *�д�����ô�����
	 */
    pm_handler_on_pm_evt(p_evt);
	
	/*���ڶ����ĸ����Ա�׼�����������Ӳ����ǰ�ȫ��
	 *�����յ�PM_EVT_CONN_SEC_FAILED
	 *ͨ������ú�����ÿ��Peer Manager event������
	 *��pm_handler_on_pm_evt()�Ĳ�����Ǵ���
	 */
    pm_handler_disconnect_on_sec_failure(p_evt);
	
	/*
	 *�����ϴ����ӵ�ʱ���peer����
	 *��Ҫʱ����Ͱ�ռ�FLASH�ڴ�
	 *�����ռ�����ʱ������������peer
	 */
    pm_handler_flash_clean(p_evt);

    switch (p_evt->evt_id)
    {
        case PM_EVT_PEERS_DELETE_SUCCEEDED:
		{
			g_lts = LTS_NO_BOND;
			LOGINFO("LTS_NO_BOND : 0x%x.", g_lts);
			app_timer_stop_calendar();
			if ((BLE_CONN_HANDLE_INVALID == g_manager_conn_handle) && (is_gpiote_init()))
			{
				door_magnet_uninit();
				LOGINFO("door sensor disable.");
			}
			fun_bond_status = USER_SUCCESS;
		}break;
		case PM_EVT_PEERS_DELETE_FAILED:
		{
			fun_bond_status = USER_FAILED;
		}break;
		case PM_EVT_CONN_SEC_START:
			LOGINFO("connect secure start.");
			break;
		
		case PM_EVT_CONN_SEC_PARAMS_REQ:
			LOGINFO("connect secure parameters request.");
			break;
		case PM_EVT_CONN_SEC_SUCCEEDED:
		{
			g_lts = LTS_BOND_CONN;
			LOGINFO("LTS_BOND_CONN : 0x%x.", g_lts);
			g_central_conn_handle = p_evt->conn_handle;
			app_timer_start_calendar();
			if (!is_gpiote_init())
			{
				door_magnet_init();
				LOGINFO("door sensor enable.");
			}
		}
		case PM_EVT_BONDED_PEER_CONNECTED:
		{
			g_lts = LTS_BOND_CONN;
			LOGINFO("LTS_BOND_CONN : 0x%x.", g_lts);
			g_central_conn_handle = p_evt->conn_handle;
		}

        default:
            break;
    }
}


/**@brief Function for initialize peer manager module.
*/
void peer_manager_init(void)
{
    ret_code_t err_code;
    ble_gap_sec_params_t sec_param;

    err_code = pm_init();
    APP_ERROR_CHECK(err_code);

    memset(&sec_param, 0, sizeof(ble_gap_sec_params_t));
    /*���ð�ȫ��������İ�ȫ����*/
    sec_param.bond              = SEC_PARAM_BOND;
    sec_param.mitm              = SEC_PARAM_MITM;
    sec_param.lesc              = SEC_PARAM_LESC;
    sec_param.keypress          = SEC_PARAM_KEYPRESS;
    sec_param.io_caps           = SEC_PARAM_IO_CAPABILITIES;
    sec_param.oob               = SEC_PARAM_OOB;
    sec_param.min_key_size      = SEC_PARAM_MIN_KEY_SIZE;
    sec_param.max_key_size      = SEC_PARAM_MAX_KEY_SIZE;
    sec_param.kdist_own.enc     = SEC_PARAM_KDIST_OWN_ENC;  
    sec_param.kdist_own.id      = SEC_PARAM_KDIST_OWN_ID; 
    sec_param.kdist_peer.enc    = SEC_PARAM_KDIST_PEER_ENC;
    sec_param.kdist_peer.id     = SEC_PARAM_KDIST_PEER_ID;

    err_code = pm_sec_params_set(&sec_param);
    APP_ERROR_CHECK(err_code);

    err_code = pm_register(pm_evt_handler);
    APP_ERROR_CHECK(err_code);


}

/**@brief ���ص�ǰ�Ƿ��а󶨵��豸
 * 
 * @retval true or false
*/
bool is_bonded(void)
{
    return (0 == pm_peer_count())? false: true;
}

/**@brief �Ӵ洢��ɾ������Ϣ
 */
void delete_bonds(void)
{
    ret_code_t err_code;

    LOGINFO("Erase bonds!");

    err_code = pm_peers_delete();
    APP_ERROR_CHECK(err_code);
}

/**@brief ��ȡ�����豸��ַ
 * 
 * @param[out]	p_bond_addr ������豸��ַָ��
 * 
 * @retval NRF_STATUS	
*/
ret_code_t local_addr_get(u8* p_local_addr)
{
	ret_code_t err_code;
	ble_gap_addr_t local_addr;

	err_code = pm_id_addr_get(&local_addr);
	APP_ERROR_CHECK(err_code);

	memcpy(p_local_addr, (const u8*)local_addr.addr, 6);
	return err_code;
}

/**@brief ��ȡ�󶨵ĶԶ��豸��ַ
 * 
 * @param[out] p_addr �洢�豸��ַ������ָ��
 * @param[in] conn_handle Ҫ��ѯ�豸�����Ӿ��
*/
ret_code_t bond_addr_get(u8* p_addr, const u16 conn_handle)
{
	ret_code_t err_code;
	pm_peer_id_t peer_id;
	pm_peer_data_bonding_t peer_data;

	if (NRF_SUCCESS == (err_code = pm_peer_id_get(conn_handle, &peer_id)))
	{
//		APP_ERROR_CHECK(err_code);
		LOGINFO("peer id: 0x%x.", peer_id);
		
		err_code = pm_peer_data_bonding_load(peer_id, &peer_data);
//		APP_ERROR_CHECK(err_code);

		memcpy(p_addr, (const u8*)peer_data.peer_ble_id.id_addr_info.addr, 6);
	}

	return err_code;
}




