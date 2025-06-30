/******************************************************************************
 * Program Name: Real Time Alarm Clock
 * Author: Piyush Surani
 * Date Created: January 12, 2025
 * Revision Number: 1.0
 ******************************************************************************/
#ifndef GLOBAL_VARIABLE_H
#define GLOBAL_VARIABLE_H

// Declare global variables which used in both task
extern bool AlarmSet;
extern bool AlarmTriggered;

// enumeration declare without extern because of no consuming memory here
//State of Real time clock
typedef enum
{
    Normal_Display,
    Set_Hour,
    Set_Minute,
    Set_Second,
    Alarm_Set_Mode,
    Alarm_Set_Hour,
    Alarm_Set_Minute,
    Alarm_Set_Second

}ClockState;

// structure declare without extern because of no consuming memory here
//use for alarm and clock settup
typedef struct {
    int hour;
    int minute;
    int second;
} ClockTime;



#endif // GLOBAL_VARIABLE_H
