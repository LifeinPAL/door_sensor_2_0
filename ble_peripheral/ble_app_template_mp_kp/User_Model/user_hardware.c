/**@file user_hardware.c
 *
 * @brief 硬件外设的用户层功能
 * 
 * @version 2.0.0.alpha.20230717
 *
 */
 
/**< Private include */
#include "HAL_watchdog.h"
#include "HAL_timer.h"
#include "model_log.h"
#include "user_hardware.h"
#include "board_door_sensor.h"
#include "HAL_saadc.h"
#include "nrf_delay.h"
#include "model_data_trs.h"
#include "HAL_flash.h"

/* global variable */
u32 g_door_event_number = 0;



/* static variable */

/* 看门狗通道号实例 */
nrf_drv_wdt_channel_id m_wdt_channel_id;

/* 注册喂狗软件定时器实例*/
APP_TIMER_DEF(m_app_timer_wdt_feed);

/* 硬件定时器实例 */
const nrf_drv_timer_t TIMER_DOOR = NRF_DRV_TIMER_INSTANCE(1);

/* 开关门防抖计数器 */
static size_t door_timer_count;
/* 开关门状态 */
static u8 door_status_current;
static u8 door_status_old;

/**@brief 测量一次电池电量
 *
 * @retval battery_voltage
 */
i32 battery_voltage_mearment(void);

/* function defination */

/**@brief WDT events handler.
 */
static void wdt_event_handler(void)
{
}

/**@brief 喂狗定时器事件处理
*/
static void app_timer_handler_wdt_feed(void* p_context)
{
	watchdog_feed(m_wdt_channel_id);
//	LOGINFO("wdt feed new.");
}

/*@brief 初始化看门狗功能
*/
void watchdog_module_init(void)
{
	ret_code_t err_code;
	
	hal_app_timer_create(&m_app_timer_wdt_feed, APP_TIMER_MODE_REPEATED, app_timer_handler_wdt_feed);
	hal_app_timer_start(m_app_timer_wdt_feed, 10000, NULL);
	err_code = HAL_watchdog_init(&m_wdt_channel_id, wdt_event_handler, true);
	APP_ERROR_CHECK(err_code);
}

/**@brief LED灯初始化 */
void leds_init(void)
{
#if BOARD_IN_USE == KP_DOOR_SENSOR
	hal_gpio_std_output_init(LED_CENTRAL_CONN);
	hal_gpio_std_output_init(LED_PERIPHERAL_CONN);
	USER_LED_OFF(LED_CENTRAL_CONN);
	USER_LED_OFF(LED_PERIPHERAL_CONN);

#elif BOARD_IN_USE == DK_PCA10100
	hal_gpio_std_output_init(USER_LED1_PIN);
	hal_gpio_std_output_init(USER_LED2_PIN);
	hal_gpio_std_output_init(USER_LED3_PIN);
	hal_gpio_std_output_init(USER_LED4_PIN);
	USER_LED_OFF(USER_LED1_PIN);
	USER_LED_OFF(USER_LED2_PIN);
	USER_LED_OFF(USER_LED3_PIN);
	USER_LED_OFF(USER_LED4_PIN);
#endif
}


/**@brief 开关门引脚变化事件处理函数
*/
static void pin_magnet_event(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
	door_status_current = HAL_GPIO_READ(USER_MAGNET_PIN);
//	LOGINFO("door status current = %d", door_status_current);
	if (!is_timer_start(&TIMER_DOOR))
	{
		timer_start(&TIMER_DOOR);		
	}
	door_timer_count = 0;
}

