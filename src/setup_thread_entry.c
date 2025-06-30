/******************************************************************************
 * Program Name: Real Time Alarm Clock
 * Author: Piyush Surani
 * Date Created: January 12, 2025
 * Revision Number: 1.0
 ******************************************************************************/

#include <after_setup_thread.h>  // Include other thread's header file
#include <setup_thread.h>        // Include current thread's header file
#include "global_variable.h"     // Global variables shared between tasks

/* Boolean flags for button states and alarm control */
bool sw4_pressed      = true;  // SW4 button state (default: high)
bool sw5_pressed      = true;  // SW5 button state (default: high)
bool AlarmSet         = false; // Alarm set status (default: off)
bool AlarmTriggered   = false; // Alarm triggered status (default: not triggered)

/* Structures to hold the clock and alarm time */
ClockTime clock_setup_time = {0, 0, 0};  // Real-time clock setup (initialized to 00:00:00)
ClockTime alarm_setup_time = {0, 0, 0};  // Alarm setup (initialized to 00:00:00)

/* Clock state enumeration to manage different display modes */
ClockState currentstate = Normal_Display;  // Initial state: Normal Display
int variable_counter1 = 0;  // Counter for incrementing timer0 (0 to 99) to complete 1 second

/* Main thread entry function */
void setup_thread_entry(void)
{
    /* Initialize timers and external interrupts */
    g_timer0.p_api->open(g_timer0.p_ctrl, g_timer0.p_cfg);  // Timer 0 for real-time clock
    g_timer1.p_api->open(g_timer1.p_ctrl, g_timer1.p_cfg);  // Timer 1 for alarm timeout
    g_external_irq10.p_api->open(g_external_irq10.p_ctrl, g_external_irq10.p_cfg); // SW5 interrupt
    g_external_irq11.p_api->open(g_external_irq11.p_ctrl, g_external_irq11.p_cfg); // SW4 interrupt

    /* Start the real-time clock timer */
    g_timer0.p_api->start(g_timer0.p_ctrl);

    /* Turn off all LEDs (active-low configuration) */
    g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_00, IOPORT_LEVEL_HIGH);  // Green LED
    g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_01, IOPORT_LEVEL_HIGH);  // Yellow LED
    g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_02, IOPORT_LEVEL_HIGH);  // Red LED

    /* Main loop for handling state transitions based on button presses */
    while (1)
    {
        /* Check if SW5 is pressed */
        if (!sw5_pressed)
        {
            if (AlarmTriggered)
            {
                tx_semaphore_put(&alarm_turn_off_semaphore);  // Turn off the alarm
            }
            else
            {
                /* Handle state transitions for SW5 */
                switch (currentstate)
                {
                    case Normal_Display:
                        currentstate = Set_Hour;  // Transition to Set Hour
                        g_timer0.p_api->stop(g_timer0.p_ctrl);  // Stop the timer while setting time
                        break;

                    case Set_Hour:
                        currentstate = Set_Minute;  // Transition to Set Minute
                        break;

                    case Set_Minute:
                        currentstate = Set_Second;  // Transition to Set Second
                        break;

                    case Set_Second:
                        currentstate = Normal_Display;  // Return to Normal Display
                        g_timer0.p_api->start(g_timer0.p_ctrl);  // Restart the timer
                        break;

                    case Alarm_Set_Mode:
                        currentstate = Alarm_Set_Hour;  // Enter Alarm Set Hour mode
                        break;

                    case Alarm_Set_Hour:
                        currentstate = Alarm_Set_Minute;  // Transition to Alarm Set Minute
                        break;

                    case Alarm_Set_Minute:
                        currentstate = Alarm_Set_Second;  // Transition to Alarm Set Second
                        break;

                    case Alarm_Set_Second:
                        currentstate = Normal_Display;  // Exit alarm set mode and set the alarm
                        AlarmSet = true;  // Mark alarm as set
                        break;

                    default:
                        break;
                }
            }
            sw5_pressed = true;  // Reset SW5 flag
        }

        /* Check if SW4 is pressed */
        else if (!sw4_pressed)
        {
            if (AlarmTriggered)
            {
                tx_semaphore_put(&alarm_turn_off_semaphore);  // Turn off the alarm
            }
            else
            {
                /* Handle state transitions for SW4 */
                switch (currentstate)
                {
                    case Normal_Display:
                        currentstate = Alarm_Set_Mode;  // Enter Alarm Set Mode
                        g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_00, IOPORT_LEVEL_HIGH);//off when alarm time change
                        AlarmSet=false; //required when changing alarm set time
                        break;

                    case Set_Hour:
                        clock_setup_time.hour = (clock_setup_time.hour + 1) % 24;  // Increment hour (max 23)
                        break;

                    case Set_Minute:
                        clock_setup_time.minute = (clock_setup_time.minute + 1) % 60;  // Increment minute (max 59)
                        break;

                    case Set_Second:
                        clock_setup_time.second = (clock_setup_time.second + 1) % 60;  // Increment second (max 59)
                        break;

                    case Alarm_Set_Mode:
                         currentstate = Normal_Display;  // Enter at main Display mode
                         break;
                    case Alarm_Set_Hour:
                        alarm_setup_time.hour = (alarm_setup_time.hour + 1) % 24;  // Increment alarm hour (max 23)
                        break;

                    case Alarm_Set_Minute:
                        alarm_setup_time.minute = (alarm_setup_time.minute + 1) % 60;  // Increment alarm minute (max 59)
                        break;

                    case Alarm_Set_Second:
                        alarm_setup_time.second = (alarm_setup_time.second + 1) % 60;  // Increment alarm second (max 59)
                        break;

                    default:
                        break;
                }
            }
            sw4_pressed = true;  // Reset SW4 flag
        }
    }
}

/* Callback for SW5 interrupt */
void sw5_external_irq_cb(external_irq_callback_args_t *p_args)
{
    if (p_args->channel == 10)  // SW5 is on channel 10
    {
        tx_thread_sleep(10);  // Debounce delay
        sw5_pressed = false;  // Mark SW5 as pressed
    }
}

/* Callback for SW4 interrupt */
void sw4_external_irq_cb(external_irq_callback_args_t *p_args)
{
    if (p_args->channel == 11)  // SW4 is on channel 11
    {
        tx_thread_sleep(10);  // Debounce delay
        sw4_pressed = false;  // Mark SW4 as pressed
    }
}

/* Timer callback for clock time increment */
void cb_TimerCh0(timer_callback_args_t *p_args)
{
    (void)p_args;  // Avoid compiler warnings

    variable_counter1++;
    if (variable_counter1 > 99)
    {
        variable_counter1 = 0;  // Reset counter after 100 ticks (1 second)
        clock_setup_time.second++;  // Increment seconds
    }

    if (clock_setup_time.second > 59)
    {
        clock_setup_time.second = 0;
        clock_setup_time.minute++;  // Increment minutes
    }

    if (clock_setup_time.minute > 59)
    {
        clock_setup_time.minute = 0;
        clock_setup_time.hour++;  // Increment hours
    }

    if (clock_setup_time.hour > 23)
    {
        clock_setup_time.hour = 0;  // Reset hours
    }
}
