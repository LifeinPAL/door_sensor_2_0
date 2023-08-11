/**@file model_calendar.h
 *
 * @brief �Զ�������ģ�����ݶ�����API�ӿ�
 */
 
#ifndef __MODEL_CALENDAR_H__
#define __MODEL_CALENDAR_H__

/**< private include */
#include "common_abbr.h"
#include "model_log.h"

/* export typedef */
typedef u16 cld_year_t;
typedef u8  cld_month_t;
typedef u8  cld_day_t;
typedef u8  cld_hour_t;
typedef u8  cld_minute_t;
typedef u8  cld_second_t;

typedef struct calendar_s
{
    cld_year_t year;
    cld_month_t month;
    cld_day_t day;
    cld_hour_t hour;
    cld_minute_t minute;
    cld_second_t second;
}calendar_data_t;


/* export macro */

#define CALENDAR_DEFAULT 	\
{					\
	.year = 1999,	\
	.month = 12,		\
	.day = 31,		\
	.hour = 23,		\
	.minute = 59,	\
	.second = 30		\
}


/* export function prototype */

/**@brief ��ȡ����ֵ
 * 
 * @param[out]   p_calender    ���������ֵ
 * 
 * @retval  NONE
*/
void calendar_get(calendar_data_t* p_calendar);

/**@brief ����ֵ����
 * 
 * @param[in,out]   p_calendar    �����µ�����ֵ
 * 
 * @retval true or false
*/
bool calendar_update(calendar_data_t* p_calendar);

/**@brief ������ʱ����ʼ��
*/
void app_timer_init_calendar(void);

/**@brief ��ʹ������*/
void angel_beat(void);

/**@brief ������ʱ����ʼ��
*/
void app_timer_start_calendar(void);

/**@brief ������ʱ����ʼ��
*/
void app_timer_stop_calendar(void);

#endif // __MODEL_CALENDAR_H__

