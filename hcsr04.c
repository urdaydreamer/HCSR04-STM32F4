#include "../inc/hcsr04.h"
#include "stm32f407xx.h"
#define _EXTI_9_5_IN_STOCK    0
#define _EXTI_15_10_IN_STOCK  0
static void rcc_gpio_on(GPIO_TypeDef* port);
static void gpio_on(GPIO_TypeDef* echo_port, GPIO_TypeDef* trig_port,
                    uint8_t echo_pin, uint8_t trig_pin);
static uint8_t port_to_exti(GPIO_TypeDef* port, uint8_t echo_pin);
static void rcc_timer_init(TIM_TypeDef* timer, uint8_t bus);
static void exti_on(GPIO_TypeDef* port, uint8_t echo_pin, IRQn_Type echo_irqn);
static void timer_init(TIM_TypeDef* timer, IRQn_Type timer_irqn,
                       uint32_t bus_tim_freq, uint8_t bus);
static void rcc_exti_init(GPIO_TypeDef* trig_port, GPIO_TypeDef* echo_port,
                          uint8_t trig_pin, uint8_t echo_pin,
                          IRQn_Type echo_irqn);
static void init_hcsr_04_struct(uint8_t sensor_num, uint8_t echo_pin,
                                GPIO_TypeDef* trig_port, uint8_t trig_pin);
static void trig_all_sensors_on(void);
static void trig_all_sensors_off(void);
static void handle_echo_single_pin(uint8_t sensor_num);
static void handle_echo_pin_9_5(void);
static void handle_echo_pin_15_10(void);
static  hc_sr04_t hc_sr04[_HC_SR04_SENSORS_NUM];
static __IO uint8_t is_triggered = 0;
static void rcc_gpio_on(GPIO_TypeDef* port)
{

    switch((uint32_t)port)
    {
        case (uint32_t)GPIOA:
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
            break;
        case (uint32_t)GPIOB:
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
            break;
        case (uint32_t)GPIOC:
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
            break;
        case (uint32_t)GPIOD:
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
            break;
        case (uint32_t)GPIOE:
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
            break;
        case (uint32_t)GPIOF:
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
            break;
        case (uint32_t)GPIOG:
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
            break;
        case (uint32_t)GPIOH:
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
            break;
        case (uint32_t)GPIOI:
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOIEN;
            break;
    }
}

static void gpio_on(GPIO_TypeDef* echo_port, GPIO_TypeDef* trig_port,
                    uint8_t echo_pin, uint8_t trig_pin)
{
    echo_port->MODER &= ~(0b11UL << (echo_pin * 2)); //Input for ECHO
    trig_port->MODER |= (0b01UL << (trig_pin * 2));  //Output for TRIG
    //Output PUSH_PULL for TRIG
    trig_port->OTYPER &= ~(1UL << trig_pin);
    //Set high speed for ECHO and TRIG
    echo_port->OSPEEDR |= (0b10 << (echo_pin * 2));
    trig_port->OSPEEDR |= (0b10 << (trig_pin * 2));
    //Set PULL-DOWN for ECHO port
    echo_port->PUPDR |= (0b10 << (echo_pin * 2));
}

static uint8_t port_to_exti(GPIO_TypeDef* port, uint8_t echo_pin)
{
    switch((uint32_t)port)
    {
        case (uint32_t)GPIOA:
            return _HC_SR04_PORTA;
            break;
        case (uint32_t)GPIOB:
            return _HC_SR04_PORTB;
            break;
        case (uint32_t)GPIOC:
            return _HC_SR04_PORTC;
            break;
        case (uint32_t)GPIOD:
            return _HC_SR04_PORTD;
            break;
        case (uint32_t)GPIOE:
            return _HC_SR04_PORTE;
            break;
        case (uint32_t)GPIOF:
            return _HC_SR04_PORTF;
            break;
        case (uint32_t)GPIOG:
            return _HC_SR04_PORTG;
            break;
        case (uint32_t)GPIOH:
            return _HC_SR04_PORTH;
            break;
        case (uint32_t)GPIOI:
            //For STM32F4 there is no IRQ for PI[15:12]
            return _HC_SR04_PORTI;
            break;
    }
}