/**@brief 硬件定时器溢出事件处理函数 */
static void timer_event_handler(nrf_timer_event_t event, void* p_context)
{
	ret_code_t err_code;
//	LOGINFO("timer handler count %d", door_timer_count);
	if (10 == ++door_timer_count)
	{
		timer_stop(&TIMER_DOOR);
		if (door_status_current != door_status_old)
		{
			
			door_data_t door_data = DOOR_DATA_DEFAULT;
			
			door_data.door_state = door_status_old = door_status_current;
			door_data.wkp_src = WKP_SRC_DOOR;
			door_data.battery_voltage = battery_voltage_mearment();
			if (g_lts != LTS_NO_BOND)
			{				
				door_data.global_number = ++g_door_event_number;
				calendar_get(&door_data.calendar);
			}
			LOGINFO("door state: %d,", door_data.door_state);
			LOGINFO("wakeup source: %d,", door_data.wkp_src);
			LOGINFO("battery voltage: %d,", door_data.battery_voltage);
			LOGINFO("event number: %d,", door_data.global_number);
			LOGINFO("current day: %d year, %d month, %d day, %d hour, %d minute, %d second.",door_data.calendar.year, door_data.calendar.month,door_data.calendar.day, door_data.calendar.hour, door_data.calendar.minute, door_data.calendar.second);
			err_code = door_data_queue_push(&door_data);
			APP_ERROR_CHECK(err_code);
			g_sm = SM_DATA_SENDING;
		}
		else
		{
			g_sm = SM_MISSION_COMPLETE;
		}		
	}
}

/*@brief 开关门防抖定时器初始化
*/
static void door_timer_init(void)
{
	ret_code_t err_code;

	HAL_timer_init_t timer_init_param;
	timer_init_param.timer_config.frequency 			= NRF_TIMER_FREQ_1MHz;
	timer_init_param.timer_config.mode					= NRF_TIMER_MODE_TIMER;
	timer_init_param.timer_config.bit_width				= NRF_TIMER_BIT_WIDTH_32;
	timer_init_param.timer_config.interrupt_priority	= NRFX_TIMER_DEFAULT_CONFIG_IRQ_PRIORITY;
	timer_init_param.timer_config.p_context				= NULL;
	timer_init_param.cc_channel							= NRF_TIMER_CC_CHANNEL1;
	timer_init_param.cc_value							= HAL_timer_ms_to_tick(&TIMER_DOOR, 100);
	timer_init_param.short_mask							= NRF_TIMER_SHORT_COMPARE1_CLEAR_MASK;
	timer_init_param.enable_init						= true;

	err_code = timer_init(&TIMER_DOOR, &timer_init_param, timer_event_handler);
	APP_ERROR_CHECK(err_code);
}


/**@brief 磁开关芯片检测模块初始化 */
void door_magnet_init(void)
{
	gpiote_init(USER_MAGNET_PIN, NULL, SENSE_TOGGLE, pin_magnet_event, true);
	door_status_old = door_status_current = HAL_GPIO_READ(USER_MAGNET_PIN);
}

/*@brief 磁开关芯片检测模块反初始化
*/
void door_magnet_uninit(void)
{
	hal_gpiote_uninit(USER_MAGNET_PIN);
}

/*@brief 电池电量监测模块初始化
*/
void battery_monitor_init(void)
{
	ret_code_t err_code;

	HAL_ssadc_init_t saadc_init_param = {
		.SAADC_INPUT = NRF_SAADC_INPUT_AIN3,
		.saadc_channel = 0
	};
	err_code = saadc_init(&saadc_init_param, NULL);
	APP_ERROR_CHECK(err_code);

	hal_gpio_std_output_init(USER_ADC_ENABLE_PIN);
	HAL_GPIO_HIGH(USER_ADC_ENABLE_PIN);
}

/**@brief 测量一次电池电量
 *
 * @retval battery_voltage
 */
i32 battery_voltage_mearment(void)
{
	i32 battery_voltage = 0;
	BM_CE_ENABLE;
	nrf_delay_ms(10);
	battery_voltage = saadc_samples_blocking_mean(SAADC_SAMPLE_TIMES);
	BM_CE_DISABLE;
	return battery_voltage * VOLTAGE_FACTOR_INSAMPLE;
}



/**@brief 硬件资源初始化
*/
void peri_on_board_init(long_term_status_t lts)
{
	leds_init();
	LOGINFO("leds init.");
	
	/* 开关门防抖硬件定时器初始化 */
	door_timer_init();
	
	/* 电池监测单元初始化 */
	battery_monitor_init();
	
	if (LTS_NO_BOND != lts)
	{
		door_magnet_init();
		LOGINFO("door sense init.");
	}
	
	HAL_flash_init();
	LOGINFO("fds init over.");
	
}

