/**@file model_log.c
 *
 * @brief ��־ģ����Ӧҵ���߼���API��װ
 * 
 * @version 2.0.0.alpha.20230717
 *
*/
 
 /* Private include */
#include "model_log.h"
 
 
/**@breif Function for initializing the nrf log module
*/
void log_init(void)
{
	ret_code_t err_code;

    /*Ĭ��ʹ�ô�����ΪLOGģ��������������ʱ���*/
    err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_DEFAULT_BACKENDS_INIT();
}


 