# Digital-Alarm-Clock
- Renesas s7g2 board
- e2studio (To programme, debug and display clock on e2studio software)


---

## Multi-Tasking
- TWO different task is created in this project.
- (1) setup_thread:- This task created to setup clock startuo point and alarm setting
- (2) After_Setup_Thread:- This task is created to control of starting and stopping alarm using buttons.


---

## RTOS Used: Threadx
- Semaphore is created to share alarm status variable between two tasks to avoid race condition-locking of task.
