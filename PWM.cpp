#include "main.h"


void updatePWM(void){

    // make changes to PWM every 20 ms
    if (tick_ms % 20 == 0){
        pwm_tick++;
        pwm_tick > 511 ? pwm_tick = 0 : pwm_tick++;

        switch(currentState){

            // All LEDs slow on/off red
            case state0:
                if (pwm_tick <= 255){
                    ledcWrite(CH0, pwm_tick);
                    ledcWrite(CH3, pwm_tick);
                    ledcWrite(CH6, pwm_tick);
                    ledcWrite(CH9, pwm_tick);
                } else {
                    ledcWrite(CH0, 511-pwm_tick);
                    ledcWrite(CH3, 511-pwm_tick);
                    ledcWrite(CH6, 511-pwm_tick);
                    ledcWrite(CH9, 511-pwm_tick);
                }
                break;

            // All LEDs slow on/off Green
            case state1:
                if (pwm_tick <= 255){
                    ledcWrite(CH1, pwm_tick);
                    ledcWrite(CH4, pwm_tick);
                    ledcWrite(CH7, pwm_tick);
                    ledcWrite(CH10, pwm_tick);
                } else {
                    ledcWrite(CH1, 511-pwm_tick);
                    ledcWrite(CH4, 511-pwm_tick);
                    ledcWrite(CH7, 511-pwm_tick);
                    ledcWrite(CH10, 511-pwm_tick);
                }
                break;

            // All LEDs slow on/off blue
            case state2:
                if (pwm_tick <= 255){
                    ledcWrite(CH2, pwm_tick);
                    ledcWrite(CH5, pwm_tick);
                    ledcWrite(CH8, pwm_tick);
                    ledcWrite(CH11, pwm_tick);
                } else {
                    ledcWrite(CH2, 511-pwm_tick);
                    ledcWrite(CH5, 511-pwm_tick);
                    ledcWrite(CH8, 511-pwm_tick);
                    ledcWrite(CH11, 511-pwm_tick);
                }
                break;
        }
    }
}