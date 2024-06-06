/**
 * @file rtc8523.c
 * @brief This library is use for Real Time Clock (chip PCF8523)
 *
 * Detailed description TBD.
 *
 * @author Nasrulhadi Abdullah
 * @date 22-04-2024
 */

#include "rtc8523.h"

/* RTC PCF8523 7-bits I2C address*/
#define RTC8523_ADDRESS         0x68 //TBD - hard code or configurable

/* Register Address */
#define REG_CONTROL_1           0x00
#define REG_CONTROL_2           0x01
#define REG_CONTROL_3           0x02
#define REG_SECONDS             0x03
#define REG_MINUTES             0x04
#define REG_HOURS               0x05
#define REG_DAYS                0x06
#define REG_WEEKDAYS            0x07
#define REG_MONTHS              0x08
#define REG_YEARS               0x09
#define REG_MINUTES_ALARM       0x0A
#define REG_HOUR_ALARM          0x0B
#define REG_DAY_ALAM            0x0C
#define REG_WEEKDAY_ALAM        0x0D
#define REG_OFFSET              0x0E
#define REG_TMR_CLKOUT_CTRL     0x0F
#define REG_TMR_A_FREQ_CTRL     0x10
#define REG_A_REG               0x11
#define REG_TMR_B_FREQ_CTRL     0x12
#define REG_B_REG               0x13

/* Bitmasks */
#define SECONDS_UPPER_DIGIT     0x70
#define SECONDS_DIGIT           0x0F 
#define MINUTES_UPPER_DIGIT     0x70
#define MINUTES_DIGIT           0x0F 
#define HOURS_AMPM              0x20
#define HOURS_12_UPPER_DIGIT    0x10
#define HOURS_24_UPPER_DIGIT    0x30
#define HOURS_DIGIT             0x0F
#define DAYS_UPPER_DIGIT        0x30
#define DAYS_DIGIT              0x0F
#define WEEKDAYS_DIGIT          0x07
#define MONTHS_UPPER_DIGIT      0x10
#define MONTHS_DIGIT            0x0F
#define YEARS_UPPER_DIGIT       0xF0
#define YEARS_DIGIT             0x0F

/* Bit-shifts */
#define BITSHIFT_HOURS_AMPM             0x05
#define BITSHIFT_HOURS_12_UPPER_DIGIT   0x04
#define BITSHIFT_HOURS_24_UPPER_DIGIT   0x04
#define BITSHIFT_MINUTES_UPPER_DIGIT    0x04
#define BITSHIFT_SECONDS_UPPER_DIGIT    0x04
#define BITSHIFT_DAYS_UPPER_DIGIT       0x04
#define BITSHIFT_MONTHS_UPPER_DIGIT     0x04
#define BITSHIFT_YEARS_UPPER_DIGIT      0x04

rtc8523Stat_t rtc8523_initialize(rtc8523_handle* handle)
{
    i2c_master_bus_config_t i2c_mst_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = 0, //not sure yet
        .scl_io_num = GPIO_NUM_22,
        .sda_io_num = GPIO_NUM_21,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    i2c_master_bus_handle_t bus_handle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle)); //TBV

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = RTC8523_ADDRESS,
        .scl_speed_hz = 100000,
    };

    //i2c_master_dev_handle_t dev_handle;
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, handle)); //TBV

    return RTC_SUCCESS;
}

rtc8523Stat_t rtc8523_getControl1(rtc8523_handle handle, control1_t* ctl1)
{
    esp_err_t espStat;
    uint8_t writeBuffer = REG_CONTROL_1;
    uint8_t readBuffer  = 0;

    espStat = i2c_master_transmit_receive(handle, &writeBuffer, 1, &readBuffer, 1, 100);
    if(ESP_ERR_INVALID_ARG == espStat)
    {
        return RTC_ERR_INVALID_PARAM;
    }
    else if(ESP_OK != espStat)
    {
        return RTC_ERR_COMM;
    }

    /* Decode control 1 */
    ctl1->capacitorSelection    = readBuffer >> 0x07;
    ctl1->stop                  = (readBuffer >> 0x05) & 0x01;
    ctl1->softwareReset         = (readBuffer >> 0x04) & 0x01;
    ctl1->mode_12_24            = (readBuffer >> 0x03) & 0x01;
    ctl1->secondIntEnabled      = (readBuffer >> 0x02) & 0x01;
    ctl1->alarmIntEnabled       = (readBuffer >> 0x01) & 0x01;
    ctl1->correnctionIntEnabled = readBuffer & 0x01;

    return RTC_SUCCESS;
}

