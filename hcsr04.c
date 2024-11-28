#include "../inc/hcsr04.h"
#include "assert.h"

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
            if (echo_pin <= 12)
                return _HC_SR04_PORTI;
        else
            return 255;
        break;
        //ERROR, INCORRECT VALUE
        default:
            return 255;
    }
}


static void exti_on(GPIO_TypeDef* port, uint8_t echo_pin, IRQn_Type echo_irqn)
{
    uint32_t exti_cr_val = port_to_exti(port, echo_pin);
    assert(exti_cr_val != 255);
    uint8_t exti_cr_index = echo_pin / 4;
    SYSCFG->EXTICR[exti_cr_index] |= (exti_cr_val << ((uint32_t)(echo_pin - 4 * exti_cr_index) * 4));
    EXTI->IMR |= (1UL << echo_pin);
    //TODO: add IRQ priority
    NVIC_EnableIRQ(echo_irqn);
}

static void timer_init(TIM_TypeDef* timer, IRQn_Type timer_irqn,
                       uint32_t bus_tim_freq)
{

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




#if _HC_SR04_SENSORS_NUM >= 2

#endif

static void hcsr_04_init(void)
{

}

