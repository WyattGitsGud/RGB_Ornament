
// ESP32 code for RGB LED christmas ornament 
// Created by Wyatt Langenheder
// September 2024

#include "main.h"

// timer + interrupt vars
uint32_t tick_ms = 0;
uint32_t tick_s = 0;
hw_timer_t * timer_instance = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

// State selector button
bool debounce = false;
uint32_t debounce_start_ms = 0;
uint32_t debounce_end_ms = 0;
uint32_t debounce_wait_ms = 20;
uint16_t BUTTON_PIN = GPIO_NUM_22;

// LEDs
uint16_t LED1_R_PIN = GPIO_NUM_27;
uint16_t LED1_G_PIN = GPIO_NUM_26;
uint16_t LED1_B_PIN = GPIO_NUM_25;

uint16_t LED2_R_PIN = GPIO_NUM_2;
uint16_t LED2_G_PIN = GPIO_NUM_4;
uint16_t LED2_B_PIN = GPIO_NUM_16;

uint16_t LED3_R_PIN = GPIO_NUM_17;
uint16_t LED3_G_PIN = GPIO_NUM_5;
uint16_t LED3_B_PIN = GPIO_NUM_18;

uint16_t LED4_R_PIN = GPIO_NUM_19;
uint16_t LED4_G_PIN = GPIO_NUM_21;
uint16_t LED4_B_PIN = GPIO_NUM_3;

// PWM
uint32_t pwm_tick = 0;

// Finite State Machine (FSM)
uint16_t currentState = 1;
uint16_t nextState = 0;


void init_gpio_pins(void){

  // button
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // LEDs
  ledcSetup(CH0, PWM_FREQ, PWM_RES);
  ledcSetup(CH1, PWM_FREQ, PWM_RES);
  ledcSetup(CH2, PWM_FREQ, PWM_RES);
  ledcAttachPin(LED1_R_PIN, CH0);
  ledcAttachPin(LED1_G_PIN, CH1);
  ledcAttachPin(LED1_B_PIN, CH2);

  ledcSetup(CH3, PWM_FREQ, PWM_RES);
  ledcSetup(CH4, PWM_FREQ, PWM_RES);
  ledcSetup(CH5, PWM_FREQ, PWM_RES);
  ledcAttachPin(LED2_R_PIN, CH3);
  ledcAttachPin(LED2_G_PIN, CH4);
  ledcAttachPin(LED2_B_PIN, CH5);

  ledcSetup(CH6, PWM_FREQ, PWM_RES);
  ledcSetup(CH7, PWM_FREQ, PWM_RES);
  ledcSetup(CH8, PWM_FREQ, PWM_RES);
  ledcAttachPin(LED3_R_PIN, CH6);
  ledcAttachPin(LED3_G_PIN, CH7);
  ledcAttachPin(LED3_B_PIN, CH8);

  ledcSetup(CH9, PWM_FREQ, PWM_RES);
  ledcSetup(CH10, PWM_FREQ, PWM_RES);
  ledcSetup(CH11, PWM_FREQ, PWM_RES);
  ledcAttachPin(LED4_R_PIN, CH9);
  ledcAttachPin(LED4_G_PIN, CH10);
  ledcAttachPin(LED4_B_PIN, CH11);
}


void button_event(void){

  if (digitalRead(BUTTON_PIN) == HIGH){
    debounce = false;
  } else {
    if (tick_ms > debounce_end_ms){
      // do below on button press

      nextState = (currentState + 1) % NUM_STATES;

      // end code for button press
      debounce = false;
    }
  }
}


void time_event(void){

  // increment millisecond count
  tick_ms++;

  // increment second count (currently not used)
  if (((tick_ms % 1000)==0) & (tick_ms != 0)){
    tick_s++;
  }

  // handle button press
  // debounce prevents multiple interrupts from a single button press
  if (debounce){
    button_event();
  }

  // Change program state
  if (currentState != nextState){
    currentState = nextState;
    pwm_tick = 0;

    // turn LEDs off
    ledcWrite(CH0, 0);
    ledcWrite(CH1, 0);
    ledcWrite(CH2, 0);

    ledcWrite(CH3, 0);
    ledcWrite(CH4, 0);
    ledcWrite(CH5, 0);

    ledcWrite(CH6, 0);
    ledcWrite(CH7, 0);
    ledcWrite(CH8, 0);

    ledcWrite(CH9, 0);
    ledcWrite(CH10, 0);
    ledcWrite(CH11, 0);
  }

  // Control LED behavior
  updatePWM();
}


void IRAM_ATTR buttonISR(){
  debounce = true;
  debounce_end_ms = tick_ms + debounce_wait_ms;
}


void IRAM_ATTR timerISR() {
  portENTER_CRITICAL_ISR(&timerMux);

  time_event();

  portEXIT_CRITICAL(&timerMux);
}


void setup() {
  Serial.begin(SERIAL_BAUD_RATE);

  init_gpio_pins();

  timer_instance = timerBegin(0, CLK_DIV, true);
  timerAttachInterrupt(timer_instance, &timerISR, true);
  timerAlarmWrite(timer_instance, TIMER_ALARM_VAL, true);
  timerAlarmEnable(timer_instance);

  attachInterrupt(BUTTON_PIN, buttonISR, FALLING);

  Serial.write("setup complete\n");
}


void loop() {
  // Inf loop begin

}