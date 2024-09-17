#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include <soc/rtc.h>
#include <soc/gpio_reg.h>
#include <soc/timer_group_reg.h>
#include <soc/interrupts.h>

#define SERIAL_BAUD_RATE    921600
#define TIMER_ALARM_VAL     1000        // Set for 1ms timer interrupts
#define CLK_DIV             80          // Set for 1ms timer interrupts

#define PWM_FREQ            2000        // PWM frequency
#define PWM_RES             8           // bit resolution

#define NUM_STATES          3           // number of FSM states

extern uint16_t currentState;
enum STATES {state0, state1, state2};
enum CHANNELS {CH0, CH1, CH2, CH3, CH4, CH5, CH6, CH7, CH8, CH9, CH10, CH11};
extern uint32_t pwm_tick;               
extern uint32_t tick_ms;

extern void updatePWM(void);            // function in "PWM.cpp" called by main.cpp