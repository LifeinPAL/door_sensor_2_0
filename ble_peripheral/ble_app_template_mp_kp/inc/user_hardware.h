/**@file user_hardware.h
 *
 * @brief 
 */
 
#ifndef __USER_HARDWARE_H__
#define __USER_HARDWARE_H__

/* private include */
#include "main.h"
#include "HAL_gpio.h"

/* export macro */
#define BM_CE_ENABLE 	HAL_GPIO_LOW(USER_ADC_ENABLE_PIN)
#define BM_CE_DISABLE 	HAL_GPIO_HIGH(USER_ADC_ENABLE_PIN)

/* export variable */
extern u32 g_door_event_number;

/* export function prototype */

/*@brief ��ʼ�����Ź�����
*/
void watchdog_module_init(void);

/**@brief LED�Ƴ�ʼ�� */
void leds_init(void);


/*@brief �ſ���оƬ���ģ���ʼ��
*/
void door_magnet_init(void);

/*@brief �ſ���оƬ���ģ�鷴��ʼ��
*/
void door_magnet_uninit(void);


/**@brief Ӳ����Դ��ʼ��
*/
void peri_on_board_init(long_term_status_t lts);

/**@brief ����һ�ε�ص���
 *
 * @retval battery_voltage
 */
i32 battery_voltage_mearment(void);


#endif // __USER_HARDWARE_H__

