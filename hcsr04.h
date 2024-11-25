#ifndef _HС_SR04_
#incliude "stm32f407xx.h"
#define _HС_SR04_
#define _HC_SR04_TIM                TIM2
#define _HC_SR04_IRQn               TIM2_IRQn
#define _HC_SR04_IRQHandler         TIM2_IRQHandler

#define _HC_SR04_SENSORS_NUM        1
#define _HC_SR04_ECHO_PIN_1         5
#define _HC_SR04_TRIG_PIN_1         4
#define _HC_SR04_ECHO_PORT_1        GPIOA
#define _HC_SR04_TIRG_PORT_1        GPIOB
#endif