static void rcc_timer_init(TIM_TypeDef* timer, uint8_t bus)
{
    switch(bus)
    {
        case _HC_SR04_APB1:
            switch((uint32_t)timer)
            {
                case (uint32_t)TIM2:
                    RCC->AHB1ENR |= RCC_APB1ENR_TIM2EN;
                    break;
                case (uint32_t)TIM3:
                    RCC->AHB1ENR |= RCC_APB1ENR_TIM3EN;
                    break;
                case (uint32_t)TIM4:
                    RCC->AHB1ENR |= RCC_APB1ENR_TIM4EN;
                    break;
                case (uint32_t)TIM5:
                    RCC->AHB1ENR |= RCC_APB1ENR_TIM5EN;
                    break;
                case (uint32_t)TIM6:
                    RCC->AHB1ENR |= RCC_APB1ENR_TIM6EN;
                    break;
                case (uint32_t)TIM7:
                    RCC->AHB1ENR |= RCC_APB1ENR_TIM7EN;
                    break;
                case (uint32_t)TIM12:
                    RCC->AHB1ENR |= RCC_APB1ENR_TIM12EN;
                    break;
                case (uint32_t)TIM13:
                    RCC->AHB1ENR |= RCC_APB1ENR_TIM13EN;
                    break;
                case (uint32_t)TIM14:
                    RCC->AHB1ENR |= RCC_APB1ENR_TIM14EN;
                    break;
            }
            break;
                case _HC_SR04_APB2:
                    switch((uint32_t)timer)
                    {
                        case (uint32_t)TIM1:
                            RCC->AHB2ENR |= RCC_APB2ENR_TIM1EN;
                            break;
                        case (uint32_t)TIM8:
                            RCC->AHB2ENR |= RCC_APB2ENR_TIM8EN;
                            break;
                        case (uint32_t)TIM9:
                            RCC->AHB2ENR |= RCC_APB2ENR_TIM9EN;
                            break;
                        case (uint32_t)TIM10:
                            RCC->AHB2ENR |= RCC_APB2ENR_TIM10EN;
                            break;
                        case (uint32_t)TIM11:
                            RCC->AHB2ENR |= RCC_APB2ENR_TIM11EN;
                            break;
                    }
                    break;
    }
}


static void exti_on(GPIO_TypeDef* port, uint8_t echo_pin, IRQn_Type echo_irqn)
{
    uint32_t exti_cr_val = port_to_exti(port, echo_pin);
    uint8_t exti_cr_index = echo_pin / 4;
    SYSCFG->EXTICR[exti_cr_index] |= (exti_cr_val << ((uint32_t)(echo_pin - 4 * exti_cr_index) * 4));
    EXTI->RTSR |= (1UL << echo_pin);
    EXTI->FTSR |= (1UL << echo_pin);
    EXTI->IMR |= (1UL << echo_pin);
    //TODO: add IRQ priority
    NVIC_EnableIRQ(echo_irqn);
}

static void timer_init(TIM_TypeDef* timer, IRQn_Type timer_irqn,
                       uint32_t bus_tim_freq, uint8_t bus)
{
    rcc_timer_init(timer, bus);
    uint32_t tim_psc = _HC_SR04_TIM_BUS_FREQ_HZ / 1000000;
    _HC_SR04_TIM->PSC = tim_psc - 1;
    _HC_SR04_TIM->ARR = 0xFFFF;
    NVIC_EnableIRQ(timer_irqn);
    //Overflow IRQ is on
    _HC_SR04_TIM->DIER |= TIM_DIER_UIE;
    //Timer is on
    _HC_SR04_TIM->CR1 = TIM_CR1_CEN;
}

