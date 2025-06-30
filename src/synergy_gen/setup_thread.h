/* generated thread header file - do not edit */
#ifndef SETUP_THREAD_H_
#define SETUP_THREAD_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
#ifdef __cplusplus
                extern "C" void setup_thread_entry(void);
                #else
extern void setup_thread_entry(void);
#endif
#include "r_gpt.h"
#include "r_timer_api.h"
#include "r_icu.h"
#include "r_external_irq_api.h"
#ifdef __cplusplus
extern "C" {
#endif
/** Timer on GPT Instance. */
extern const timer_instance_t g_timer0;
#ifndef cb_TimerCh0
void cb_TimerCh0(timer_callback_args_t *p_args);
#endif
/* External IRQ on ICU Instance. */
extern const external_irq_instance_t g_external_irq11;
#ifndef sw4_external_irq_cb
void sw4_external_irq_cb(external_irq_callback_args_t *p_args);
#endif
/* External IRQ on ICU Instance. */
extern const external_irq_instance_t g_external_irq10;
#ifndef sw5_external_irq_cb
void sw5_external_irq_cb(external_irq_callback_args_t *p_args);
#endif
extern TX_SEMAPHORE alarm_turn_off_semaphore;
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* SETUP_THREAD_H_ */
