/**@file model_log.h
 *
 * @brief 日志模块sdk API接口封装
 */
 
#ifndef __MODEL_LOG_H__
#define __MODEL_LOG_H__

/**< private include */
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

/**< export macro */
#define LOGINFO NRF_LOG_INFO
#define LOGDEBUG NRF_LOG_DEBUG
#define LOGWAR NRF_LOG_WARNING
#define LOGERR NRF_LOG_ERROR

/**< export function prototype */
void log_init(void);
#endif // __MODLE_LOG_H__