static void rcc_exti_init(GPIO_TypeDef* trig_port, GPIO_TypeDef* echo_port,
                          uint8_t trig_pin, uint8_t echo_pin,
                          IRQn_Type echo_irqn)
{
    rcc_gpio_on(trig_port);
    rcc_gpio_on(echo_port);
    gpio_on(echo_port, trig_port, echo_pin, trig_pin);
    exti_on(echo_port, echo_pin, echo_irqn);
}
/////////////////////////////////////////////////////////////////////////
static void init_hcsr_04_struct(uint8_t sensor_num, uint8_t echo_pin,
                                GPIO_TypeDef* trig_port, uint8_t trig_pin)
{
    hc_sr04[sensor_num - 1].result_time = 0;
    hc_sr04[sensor_num - 1].start_time = 0;
    hc_sr04[sensor_num - 1].end_time = 0;
    hc_sr04[sensor_num - 1].waiting_rising = 1;
    hc_sr04[sensor_num - 1].is_waiting_echo = 0;
    hc_sr04[sensor_num - 1].is_new_time = 0;
    hc_sr04[sensor_num - 1].echo_pin = echo_pin;
    hc_sr04[sensor_num - 1].trigger_port = trig_port;
    hc_sr04[sensor_num - 1].trigger_pin = trig_pin;
}

void hcsr_04_init(void)
{
    timer_init(_HC_SR04_TIM, _HC_SR04_TIM_IRQn, _HC_SR04_TIM_BUS_FREQ_HZ, _HC_SR04_TIM_BUS);
    #if _HC_SR04_SENSORS_NUM >= 1
    init_hcsr_04_struct(1, _HC_SR04_ECHO_PIN_1, _HC_SR04_TRIG_PORT_1, _HC_SR04_TRIG_PIN_1);
    rcc_exti_init(_HC_SR04_TRIG_PORT_1, _HC_SR04_ECHO_PORT_1, _HC_SR04_TRIG_PIN_1,
                  _HC_SR04_ECHO_PIN_1, _HC_SR04_EXTI_IRQn_1);
    #endif
    #if _HC_SR04_SENSORS_NUM >= 2
    init_hcsr_04_struct(2, _HC_SR04_ECHO_PIN_2, _HC_SR04_TRIG_PORT_2, _HC_SR04_TRIG_PIN_2);
    rcc_exti_init(_HC_SR04_TRIG_PORT_2, _HC_SR04_ECHO_PORT_2, _HC_SR04_TRIG_PIN_2,
                  _HC_SR04_ECHO_PIN_2, _HC_SR04_EXTI_IRQn_2);
    #endif
    #if _HC_SR04_SENSORS_NUM >= 3
    init_hcsr_04_struct(3, _HC_SR04_ECHO_PIN_3, _HC_SR04_TRIG_PORT_3, _HC_SR04_TRIG_PIN_3);
    rcc_exti_init(_HC_SR04_TRIG_PORT_3, _HC_SR04_ECHO_PORT_3, _HC_SR04_TRIG_PIN_3,
                  _HC_SR04_ECHO_PIN_3, _HC_SR04_EXTI_IRQn_3);
    #endif

}
///////////////////////////////////////////////////

static void trig_all_sensors_on(void)
{
    for (uint8_t i = 0; i < _HC_SR04_SENSORS_NUM; i++)
    {
        if (hc_sr04[i].is_waiting_echo)
        {
            continue;
        }
        else
        {
            hc_sr04[i].trigger_port->BSRR |= (1UL << hc_sr04[i].trigger_pin);
        }
    }
    _HC_SR04_TIM->CNT = 0xFFFF - 9; // 10us to overflow
}


static void trig_all_sensors_off(void)
{
    for (uint8_t i = 0; i < _HC_SR04_SENSORS_NUM; i++)
    {
        hc_sr04[i].is_waiting_echo = 1;
        hc_sr04[i].trigger_port->BSRR |= (1UL << (16 + hc_sr04[i].trigger_pin));
    }
}


