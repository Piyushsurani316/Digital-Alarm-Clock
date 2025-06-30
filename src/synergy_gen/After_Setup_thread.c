/* generated thread source file - do not edit */
#include "after_setup_thread.h"

TX_THREAD after_setup_thread;
void after_setup_thread_create(void);
static void after_setup_thread_func(ULONG thread_input);
static uint8_t after_setup_thread_stack[1024] BSP_PLACE_IN_SECTION_V2(".stack.after_setup_thread") BSP_ALIGN_VARIABLE_V2(BSP_STACK_ALIGNMENT);
void tx_startup_err_callback(void *p_instance, void *p_data);
void tx_startup_common_init(void);
#if (3) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_timer1) && !defined(SSP_SUPPRESS_ISR_GPT1)
SSP_VECTOR_DEFINE_CHAN(gpt_counter_overflow_isr, GPT, COUNTER_OVERFLOW, 1);
#endif
#endif
static gpt_instance_ctrl_t g_timer1_ctrl;
static const timer_on_gpt_cfg_t g_timer1_extend =
{ .gtioca =
{ .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
  .gtiocb =
  { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
  .shortest_pwm_signal = GPT_SHORTEST_LEVEL_OFF, };
static const timer_cfg_t g_timer1_cfg =
{ .mode = TIMER_MODE_PERIODIC,
  .period = 10,
  .unit = TIMER_UNIT_PERIOD_MSEC,
  .duty_cycle = 50,
  .duty_cycle_unit = TIMER_PWM_UNIT_RAW_COUNTS,
  .channel = 1,
  .autostart = false,
  .p_callback = cb_TimerCh1,
  .p_context = &g_timer1,
  .p_extend = &g_timer1_extend,
  .irq_ipl = (3), };
/* Instance structure to use this module. */
const timer_instance_t g_timer1 =
{ .p_ctrl = &g_timer1_ctrl, .p_cfg = &g_timer1_cfg, .p_api = &g_timer_on_gpt };
extern bool g_ssp_common_initialized;
extern uint32_t g_ssp_common_thread_count;
extern TX_SEMAPHORE g_ssp_common_initialized_semaphore;

void after_setup_thread_create(void)
{
    /* Increment count so we will know the number of ISDE created threads. */
    g_ssp_common_thread_count++;

    /* Initialize each kernel object. */

    UINT err;
    err = tx_thread_create (&after_setup_thread, (CHAR*) "After Setup Thread", after_setup_thread_func, (ULONG) NULL,
                            &after_setup_thread_stack, 1024, 3, 3, 1, TX_AUTO_START);
    if (TX_SUCCESS != err)
    {
        tx_startup_err_callback (&after_setup_thread, 0);
    }
}

static void after_setup_thread_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize common components */
    tx_startup_common_init ();

    /* Initialize each module instance. */

    /* Enter user code for this thread. */
    after_setup_thread_entry ();
}
