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

    // Set Prescaler to (1,000 - 1), to get CLK of 10MHz to equa 10KHz we get l: (10,000 KHz) /(100) = 10KHz
    // This means we have 10,000 clock cycles per second.
    // To do this – TIM6_PSC[15:0] set to 0000001111100111  (which is 999)

    // We want the timer to raise the UIF flag 10 times per millisecond
    // The Prescale value sets CK_INT = f_{CK_PSC} / (PSC[15:0]+1)
    // We set PSC[15:0] = CK_PSC/10,000   
    // This results in CK_INT = CK_PSC * 10,000 / CK_PSC = 10,000 Hz = 10Khz (10 clock cycles per ms)

    TIM6->PSC &= (~(0b1111111111111111));             // Clear bits 15:0
    TIM6->PSC |=    0b0010011100001111;               // Set bits to intended prescaler
                         
    // Set the top of the counter value – TIM6_ARR[15:0] set to 0000000000001010
    // Set the top of the counter value to 10, dividing the signal again, resulting in an overflow every 1ms
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

void initTIM2(uint32_t freq) {
    /* Function to initialize TIM2 in PWM mode, outputting a square wave at
    the given frequency "freq" in hertz. 
    
    If you input a frequency of 0, this function should instead turn off  */
  
    //-----------------------// Timx control register 1 | TIMx_CR1

    // Counter Disable bit 0
    TIM2->CR1 &= 0b0; 
    // DISABLE Capture/Compare 1 output 
    TIM2->CCER &= 0b0; 


    //disable OC1
    
    //// UIFREMAP bit 11  --- bit not present in Debugger?
    //TIM2->CR1 &= ~(0b1 << 11);

    // Clock Division bits 9:8
    TIM2->CR1 &= ~(0b11 << 9);  // No Division 00

    // DISABLE ARPE Auto-reload preload enable bit 7
    TIM2-> CR1 &= ~(0b1 <<7); // Auto-reload preload disabled
    //TIM2->CR1 |= (0b1 <<7);

    // CMS Center-aligned mode selection bits 6:5
    TIM2->CR1 &= ~(0b11 << 7);  // 00: Edge-aligned mode

    // DIR Counter Direction bit 4
    TIM2->CR1 &= ~(0b1 << 4);

    // OPM One Pulse Mode bit 3
    TIM2->CR1 &= ~(0b1 << 3);

    //-----------------------// TIMx control register 2 | TIMx_CR2

    // TI1S Selection bit 7
    TIM2->CR2 &= ~(0b1 << 7); // 0 TIMx_CH1 pin is connected to TI1 input

    // MMS[2:0] Master Mode Selection bits 6:4
    TIM2->CR2 &= ~(0b111 << 4);

    // CCDS Capture/Compare DMA Selection bit 3
    TIM2->CR2 &= ~(0b1 << 3);

    //-----------------------// TIMx slave mode control register | TIMx_SMCR
    

    //-----------------------// TIMx capture/compare mode reigster | TIMx_CCMR1

    // OC1M: Output compare 1 mode bits 6:4
    TIM2->CCMR1 &= ~(0b111 << 4);
    TIM2->CCMR1 |= (0b0110 << 4); //  PWM mode 1 - In upcounting, channel 1 is active as long as TIMx_CNT<TIMx_CCR1
                                  //  else inactive
    
    // OC1PE: Output compare 1 preload enable bit 3
    TIM2->CCMR1 |= (0b1 << 3);  // Preload register enabled

    // CC1S: Capture/Compare 1 selection bits 1:0
    TIM2->CCMR1 &= ~(0b11);

    // ENABLE ARPE Auto-reload preload enable bit 7
    TIM2-> CR1 |= 0b1 << 7; // Auto-reload preload disabled

    //-----------------------//  TIMx prescaler | TIMx_PSC
    
    // PSC[15:0] Prescaler value
    // Set Prescaler to (1,000 - 1), to get CLK of 10MHz to equa 10KHz we get l: (10,000 KHz) /(100) = 10KHz
    // This means we have 10,000 clock cycles per second.
    // To do this – TIM6_PSC[15:0] set to 0000001111100111  (which is 999)
    TIM2->PSC &= (~(0b1111111111111111));           // Clear bits 15:0
    //TIM2->PSC |= 0b0000001111100111;                // Set bits to intended prescaler (999)
    //TIM2->PSC |= 0b0010011100010000;
    //TIM2->PSC |= 0b00000000011000011;                // Set bits to intended prescaler (99)
    TIM2->PSC |= 0b00000000000000000;                // Set bits to intended prescaler (0)


    //-----------------------// TIMx auto-reload register | TIMx_ARR
    // Set Frequency
    // ARR = (SYSCLK/((PSC + 1) * (freq))-1
    TIM2->ARR = (4000000/(freq))-1;

    // set duty
    TIM2->CCR1 = ((4000000/(freq))-1)/2;
    
    //TIM2->ARR = (10000/((0b0000001111100111+0b1)*(freq)))-1;

    //// set duty
    //TIM2->CCR1 = ((10000/((0b0000001111100111+0b1)*(freq)))-1)/2;
    
    //-----------------------// TIMx event generation register | TIMx_EGR
 
    // UG: Update generation bit 0
    TIM2->EGR |= 0b1; // enable UG bit and generate update
    
    

    if (freq >= 0) {
        // ENABLE COUNTER BIT
        TIM2->CR1 |= 0b1;
        // ENABlE Capture/Compare 1 output 
        TIM2->CCER |= 0b1; 
    }

  // ENABLE OC1
  // Enable Counter
}

//// Function for dummy delay by executing nops
void ms_delay(int ms) {
   while (ms-- > 0) {
      volatile int x=1000;
      while (x-- > 0)
         __asm("nop");
   }
}


void play_note(uint32_t freq, uint32_t milliseconds)
{
    //if f(~freq)
    initTIM2(freq);                 // turn on PWM at given frequency "freq"
    //delayMillis(milliseconds);      // leave the note on for time "milliseconds"
    ms_delay(milliseconds);
    initTIM2(0);
    // turn on PWM
    // delay
    //turn off PWM
}