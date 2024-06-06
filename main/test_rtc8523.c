/**
 * @file test_rtc8523.c
 * @brief This application is for test the library Real Time Clock (chip PCF8523)
 *
 * Detailed description TBD.
 *
 * @author Nasrulhadi Abdullah
 * @date 22-04-2024
 */

#include <stdio.h>
#include "rtc8523.h"

void app_main(void)
{
    rtc8523_handle rtc8423Handle;
    control1_t ctrl1;
    uint8_t currSeconds = 0;
    int8_t prevSeconds = -1;
    uint8_t minutes = 0;
    hours_t hours;
    char sAMPM[2][3] = {"AM", "PM"};

    rtc8523_initialize(&rtc8423Handle);

    rtc8523_getControl1(rtc8423Handle, &ctrl1);

    printf("CAP_SEL : %d\n", ctrl1.capacitorSelection);
    printf("STOP    : %d\n", ctrl1.stop);
    printf("SR      : %d\n", ctrl1.softwareReset);
    printf("12_24   : %d\n", ctrl1.mode_12_24);
    printf("SIE     : %d\n", ctrl1.secondIntEnabled);
    printf("AIE     : %d\n", ctrl1.alarmIntEnabled);
    printf("CIE     : %d\n\n", ctrl1.correnctionIntEnabled);

    ctrl1.mode_12_24 = 1;
    rtc8523_setControl1(rtc8423Handle, ctrl1);
    rtc8523_getControl1(rtc8423Handle, &ctrl1);

    printf("CAP_SEL : %d\n", ctrl1.capacitorSelection);
    printf("STOP    : %d\n", ctrl1.stop);
    printf("SR      : %d\n", ctrl1.softwareReset);
    printf("12_24   : %d\n", ctrl1.mode_12_24);
    printf("SIE     : %d\n", ctrl1.secondIntEnabled);
    printf("AIE     : %d\n", ctrl1.alarmIntEnabled);
    printf("CIE     : %d\n", ctrl1.correnctionIntEnabled);

    hours.ampm = 0;
    hours.h12  = 11;
    rtc8523_setHours(rtc8423Handle, hours, ctrl1.mode_12_24);
    rtc8523_setMinutes(rtc8423Handle, 58);
    
    while(1)
    {
        rtc8523_getSeconds(rtc8423Handle, &currSeconds);
        rtc8523_getMinutes(rtc8423Handle, &minutes);
        rtc8523_getHours(rtc8423Handle, &hours);

        if(prevSeconds > currSeconds && prevSeconds != -1)
        {
            prevSeconds = -1;
        }

        if(prevSeconds < currSeconds )
        {
            if(ctrl1.mode_12_24 == 0)
            {
                printf("%02d:%02d:%02d\n", hours.h24, minutes, currSeconds);
            }
            else if(ctrl1.mode_12_24 == 1)
            {
                printf("%02d:%02d:%02d %s\n", hours.h12, minutes, currSeconds, sAMPM[hours.ampm]);
            }

            prevSeconds = currSeconds ;
        }
    }
}
