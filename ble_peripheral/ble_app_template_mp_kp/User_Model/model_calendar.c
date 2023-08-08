/**@file model_calendar.c
 *
 * @brief 自定义日历模块，数据定义与运算法则
 * 
 * @version 2.0.0.alpha.20230717
 *
 */
 
/**< Private include */

#include "model_calendar.h"
#include "HAL_timer.h"



/* static variable */
static calendar_data_t m_calendar_data = CALENDAR_DEFAULT;
APP_TIMER_DEF(m_app_timer_calendar);	/* 注册一个日历定时器 */ 

/**@brief 判断是否为闰年
 * 
 * @param[in] year 待查的年份
 * 
 * @retval  true
 *          false 
*/
static bool is_leap_year(cld_year_t year)
{
    return ((0 == year % 400) || ((0 == year % 4) && (0 != year % 100)));
}

/**@brief 根据年和月获取该月的天数
 * 
 * @param[in]   year    待查的年份
 * @param[in]   month   待查的月份
 * 
 * @retval day  返回当月的天数
*/
static cld_day_t get_days_in_month(cld_year_t year, cld_month_t month)
{
    cld_day_t days[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

    if (month < 1 || month > 12)
    {
        /*不在正常月份范围返回错误值0*/
       return 0;
    }
    else
    {
        if ((2 == month) && is_leap_year(year))
        {
            return days[month - 1] + 1;
        }
        else
        {
            return days[month - 1];
        }
        
    }
    
}

/**@brief 检查日历值合法性
 * 
 * @param[in]   p_calendar    待查的日历值
 * 
 * @retval  true
 *          false
*/
bool islegal_calender(const calendar_data_t* p_calendar)
{
    bool check = true;
    if ((p_calendar->hour > 23) ||
        (p_calendar->minute > 59) ||
        (p_calendar->second > 59) ||
        (p_calendar->month > 12) ||
        (p_calendar->month < 1) || 
        (p_calendar->day < 1) ||
        (p_calendar->day > get_days_in_month(p_calendar->year, p_calendar->month)))
    {
        check = false;
    }

    return check;    
}

/**@brief 日历值心跳更新
 * 
 * @param[in,out]   p_calendar    待查的日历值
 * 
 * @retval 
*/
void calendar_beat_update(calendar_data_t* p_calendar)
{
    if (++(p_calendar->second) > 59)
    {
        p_calendar->second = 0;
        if (++(p_calendar->minute) > 59)
        {
            p_calendar->minute = 0;
            if (++(p_calendar->hour) > 23)
            {
                p_calendar->hour = 0;
                if (++(p_calendar->day) > get_days_in_month(p_calendar->year, p_calendar->month))
                {
                    p_calendar->day = 1;
                    if (++(p_calendar->month) > 12)
                    {
                        p_calendar->month = 1;
                        p_calendar->year++;
                    }
                    
                }
                
            }
            
        }
           
    }   
}

/**@brief 天使的心跳*/
void angel_beat(void)
{
    calendar_beat_update(&m_calendar_data);
}


/**@brief 获取日历值
 * 
 * @param[out]   p_calendar    输出的日历值
 * 
 * @retval  NONE
*/
void calendar_get(calendar_data_t* p_calendar)
{
    p_calendar->year = m_calendar_data.year;
    p_calendar->month = m_calendar_data.month;
    p_calendar->day = m_calendar_data.day;
    p_calendar->hour = m_calendar_data.hour;
    p_calendar->minute = m_calendar_data.minute;
    p_calendar->second = m_calendar_data.second;
}

/**@brief 日历值更新
 * 
 * @param[in,out]   p_calendar    待更新的日历值
 * 
 * @retval true or false
*/
bool calendar_update(calendar_data_t* p_calendar)
{
	bool flag_legal;
	if ((flag_legal = islegal_calender(p_calendar)))
	{
		m_calendar_data.year 	= p_calendar->year;
		m_calendar_data.month	= p_calendar->month;
		m_calendar_data.day		= p_calendar->day;
		m_calendar_data.hour	= p_calendar->hour;
		m_calendar_data.minute	= p_calendar->minute;
		m_calendar_data.second	= p_calendar->second;
	}
	
	return flag_legal;
}


/**@brief 日历心跳事件
*/
static void app_timer_handler_calendar(void* p_context)
{
	UNUSED_PARAMETER(p_context);
	angel_beat();
//	LOGINFO("current day: %d year, %d month, %d day, %d hour, %d minute, %d second.",m_calendar_data.year, m_calendar_data.month,m_calendar_data.day, m_calendar_data.hour, m_calendar_data.minute, m_calendar_data.second);
}

/**@brief 日历定时器初始化
*/
void app_timer_init_calendar(void)
{
	hal_app_timer_create(&m_app_timer_calendar, APP_TIMER_MODE_REPEATED, app_timer_handler_calendar);
}

/**@brief 日历定时器初始化
*/
void app_timer_start_calendar(void)
{
	hal_app_timer_start(m_app_timer_calendar, 1000, NULL);
}

/**@brief 日历定时器初始化
*/
void app_timer_stop_calendar(void)
{
	hal_app_timer_stop(m_app_timer_calendar);
}

