/**@file board_door_sensor.h
 *
 * @brief 用于记录开关门传感器硬件信息
 */
 
#ifndef __BOARD_DOOR_SENSOR_H__
#define __BOARD_DOOR_SENSOR_H__


/* private include */
#include "HAL_gpio.h"

/* board defined */
#define DK_PCA10100	0
#define KP_DOOR_SENSOR 1

/* pin mapping information for nRF52833 */
#if BOARD_IN_USE == KP_DOOR_SENSOR
#define	USER_LED1_PIN	HAL_PIN_MAP(0,15)
#define	USER_LED2_PIN	HAL_PIN_MAP(0,17)
#elif BOARD_IN_USE == DK_PCA10100
#define	USER_LED1_PIN	HAL_PIN_MAP(0,13)
#define	USER_LED2_PIN	HAL_PIN_MAP(0,14)
#define	USER_LED3_PIN	HAL_PIN_MAP(0,15)
#define	USER_LED4_PIN	HAL_PIN_MAP(0,16)
#endif //BOARD_IN_USE

#define	USER_MAGNET_PIN	HAL_PIN_MAP(0,20)
#define USER_RESET_PIN	HAL_PIN_MAP(0,18)
#define USER_UART_TX_PIN	HAL_PIN_MAP(1,9)
#define USER_UART_RX_PIN	HAL_PIN_MAP(0,11)
#define USER_ADC_ENABLE_PIN	HAL_PIN_MAP(0,4)
#define USER_ADC_SAMPLE_PIN	HAL_PIN_MAP(0.5)
#define	LED_CENTRAL_CONN		USER_LED1_PIN
#define	LED_PERIPHERAL_CONN		USER_LED2_PIN

/* LED implementation macro define */
#if BOARD_IN_USE == KP_DOOR_SENSOR
#define USER_LED_ON(_led_pin) 	HAL_GPIO_HIGH(_led_pin)
#define USER_LED_OFF(_led_pin) 	HAL_GPIO_LOW(_led_pin)
#elif BOARD_IN_USE == DK_PCA10100
#define USER_LED_ON(_led_pin) 	HAL_GPIO_LOW(_led_pin)
#define USER_LED_OFF(_led_pin) 	HAL_GPIO_HIGH(_led_pin)
#endif

/* battery monitor macro define */
#define VOLTAGE_FACTOR_INSAMPLE	3
#define SAADC_SAMPLE_TIMES 8

#endif // __BOARD_DOOR_SENSOR_H__


