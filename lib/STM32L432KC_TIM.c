#include "STM32L432KC_TIM.h"
#include <stdint.h>


void initTIM6() {
    /*
    To read out the UIF (update interrupt flag), which turns itself on and stays on whenever
    the timer overflows, you access TIM6_SR[0]. This is 0 if no recent overflow, or 1 if there
    has been an overflow since the last time this bit was cleared.

    To force an update event, resetting the counter and all that good stuff, set TIM6_EGR[0]
    to 1. This is auto-cleared by hardware after it has reset and restarted.
    */

    // Make sure TIM6 is off as we make changes – TIM6_CR1[0] clear to 0
    TIM6->CR1 &= ~0b1;          // Clear the 0th bit

    // Turn off Shadow Registers/Buffer to the counterTop value – TIM6_CR1[7] clear to 0
    TIM6->CR1 &= (~(0b1<<7));   // Clear the 7th bit

    // Disable UIF Update Interrupt, likely not needed it is off by default – TIM6_DIER[0] clear to 0
    TIM6->DIER &= (~0b1);       // Clear the 0th bit

    // Make sure that we can generate update events on counter overflow (UDIS) – TIM6_CR1[1] clear to 0
    TIM6->CR1 &= (~(0b1<<1));   // Set bit 1

    // Set Prescaler to (2,000 - 1), to get 20MHz CLK down to generate a 100µs delay.
    // To do this – TIM6_PSC[15:0] set to 0000011111001111  (which is 1,999)
    TIM6->PSC &= (~(0b1111111111111111));           // Clear bits 15:0
    TIM6->PSC |= 0b0000011111001111;                // Set bits to intended prescaler

    // Set the top of the counter value – TIM6_ARR[15:0] set to 0000000000001010
    // Set the top of the counter value to 10, dividing the signal again, giving a final delay output of 1ms total
    TIM6->ARR &= (~(0b1111111111111111));           // Clear bits 15:0
    TIM6->ARR |= 0b0000000000001010;                // Set bits to intended counter top

    // Make sure Counter is at zero – TIM6_CNT[15:0], clear bits
    TIM6->CNT &= (~(0b1111111111111111));   // Clear bits 15:0

    // Make sure the UIF Flag is down – TIM6_SR[0] clear to 0
    TIM6->SR &= (~0b1);                     // Clear bit 0

    // Turn TIM6 back on – TIM6_CR1[0] set to 1
    TIM6->CR1 |= 0b1;           // Set bit 0 to 1

    // Reinitialize the timer counter and update its registers – TIM6_EGR[0] set to 1
    TIM6->EGR |= 0b1;           // Set bit 0 to 1
}

void delayMillis(uint32_t ms) {
    /*  */
    initTIM6();                         // Initialize Timer 6 to fun in 1ms loops
    TIM6->SR &= (~(0b1));                   // Ensure flag is down
    for(int i = 0; i<ms; i++) {
        while (~(TIM6->SR & 0b1)) {}        // While the flag is down, wait
        TIM6->SR &= (~(0b1));               // After flag is up, reset it down
    }
}

initTIM2(uint32_t freq) {
    /* Function to initialize TIM2 in PWM mode, outputting a square wave at
    the given frequency "freq" in hertz. 
    
    If you input a frequency of 0, this function should instead turn off  */


    if (freq >= 0) {
        TIM2->//CR1, bit CEN |= 1; // SMTH LIKE THIS
    }
}

void playNote(uint32_t freq, uint32_t milliseconds)
{
    //if f(~freq)
    initTIM2(freq);                 // turn on PWM at given frequency "freq"
    delayMillis(milliseconds);      // leave the note on for time "milliseconds"
    initTIM2(0);
    // turn on PWM
    // delay
    //turn off PWM
}