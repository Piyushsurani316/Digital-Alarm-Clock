/******************************************************************************
 * Program Name: Real Time Alarm Clock
 * Author: Piyush Surani
 * Date Created: January 12, 2025
 * Revision Number: 1.0
 ******************************************************************************/
#include <after_setup_thread.h>
#include <setup_thread.h>
#include "global_variable.h"

// External global variables
extern bool AlarmSet;         // Flag to indicate if the alarm is set
extern bool AlarmTriggered;   // Flag to indicate if the alarm is triggered
extern ClockTime clock_setup_time;  // Real-time clock (using ClockTime structure)
extern ClockTime alarm_setup_time;  // Alarm time (using ClockTime structure)

// Local variables
bool led1_toggle = true;      // Toggle state for yellow LED
bool led2_toggle = false;     // Toggle state for red LED
int AlarmTimeoutCounter = 0;  // Counter for alarm timeout
uint8_t variable_Counter2 = 0; // Counter for incrementing timer1 (0 to 99) to complete 1 second

// Thread entry function for handling alarm
void after_setup_thread_entry(void)
{
    while (1)
    {
        // Check if the alarm is set
        if (AlarmSet)
        {
            // Turn ON green LED to indicate the alarm is active
            g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_00, IOPORT_LEVEL_LOW);

            // Check if the current time matches the alarm time
            if (clock_setup_time.hour == alarm_setup_time.hour &&
                clock_setup_time.minute == alarm_setup_time.minute &&
                clock_setup_time.second == alarm_setup_time.second)
            {
                // If time matches, trigger the alarm
                AlarmTriggered = true;
                g_timer1.p_api->start(g_timer1.p_ctrl); // Start timer to track alarm timeout

                // Alarm-handling loop
                while (AlarmTriggered)
                {
                    // Toggle LEDs to create an alternating alarm effect
                    g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_01, led1_toggle ? IOPORT_LEVEL_LOW : IOPORT_LEVEL_HIGH); // Yellow LED
                    g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_02, led2_toggle ? IOPORT_LEVEL_LOW : IOPORT_LEVEL_HIGH); // Red LED

                    // Update toggle states for alternating behavior
                    led1_toggle = !led1_toggle;
                    led2_toggle = !led2_toggle;

                    // Check for alarm turn-off condition
                    if (tx_semaphore_get(&alarm_turn_off_semaphore, TX_NO_WAIT) == TX_SUCCESS || AlarmTimeoutCounter > 59)
                    {
                        // Stop the alarm if semaphore is received or timeout occurs
                        AlarmTimeoutCounter = 0; // Reset timeout counter
                        g_timer1.p_api->stop(g_timer1.p_ctrl); // Stop the timer
                        AlarmTriggered = false;  // Reset alarm triggered flag
                        AlarmSet = false;        // Reset alarm set flag

                        // Turn OFF all LEDs
                        g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_00, IOPORT_LEVEL_HIGH); // Turn off green LED
                        g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_01, IOPORT_LEVEL_HIGH); // Turn off yellow LED
                        g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_02, IOPORT_LEVEL_HIGH); // Turn off red LED

                        // Reset alarm time to default
                        alarm_setup_time.hour = 0;
                        alarm_setup_time.minute = 0;
                        alarm_setup_time.second = 0;
                   }

                    // Sleep for a short period to create LED toggling effect and avoid high CPU usage
                    tx_thread_sleep(100); // Adjust sleep time based on system tick frequency
                }
            }
        }

        // Sleep to avoid excessive CPU usage when the alarm is not active
        tx_thread_sleep(100);
    }
}

// Timer callback function for tracking alarm timeout
void cb_TimerCh1(timer_callback_args_t *p_args)
{
    (void)p_args; // Mark p_args as unused to avoid compiler warnings

    variable_Counter2++;
    if (variable_Counter2 > 99)
    {
        variable_Counter2 = 0;

        // Increment the timeout counter each time the timer interrupt occurs
        AlarmTimeoutCounter++;
    }
}
