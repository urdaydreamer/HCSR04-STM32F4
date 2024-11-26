#ifndef _HС_SR04_
//#include "stm32f407xx.h"

#define _HC_SR04_EXTI_PIN_IRQn(PIN) \
(PIN == 0)? EXTI0_IRQn \
(PIN == 1)? EXTI1_IRQn \
(PIN == 2)? EXTI2_IRQn \
(PIN == 3)? EXTI3_IRQn \
(PIN == 4)? EXTI4_IRQn \
(PIN <= 9)? EXTI9_5_IRQn : EXTI15_10_IRQn

#define _HC_SR04_PIN_TO_HANDLER(PIN) \
(PIN == 0)? EXTI0_IRQHandler \
(PIN == 1)? EXTI1_IRQHandler \
(PIN == 2)? EXTI2_IRQHandler \
(PIN == 3)? EXTI3_IRQHandler \
(PIN == 4)? EXTI4_IRQHandler \
(PIN <= 9)? EXTI9_5_IRQHandler : EXTI15_10_IRQHandler


//TIM defines
#define _HС_SR04_
#define _HC_SR04_TIM                    TIM2
#define _HC_SR04_TIM_IRQn               TIM2_IRQn
#define _HC_SR04_TIM_IRQHandler         TIM2_IRQHandler


//Numbers of sensors define
#define _HC_SR04_SENSORS_NUM            2

//ECHO and TRIG port and pins defines for FIRST SENSOR
#define _HC_SR04_ECHO_PIN_1             5
#define _HC_SR04_TRIG_PIN_1             4
#define _HC_SR04_ECHO_PORT_1            GPIOA
#define _HC_SR04_TRIG_PORT_1            GPIOB
#define _HC_SR04_EXTI_IRQn_1            _HC_SR04_EXTI_PIN_IRQn(_HC_SR04_ECHO_PIN_1)
#define _HC_SR04_EXTI_IRQHandler_1      _HC_SR04_PIN_TO_HANDLER(_HC_SR04_ECHO_PIN_1)

//ECHO and TRIG port and pins defines for SECOND SENSOR
#define _HC_SR04_ECHO_PIN_2             2
#define _HC_SR04_TRIG_PIN_2             3
#define _HC_SR04_ECHO_PORT_2            GPIOF
#define _HC_SR04_TIRG_PORT_2            GPIOD
#define _HC_SR04_EXTI_IRQn_2            _HC_SR04_EXTI_PIN_IRQn(_HC_SR04_ECHO_PIN_2)
#define _HC_SR04_EXTI_IRQHandler_2      _HC_SR04_PIN_TO_HANDLER(_HC_SR04_ECHO_PIN_2)

#endif