rtc8523Stat_t rtc8523_setControl1(rtc8523_handle handle, control1_t ctl1)
{
    esp_err_t espStat;
    uint8_t writeBuffer[2] = {0};
    writeBuffer[0] = REG_CONTROL_1;

    /* encode */
    writeBuffer[1] = ctl1.capacitorSelection << 0x07;
    writeBuffer[1] |= ctl1.stop << 0x05;
    writeBuffer[1] |= ctl1.softwareReset << 0x04;
    writeBuffer[1] |= ctl1.mode_12_24 << 0x03;
    writeBuffer[1] |= ctl1.secondIntEnabled << 0x02;
    writeBuffer[1] |= ctl1.alarmIntEnabled << 0x01;
    writeBuffer[1] |= ctl1.correnctionIntEnabled;

    espStat = i2c_master_transmit(handle, writeBuffer, 2, 100);
    if(ESP_ERR_INVALID_ARG == espStat)
    {
        return RTC_ERR_INVALID_PARAM;
    }
    else if(ESP_OK != espStat)
    {
        return RTC_ERR_COMM;
    }

    return RTC_SUCCESS;
}

rtc8523Stat_t rtc8523_getSeconds(rtc8523_handle handle, uint8_t* seconds)
{
    esp_err_t espStat;
    uint8_t writeBuffer = REG_SECONDS;
    uint8_t readBuffer  = 0;

    espStat = i2c_master_transmit_receive(handle, &writeBuffer, 1, &readBuffer, 1, 100);
    if(ESP_ERR_INVALID_ARG == espStat)
    {
        return RTC_ERR_INVALID_PARAM;
    }
    else if(ESP_OK != espStat)
    {
        return RTC_ERR_COMM;
    }

    /* decode seconds */
    *seconds = ((readBuffer & SECONDS_UPPER_DIGIT) >> 4) * 10;
    *seconds += ((readBuffer & SECONDS_DIGIT));

    return RTC_SUCCESS;
}

rtc8523Stat_t rtc8523_setSeconds(rtc8523_handle handle, uint8_t seconds)
{
    esp_err_t espStat;
    uint8_t writeBuffer[2] = {0};

    /* Verify the parameter */
    if(seconds > 59)  ///< not need to check "seconds < 0" because "uint8_t"
    {
        return RTC_ERR_INVALID_PARAM;
    }

    /* register address */
    writeBuffer[0] = REG_SECONDS;

    /* encode seconds */
    writeBuffer[1] = (seconds / 10) << BITSHIFT_SECONDS_UPPER_DIGIT;
    writeBuffer[1] |= seconds % 10;

    espStat = i2c_master_transmit(handle, writeBuffer, 2, 100); //TBV for time out (ms)
    if(ESP_ERR_INVALID_ARG == espStat)
    {
        return RTC_ERR_INVALID_PARAM;
    }
    else if(ESP_OK != espStat)
    {
        return RTC_ERR_COMM;
    }

    return RTC_SUCCESS;
}

rtc8523Stat_t rtc8523_getMinutes(rtc8523_handle handle, uint8_t* minutes)
{
    esp_err_t espStat;
    uint8_t writeBuffer = REG_MINUTES;
    uint8_t readBuffer  = 0;

    espStat = i2c_master_transmit_receive(handle, &writeBuffer, 1, &readBuffer, 1, 100); //TBV
    if(ESP_ERR_INVALID_ARG == espStat)
    {
        return RTC_ERR_INVALID_PARAM;
    }
    else if(ESP_OK != espStat)
    {
        return RTC_ERR_COMM;
    }

    /* decode minutes */
    *minutes = ((readBuffer & MINUTES_UPPER_DIGIT) >> 4) * 10;
    *minutes += ((readBuffer & MINUTES_DIGIT));

    return RTC_SUCCESS;
}

rtc8523Stat_t rtc8523_setMinutes(rtc8523_handle handle, uint8_t minutes)
{
    esp_err_t espStat;
    uint8_t writeBuffer[2] = {0};

    /* Verify the parameter */
    if(minutes > 59)    ///< not need to check "minutes < 0" because "uint8_t"
    {
        return RTC_ERR_INVALID_PARAM;
    }

    /* register address */
    writeBuffer[0] = REG_MINUTES;

    /* encode minutes */
    writeBuffer[1] = (minutes / 10) << BITSHIFT_MINUTES_UPPER_DIGIT;
    writeBuffer[1] |= minutes % 10;

    espStat = i2c_master_transmit(handle, writeBuffer, 2, 100); //TBV for time timeout
    if(ESP_ERR_INVALID_ARG == espStat)
    {
        return RTC_ERR_INVALID_PARAM;
    }
    else if(ESP_OK != espStat)
    {
        return RTC_ERR_COMM;
    }

    return RTC_SUCCESS;
}

