// STM32L432KC_RCC.c
// Source code for RCC functions

#include "STM32L432KC_RCC.h"

void configurePLL() {
    // Set clock to 10 MHz
    // Output freq = (src_clk) * (N/M) / R
    // (4 MHz) * (10/1) / 4 = 10 MHz
    // M: 1, N: 10, R: 4
    // Use MSI as PLLSRC

    // ------------------------------- TURN PLL ON AND SET SYS_CLOCK TO 10 MHZ -------------------------------

    // Turn off PLL
    RCC->CR &= ~(0b1 << 24);
    
    // Wait till PLL is unlocked (e.g., off)
    while ((RCC->CR >> 25 & 1) != 0);

    // Load configuration
    // Set PLL SRC to MSI
    RCC->PLLCFGR |= (0b1 << 0);
    RCC->PLLCFGR &= ~(0b1 << 1);

    // Set PLLN
    RCC->PLLCFGR &= ~(0b11111111 << 8); // Clear all bits of PLLN
    RCC->PLLCFGR |= (0b0001010 << 8);   // |= 10
    //RCC->PLLCFGR |= (0b1010000 << 8); // |= 80
     
    // Set PLLM
    RCC->PLLCFGR &= ~(0b111 << 4);  // Clear all bits
    
    // Set PLLR
    RCC->PLLCFGR &= ~(0b1 << 26);
    RCC->PLLCFGR |= (0b1 << 25);
    
    // Enable PLLR output
    RCC->PLLCFGR |= (0b1 << 24);

    // Enable PLL
    RCC->CR |= (0b1 << 24);
    
    // Wait until PLL is locked
    while ((RCC->CR >> 25 & 1) != 1);
}

void configureClock(){
    // Configure and turn on PLL
    configurePLL();

    // Select PLL as clock source
    RCC->CFGR |= (0b11 << 0);
    while(!((RCC->CFGR >> 2) & 0b11));

    //Enable SYS_CLK to TIM6 and TIM2
    RCC->APB1ENR1 |= 0b1; // Bit 0 set to 1, TIM2EN
    RCC->APB1ENR1 |= (0b1 << 4); // Bit 4 set to 1, TIM6EN

    // Set AHB Prescaler
    RCC->CFGR &= (0b0 << 7); // bits 7:4 set to 0xxxx for SYCLK not divided

    // Set APB1 Prescaler
    RCC->CFGR &= (0b0 << 10); // bits 10:8 set to 0xx for HCLK not divided

    // TIMx Multiplier does not need to be configured, APB1 Prescaler is 1, so no multiplication is applied
}