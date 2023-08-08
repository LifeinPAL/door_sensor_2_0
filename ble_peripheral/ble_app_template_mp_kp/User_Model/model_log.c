/**@file model_log.c
 *
 * @brief 日志模块相应业务逻辑与API封装
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

    /*默认使用串口作为LOG模块后端输出，不添加时间戳*/
    err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_DEFAULT_BACKENDS_INIT();
}


 