rtc8523Stat_t rtc8523_getHours(rtc8523_handle handle, hours_t* hours)
{
    esp_err_t espStat;
    uint8_t writeBuffer = REG_HOURS;
    uint8_t readBuffer  = 0;

    espStat = i2c_master_transmit_receive(handle, &writeBuffer, 1, &readBuffer, 1, 100); //TBV for time timeout
    if(ESP_ERR_INVALID_ARG == espStat)
    {
        return RTC_ERR_INVALID_PARAM;
    }
    else if(ESP_OK != espStat)
    {
        return RTC_ERR_COMM;
    }

    /* decode AM PM flag*/
    hours->ampm = (readBuffer & HOURS_AMPM) >> BITSHIFT_HOURS_AMPM;

    /* decode 12-hours format*/
    hours->h12 = ((readBuffer & HOURS_12_UPPER_DIGIT) >> BITSHIFT_HOURS_12_UPPER_DIGIT) * 10;
    hours->h12 += ((readBuffer & HOURS_DIGIT));

    /* decode 24-hours format*/
    hours->h24 = ((readBuffer & HOURS_24_UPPER_DIGIT) >> BITSHIFT_HOURS_24_UPPER_DIGIT) * 10;
    hours->h24 += ((readBuffer & HOURS_DIGIT));

    return RTC_SUCCESS;
}

rtc8523Stat_t rtc8523_setHours(rtc8523_handle handle, hours_t hours, bool mode12h24h)
{
    esp_err_t espStat;
    uint8_t writeBuffer[2] = {0};

    /* register address */
    writeBuffer[0] = REG_HOURS;

    if(mode12h24h == 0)
    {
        /* verify the parameter */
        if(hours.h24 > 23)     ///< not need to check "hours.h24 < 0" because "uint8_t"
        {
            return RTC_ERR_INVALID_PARAM;
        }

        /* encode 24-hours format */
        writeBuffer[1] = (hours.h24 / 10) << BITSHIFT_HOURS_24_UPPER_DIGIT;
        writeBuffer[1] |= hours.h24 % 10; 
    }
    else if(mode12h24h == 1)
    {
        /* verify the parameter */
        if(hours.h12 > 12) ///< not need to check "hours.h12 < 0" because "uint8_t"
        {
            return RTC_ERR_INVALID_PARAM;
        }

        /* encode 12-hours format */
        writeBuffer[1] = hours.ampm << 5;
        writeBuffer[1] |= (hours.h12 / 10) << BITSHIFT_HOURS_12_UPPER_DIGIT;
        writeBuffer[1] |= hours.h12 % 10; 
    }

    espStat = i2c_master_transmit(handle, writeBuffer, 2, 100); //TBV
    if(ESP_ERR_INVALID_ARG == espStat)
    {
        return RTC_ERR_INVALID_PARAM;
    }
    else if(ESP_OK != espStat)
    {
        return RTC_ERR_COMM;
    }

    return RTC_SUCCESS;
}

rtc8523Stat_t rtc8523_getDays(rtc8523_handle handle, uint8_t* days)
{
    esp_err_t espStat;
    uint8_t writeBuffer = REG_DAYS;
    uint8_t readBuffer  = 0;

    espStat = i2c_master_transmit_receive(handle, &writeBuffer, 1, &readBuffer, 1, 100); // TBV
    if(ESP_ERR_INVALID_ARG == espStat)
    {
        return RTC_ERR_INVALID_PARAM;
    }
    else if(ESP_OK != espStat)
    {
        return RTC_ERR_COMM;
    }

    /* decode days */
    *days = ((readBuffer & DAYS_UPPER_DIGIT) >> 4) * 10;
    *days += ((readBuffer & DAYS_DIGIT));

    return RTC_SUCCESS;
}

rtc8523Stat_t rtc8523_setDays(rtc8523_handle handle, uint8_t days)
{
    esp_err_t espStat;
    uint8_t writeBuffer[2] = {0};

    /* verify the parameter */
    if(days < 1 || days > 31)
    {
        return RTC_ERR_INVALID_PARAM;
    }

    /* register address */
    writeBuffer[0] = REG_DAYS;

    /* encode days */
    writeBuffer[1] = (days / 10) << BITSHIFT_DAYS_UPPER_DIGIT;
    writeBuffer[1] |= days % 10;

    espStat = i2c_master_transmit(handle, writeBuffer, 2, 100); // TBV
    if(ESP_ERR_INVALID_ARG == espStat)
    {
        return RTC_ERR_INVALID_PARAM;
    }
    else if(ESP_OK != espStat)
    {
        return RTC_ERR_COMM;
    }

    return RTC_SUCCESS;
}