static void handle_echo_single_pin(uint8_t sensor_num)
{
    if (hc_sr04[sensor_num - 1].waiting_rising)
    {
        hc_sr04[sensor_num - 1].start_time = _HC_SR04_TIM->CNT;
        hc_sr04[sensor_num - 1].waiting_rising = 0;
    }
    else if (!hc_sr04[sensor_num - 1].waiting_rising)
    {
        hc_sr04[sensor_num - 1].end_time = _HC_SR04_TIM->CNT;
        hc_sr04[sensor_num - 1].waiting_rising = 1;
        hc_sr04[sensor_num - 1].is_waiting_echo = 0;
        hc_sr04[sensor_num - 1].result_time = hc_sr04[sensor_num - 1].end_time -
        hc_sr04[sensor_num - 1].start_time;
        hc_sr04[sensor_num - 1].is_new_time = 1;
    }
    EXTI->PR &= ~(1UL << hc_sr04[sensor_num - 1].echo_pin);
}


static void handle_echo_pin_9_5(void)
{
    for (uint8_t i = 0; i < _HC_SR04_SENSORS_NUM; i++)
    {
        if (hc_sr04[i].echo_pin <= 9 && hc_sr04[i].echo_pin >= 5)
        {
            if (EXTI->PR & (1UL << hc_sr04[i].echo_pin))
            {
                handle_echo_single_pin(i + 1);
                break;
            }
        }
        else
        {
            continue;
        }
    }
}

static void handle_echo_pin_15_10 (void)
{
    for (uint8_t i = 0; i < _HC_SR04_SENSORS_NUM; i++)
    {
        if (hc_sr04[i].echo_pin <= 15 && hc_sr04[i].echo_pin >= 10)
        {
            if (EXTI->PR & (1UL << hc_sr04[i].echo_pin))
            {
                handle_echo_single_pin(i + 1);
                break;
            }
        }
        else
        {
            continue;
        }
    }
}

//12345678//12345678//12345678//12345678//12345678//12345678//12345678//12345678//12345678//12345678


#if _HC_SR04_SENSORS_NUM >= 1
void _HC_SR04_TIM_IRQHandler(void)
{
    if (is_triggered)
    {
        is_triggered = 0;
        trig_all_sensors_off();
    }
    else
    {
        is_triggered = 1;
        trig_all_sensors_on();
    }
}
#endif



#if _HC_SR04_SENSORS_NUM >= 1 && _HC_SR04_EXTI_1
void _HC_SR04_EXTI_IRQHandler_1(void)
{
    #if _HC_SR04_EXTI_1 == __HC_SR04_EXTI_15_10
    handle_echo_pin_15_10();
    #endif

    #if _HC_SR04_EXTI_1 == __HC_SR04_EXTI_9_5
    handle_echo_pin_9_5();
    #endif

    #if _HC_SR04_EXTI_1 != __HC_SR04_EXTI_9_5 && _HC_SR04_EXTI_1 != __HC_SR04_EXTI_15_10
    handle_echo_single_pin(1);
    #endif
}
#endif



#if _HC_SR04_SENSORS_NUM >= 2 && _HC_SR04_EXTI_2
void _HC_SR04_EXTI_IRQHandler_2(void)
{
    #if _HC_SR04_EXTI_2 == __HC_SR04_EXTI_15_10
    handle_echo_pin_15_10();
    #endif

    #if _HC_SR04_EXTI_2 == __HC_SR04_EXTI_9_5
    handle_echo_pin_9_5();
    #endif

    #if _HC_SR04_EXTI_2 != __HC_SR04_EXTI_9_5 && _HC_SR04_EXTI_2 != __HC_SR04_EXTI_15_10
    handle_echo_single_pin(2);
    #endif
}
#endif


#if _HC_SR04_SENSORS_NUM >= 3 && _HC_SR04_EXTI_3
void _HC_SR04_EXTI_IRQHandler_3(void)
{
    #if _HC_SR04_EXTI_3 == __HC_SR04_EXTI_15_10
    handle_echo_pin_15_10();
    #endif

    #if _HC_SR04_EXTI_3 == __HC_SR04_EXTI_9_5
    handle_echo_pin_9_5();
    #endif

    #if _HC_SR04_EXTI_3 != __HC_SR04_EXTI_9_5 && _HC_SR04_EXTI_3 != __HC_SR04_EXTI_15_10
    handle_echo_single_pin(3);
    #endif
}
#endif

