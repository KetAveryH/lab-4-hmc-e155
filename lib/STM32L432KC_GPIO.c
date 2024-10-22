// STM32L432KC_GPIO.c
// Source code for GPIO functions

#include "STM32L432KC_GPIO.h"

void pinMode(int pin, int function) {
    switch(function) {
        case GPIO_INPUT:
            GPIO->MODER &= ~(0b11 << 2*pin);
            break;
        case GPIO_OUTPUT:
            GPIO->MODER |= (0b1 << 2*pin);
            GPIO->MODER &= ~(0b1 << (2*pin+1));
            break;
        case GPIO_ALT:
            GPIO->MODER &= ~(0b1 << 2*pin);
            GPIO->MODER |= (0b1 << (2*pin+1));
            break;
        case GPIO_ANALOG:
            GPIO->MODER |= (0b11 << 2*pin);
            break;
    }
}

int digitalRead(int pin) {
    return ((GPIO->IDR) >> pin) & 1;
}

void digitalWrite(int pin, int val) {
    GPIO->ODR |= (1 << pin);
}

void togglePin(int pin) {
    // Use XOR to toggle
    GPIO->ODR ^= (1 << pin);
}

void configureGPIOA() {
    // Set GPIO MODER5 to Alternate Functions Mode 10
    //GPIOA->MODER &= ~(0b11 << 10);  // MODE5 / PA5
    //GPIOA->MODER |= (0b10 << 10);
    
    //Set Output Type         
    GPIO->OTYPER &= ~(0b1 << 5); // Output push-pullPA5 

    // Set port Output speed register   ------ Need to double check frequency specification and power supply / load conditions for each speed
    GPIO->OSPEEDR &= ~(0b11 << 10);  // OSPEED5[1:0] or OSPEED[11:10]
    GPIO->OSPEEDR |= (0b01 << 10); // Medium Speed

    // Set alternative function low register
    GPIO->AFRL &= ~(0b1111 << 20);
    GPIO->AFRL |= (0b0001 << 20);   // AF1 TIM2_CH1 on PA5 port  (If confused check Alternate Function Chart on data sheet)

}