rtc8523Stat_t rtc8523_getWeekdays(rtc8523_handle handle, uint8_t* weekdays)
{
    esp_err_t espStat;
    uint8_t writeBuffer = REG_WEEKDAYS;
    uint8_t readBuffer  = 0;

    espStat = i2c_master_transmit_receive(handle, &writeBuffer, 1, &readBuffer, 1, 100); //TBV
    if(ESP_ERR_INVALID_ARG == espStat)
    {
        return RTC_ERR_INVALID_PARAM;
    }
    else if(ESP_OK != espStat)
    {
        return RTC_ERR_COMM;
    }

    /* decode weekdays */
    *weekdays = ((readBuffer & WEEKDAYS_DIGIT));

    return RTC_SUCCESS;
}

rtc8523Stat_t rtc8523_setWeekdays(rtc8523_handle handle, uint8_t weekdays)
{
    esp_err_t espStat;
    uint8_t writeBuffer[2] = {0};

    /* verify the parameter */
    if(weekdays > 6)    ///< not need to check "weekdays < 0" because "uint8_t"
    {
        return RTC_ERR_INVALID_PARAM;
    }

    /* register address */
    writeBuffer[0] = REG_WEEKDAYS;
    writeBuffer[1] = weekdays; //TBV

    espStat = i2c_master_transmit(handle, writeBuffer, 2, 100); //TBV
    if(ESP_ERR_INVALID_ARG == espStat)
    {
        return RTC_ERR_INVALID_PARAM;
    }
    else if(ESP_OK != espStat)
    {
        return RTC_ERR_COMM;
    }

    return RTC_SUCCESS;
}

rtc8523Stat_t rtc8523_getMonths(rtc8523_handle handle, uint8_t* months)
{
    esp_err_t espStat;
    uint8_t writeBuffer = REG_MONTHS;
    uint8_t readBuffer  = 0;

    espStat = i2c_master_transmit_receive(handle, &writeBuffer, 1, &readBuffer, 1, 100); //TBV
    if(ESP_ERR_INVALID_ARG == espStat)
    {
        return RTC_ERR_INVALID_PARAM;
    }
    else if(ESP_OK != espStat)
    {
        return RTC_ERR_COMM;
    }

    /* decode months */
    *months = ((readBuffer & MONTHS_UPPER_DIGIT) >> 4) * 10;
    *months += ((readBuffer & MONTHS_DIGIT));

    return RTC_SUCCESS;
}

rtc8523Stat_t rtc8523_setMonths(rtc8523_handle handle, uint8_t months)
{
    esp_err_t espStat;
    uint8_t writeBuffer[2] = {0};

    /* verify the parameter */
    if(months < 1 || months > 12)
    {
        return RTC_ERR_INVALID_PARAM;
    }

    /* register address */
    writeBuffer[0] = REG_MONTHS;

    /* encode months */
    writeBuffer[1] = (months / 10) << BITSHIFT_MONTHS_UPPER_DIGIT;
    writeBuffer[1] |= months % 10;

    espStat = i2c_master_transmit(handle, writeBuffer, 2, 100); //TBV
    if(ESP_ERR_INVALID_ARG == espStat)
    {
        return RTC_ERR_INVALID_PARAM;
    }
    else if(ESP_OK != espStat)
    {
        return RTC_ERR_COMM;
    }

    return RTC_SUCCESS;
}

rtc8523Stat_t rtc8523_getYears(rtc8523_handle handle, uint8_t* years)
{
    esp_err_t espStat;
    uint8_t writeBuffer = REG_YEARS;
    uint8_t readBuffer  = 0;

    espStat = i2c_master_transmit_receive(handle, &writeBuffer, 1, &readBuffer, 1, 100); //TBV
    if(ESP_ERR_INVALID_ARG == espStat)
    {
        return RTC_ERR_INVALID_PARAM;
    }
    else if(ESP_OK != espStat)
    {
        return RTC_ERR_COMM;
    }

    /* decode years */
    *years = ((readBuffer & YEARS_UPPER_DIGIT) >> 4) * 10;
    *years += ((readBuffer & YEARS_DIGIT));

    return RTC_SUCCESS;
}

rtc8523Stat_t rtc8523_setYears(rtc8523_handle handle, uint8_t years)
{
    esp_err_t espStat;
    uint8_t writeBuffer[2] = {0};

    /* verify the parameter */
    if(years < 1 || years > 99)
    {
        return RTC_ERR_INVALID_PARAM;
    }

    /* register address */
    writeBuffer[0] = REG_YEARS;

    /* encode years */
    writeBuffer[1] = (years / 10) << BITSHIFT_YEARS_UPPER_DIGIT;
    writeBuffer[1] |= years % 10;

    espStat = i2c_master_transmit(handle, writeBuffer, 2, 100); //TBV
    if(ESP_ERR_INVALID_ARG == espStat)
    {
        return RTC_ERR_INVALID_PARAM;
    }
    else if(ESP_OK != espStat)
    {
        return RTC_ERR_COMM;
    }

    return RTC_SUCCESS;
}