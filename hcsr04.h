#ifndef _HC_SR04_H
#define _HC_SR04_H
#include "stm32f407xx.h"

/*
 * При создании обработчика проверяем, является ли то, что мы создаем - EXTI9_5_IRQHandler. Если является, то делаем #define _EXTI_9_5_IN_STOCK.
 * Ну и при создании каждого последующего обработчика делаем эту проверку.
 * Массив индексов - нахуй. Сделаем просто функцию, которая будет искать номер модуля в зависимости от пина прерывания.
 */

#include <stdint.h>
typedef struct
{
    uint16_t time_before_echo;
    uint16_t time_after_echo;
    uint16_t result_time;
    uint8_t is_waiting_echo : 1;
    uint8_t is_new_time : 1;
    uint8_t ECHO_pin : 6;
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



//TIM defines
#define _HC_SR04_TIM                    TIM2
#define _HC_SR04_TIM_IRQn               TIM2_IRQn
#define _HC_SR04_TIM_IRQHandler         TIM2_IRQHandler
#define _HC_SR04_TIM_BUS_FREQ_HZ        10000



//Numbers of sensors define
#define _HC_SR04_SENSORS_NUM            1

//ECHO and TRIG port and pins defines for FIRST SENSOR
#define _HC_SR04_ECHO_PIN_1             5
#define _HC_SR04_TRIG_PIN_1             4
#define _HC_SR04_ECHO_PORT_1            GPIOA
#define _HC_SR04_TRIG_PORT_1            GPIOB
#define _HC_SR04_EXTI_IRQn_1            EXTI0_IRQn
#define _HC_SR04_EXTI_IRQHandler_1      EXTI0_IRQHandler


//ECHO and TRIG port and pins defines for SECOND SENSOR
#if _HC_SR04_SENSORS_NUM >= 2
#define _HC_SR04_ECHO_PIN_2             2
#define _HC_SR04_TRIG_PIN_2             3
#define _HC_SR04_ECHO_PORT_2            GPIOF
#define _HC_SR04_TIRG_PORT_2            GPIOD
#define _HC_SR04_EXTI_IRQn_2
#define _HC_SR04_EXTI_IRQHandler_2
#endif


//ECHO and TRIG port and pins defines for THIRD SENSOR
#if _HC_SR04_SENSORS_NUM >= 3
#define _HC_SR04_ECHO_PIN_3             2
#define _HC_SR04_TRIG_PIN_3             3
#define _HC_SR04_ECHO_PORT_3            GPIOF
#define _HC_SR04_TIRG_PORT_3            GPIOD
#define _HC_SR04_EXTI_IRQn_3
#define _HC_SR04_EXTI_IRQHandler_3
#endif


/*Так как все добавляется по очереди, то в обработчике прерываний для 1 сенсора будет - sensors[0], для 2го - sensors[1] и так далее.
 * Нужно только разобраться с
 */
#endif

