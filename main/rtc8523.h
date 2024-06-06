/**
 * @file rtc8523.h
 * @brief This library is use for Real Time Clock (chip PCF8523).
 *
 * Detailed description TBD.
 *
 * @author Nasrulhadi Abdullah
 * @date 22-04-2024
 */

#ifndef RTC_8523   ///< Header guard
#define RTC_8523

#include "driver/i2c_master.h"

typedef i2c_master_dev_handle_t rtc8523_handle;

/* Control 1 strunct*/
typedef struct{
    bool capacitorSelection;
    bool stop;
    bool softwareReset;
    bool mode_12_24;
    bool secondIntEnabled;
    bool alarmIntEnabled;
    bool correnctionIntEnabled;
}control1_t;

/* hours struct */
typedef struct{
    bool ampm;
    uint8_t h12;
    uint8_t h24;
}hours_t;

typedef enum{
    SUNDAY = 0,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY
}weekdayAssign_t;

typedef enum{
    JANUARY = 0,
    FEBRUARY,
    MARCH,
    APRIL,
    MAY,
    JUNE,
    JULY,
    AUGUST,
    SEPTEMBER,
    OCTOBER,
    NOVEMBER,
    DECEMBER
}mounthAssign_t;

typedef enum{
    RTC_SUCCESS,
    RTC_ERR_COMM,
    RTC_ERR_INVALID_PARAM
}rtc8523Stat_t;

/* Initialize */
rtc8523Stat_t rtc8523_initialize(rtc8523_handle* handle);

/* Control */
rtc8523Stat_t rtc8523_getControl1(rtc8523_handle handle, control1_t* ctl1);
rtc8523Stat_t rtc8523_setControl1(rtc8523_handle handle, control1_t ctl1);

/* Time and date */
rtc8523Stat_t rtc8523_getSeconds(rtc8523_handle handle, uint8_t* seconds);
rtc8523Stat_t rtc8523_setSeconds(rtc8523_handle handle, uint8_t seconds);
rtc8523Stat_t rtc8523_getMinutes(rtc8523_handle handle, uint8_t* minutes);
rtc8523Stat_t rtc8523_setMinutes(rtc8523_handle handle, uint8_t minutes);
rtc8523Stat_t rtc8523_getHours(rtc8523_handle handle, hours_t* hours); //TBV
rtc8523Stat_t rtc8523_setHours(rtc8523_handle handle, hours_t hours, bool mode12h24h);
rtc8523Stat_t rtc8523_getDays(rtc8523_handle handle, uint8_t* days);
rtc8523Stat_t rtc8523_setDays(rtc8523_handle handle, uint8_t days);
rtc8523Stat_t rtc8523_getWeekdays(rtc8523_handle handle, uint8_t* weekdays);
rtc8523Stat_t rtc8523_setWeekdays(rtc8523_handle handle, uint8_t weekdays);
rtc8523Stat_t rtc8523_getMonths(rtc8523_handle handle, uint8_t* months);
rtc8523Stat_t rtc8523_setMonths(rtc8523_handle handle, uint8_t months);
rtc8523Stat_t rtc8523_getYears(rtc8523_handle handle, uint8_t* years);
rtc8523Stat_t rtc8523_setYears(rtc8523_handle handle, uint8_t years);

#endif //RTC_8523