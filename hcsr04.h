#ifndef _HC_SR04_H
#define _HC_SR04_H
#include "stm32f407xx.h"
typedef struct
{
    uint16_t result_time;
    uint16_t start_time;
    uint16_t end_time;
    uint8_t waiting_rising : 1;
    uint8_t is_waiting_echo : 1;
    uint8_t is_new_time : 1;
    uint8_t echo_pin : 5;
    GPIO_TypeDef* trigger_port;
    uint8_t trigger_pin;
} hc_sr04_t;


#define _HC_SR04_PORTA                  0
#define _HC_SR04_PORTB                  1
#define _HC_SR04_PORTC                  2
#define _HC_SR04_PORTD                  3
#define _HC_SR04_PORTE                  4
#define _HC_SR04_PORTF                  5
#define _HC_SR04_PORTG                  6
#define _HC_SR04_PORTH                  7
#define _HC_SR04_PORTI                  8


#define __HC_SR04_DOUBLE_EXTI           0
#define __HC_SR04_EXTI_0                1
#define __HC_SR04_EXTI_1                2
#define __HC_SR04_EXTI_2                3
#define __HC_SR04_EXTI_3                4
#define __HC_SR04_EXTI_4                5
#define __HC_SR04_EXTI_9_5              6
#define __HC_SR04_EXTI_15_10            7



#define _HC_SR04_APB1                   1
#define _HC_SR04_APB2                   2




//TIM defines
#define _HC_SR04_TIM                    TIM2
#define _HC_SR04_TIM_IRQn               TIM2_IRQn
#define _HC_SR04_TIM_IRQHandler         TIM2_IRQHandler
#define _HC_SR04_TIM_BUS                _HC_SR04_APB1
#define _HC_SR04_TIM_BUS_DIV            4UL
#define _HC_SR04_TIM_BUS_FREQ_HZ        10000UL



//WARNING: YOU CANT MAKE THE SAME IRQHandlers FOR TWO OR MORE SENSORS, thanks. If there 2 or more same IRQHandlers, u need  _HC_SR04_EXTI_n (where n is number of sensor)
//                                                          define as __HC_SR04_DOUBLE_EXTI.


//Numbers of sensors define
#define _HC_SR04_SENSORS_NUM            1

//ECHO and TRIG port and pins defines for FIRST SENSOR
#if _HC_SR04_SENSORS_NUM >= 1
#define _HC_SR04_ECHO_PIN_1             11UL
#define _HC_SR04_TRIG_PIN_1             4UL
#define _HC_SR04_ECHO_PORT_1            GPIOA
#define _HC_SR04_TRIG_PORT_1            GPIOB
#define _HC_SR04_EXTI_1                 __HC_SR04_EXTI_15_10
#define _HC_SR04_EXTI_IRQn_1            EXTI15_10_IRQn
#define _HC_SR04_EXTI_IRQHandler_1      EXTI15_10_IRQHandler
#endif

//ECHO and TRIG port and pins defines for SECOND SENSOR
#if _HC_SR04_SENSORS_NUM >= 2
#define _HC_SR04_ECHO_PIN_2             2UL
#define _HC_SR04_TRIG_PIN_2             3UL
#define _HC_SR04_ECHO_PORT_2            GPIOF
#define _HC_SR04_TRIG_PORT_2            GPIOD
#define _HC_SR04_EXTI_2                 __HC_SR04_EXTI_2
#define _HC_SR04_EXTI_IRQn_2            EXTI2_IRQn
#define _HC_SR04_EXTI_IRQHandler_2      EXTI2_IRQHandler
#endif



//ECHO and TRIG port and pins defines for THIRD SENSOR
#if _HC_SR04_SENSORS_NUM >= 3
#define _HC_SR04_ECHO_PIN_3             3
#define _HC_SR04_TRIG_PIN_3             3
#define _HC_SR04_ECHO_PORT_3            GPIOF
#define _HC_SR04_TIRG_PORT_3            GPIOA
#define _HC_SR04_EXTI_3                 __HC_SR04_EXTI_3
#define _HC_SR04_EXTI_IRQn_3            EXTI3_IRQn
#define _HC_SR04_EXTI_IRQHandler_3      EXTI3_IRQHandler
#endif





//NOTE: THIS PART YOU SHOULDNT TOUCH
void hcsr_04_init(void);
void _HC_SR04_TIM_IRQHandler(void);
void _HC_SR04_EXTI_IRQHandler_1(void);
#if _HC_SR04_SENSORS_NUM >= 2 && _HC_SR04_EXTI_1
void _HC_SR04_EXTI_IRQHandler_2(void);
#endif
#if _HC_SR04_SENSORS_NUM >= 3 && _HC_SR04_EXTI_3
void _HC_SR04_EXTI_IRQHandler_3(void);
#endif
#endif

