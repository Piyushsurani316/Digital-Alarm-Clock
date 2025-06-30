/* generated thread source file - do not edit */
#include "setup_thread.h"

TX_THREAD setup_thread;
void setup_thread_create(void);
static void setup_thread_func(ULONG thread_input);
static uint8_t setup_thread_stack[1024] BSP_PLACE_IN_SECTION_V2(".stack.setup_thread") BSP_ALIGN_VARIABLE_V2(BSP_STACK_ALIGNMENT);
void tx_startup_err_callback(void *p_instance, void *p_data);
void tx_startup_common_init(void);
#if (3) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_timer0) && !defined(SSP_SUPPRESS_ISR_GPT0)
SSP_VECTOR_DEFINE_CHAN(gpt_counter_overflow_isr, GPT, COUNTER_OVERFLOW, 0);
#endif
#endif
static gpt_instance_ctrl_t g_timer0_ctrl;
static const timer_on_gpt_cfg_t g_timer0_extend =
{ .gtioca =
{ .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
  .gtiocb =
  { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
  .shortest_pwm_signal = GPT_SHORTEST_LEVEL_OFF, };
static const timer_cfg_t g_timer0_cfg =
{ .mode = TIMER_MODE_PERIODIC,
  .period = 10,
  .unit = TIMER_UNIT_PERIOD_MSEC,
  .duty_cycle = 50,
  .duty_cycle_unit = TIMER_PWM_UNIT_RAW_COUNTS,
  .channel = 0,
  .autostart = true,
  .p_callback = cb_TimerCh0,
  .p_context = &g_timer0,
  .p_extend = &g_timer0_extend,
  .irq_ipl = (3), };
/* Instance structure to use this module. */
const timer_instance_t g_timer0 =
{ .p_ctrl = &g_timer0_ctrl, .p_cfg = &g_timer0_cfg, .p_api = &g_timer_on_gpt };
#if (4) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_external_irq11) && !defined(SSP_SUPPRESS_ISR_ICU11)
SSP_VECTOR_DEFINE( icu_irq_isr, ICU, IRQ11);
#endif
#endif
static icu_instance_ctrl_t g_external_irq11_ctrl;
static const external_irq_cfg_t g_external_irq11_cfg =
{ .channel = 11,
  .trigger = EXTERNAL_IRQ_TRIG_FALLING,
  .filter_enable = true,
  .pclk_div = EXTERNAL_IRQ_PCLK_DIV_BY_64,
  .autostart = true,
  .p_callback = sw4_external_irq_cb,
  .p_context = &g_external_irq11,
  .p_extend = NULL,
  .irq_ipl = (4), };
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq11 =
{ .p_ctrl = &g_external_irq11_ctrl, .p_cfg = &g_external_irq11_cfg, .p_api = &g_external_irq_on_icu };
#if (4) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_external_irq10) && !defined(SSP_SUPPRESS_ISR_ICU10)
SSP_VECTOR_DEFINE( icu_irq_isr, ICU, IRQ10);
#endif
#endif
static icu_instance_ctrl_t g_external_irq10_ctrl;
static const external_irq_cfg_t g_external_irq10_cfg =
{ .channel = 10,
  .trigger = EXTERNAL_IRQ_TRIG_FALLING,
  .filter_enable = true,
  .pclk_div = EXTERNAL_IRQ_PCLK_DIV_BY_64,
  .autostart = true,
  .p_callback = sw5_external_irq_cb,
  .p_context = &g_external_irq10,
  .p_extend = NULL,
  .irq_ipl = (4), };
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq10 =
{ .p_ctrl = &g_external_irq10_ctrl, .p_cfg = &g_external_irq10_cfg, .p_api = &g_external_irq_on_icu };
TX_SEMAPHORE alarm_turn_off_semaphore;
extern bool g_ssp_common_initialized;
extern uint32_t g_ssp_common_thread_count;
extern TX_SEMAPHORE g_ssp_common_initialized_semaphore;

void setup_thread_create(void)
{
    /* Increment count so we will know the number of ISDE created threads. */
    g_ssp_common_thread_count++;

    /* Initialize each kernel object. */
    UINT err_alarm_turn_off_semaphore;
    err_alarm_turn_off_semaphore = tx_semaphore_create (&alarm_turn_off_semaphore, (CHAR*) "Alarm Turn Off Semaphore",
                                                        0);
    if (TX_SUCCESS != err_alarm_turn_off_semaphore)
    {
        tx_startup_err_callback (&alarm_turn_off_semaphore, 0);
    }

    UINT err;
    err = tx_thread_create (&setup_thread, (CHAR*) "Clock and Alarm Setup Thread", setup_thread_func, (ULONG) NULL,
                            &setup_thread_stack, 1024, 3, 3, 1, TX_AUTO_START);
    if (TX_SUCCESS != err)
    {
        tx_startup_err_callback (&setup_thread, 0);
    }
}

static void setup_thread_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize common components */
    tx_startup_common_init ();

    /* Initialize each module instance. */

    /* Enter user code for this thread. */
    setup_thread_entry ();
}
