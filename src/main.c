// main.c
// GPIO blink LED with clock configuration
// Josh Brake
// jbrake@hmc.edu
// 9/16/24

// Includes for libraries
#include "STM32L432KC_RCC.h"
#include "STM32L432KC_GPIO.h"
#include "STM32L432KC_FLASH.h"
#include "STM32L432KC_TIM.h"

// Pitch in Hz, duration in ms
const int notes[][2] = {
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{  0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{  0,	125},
{330,	125},
{416,	125},
{494,	125},
{523,	250},
{  0,	125},
{330,	125},
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{  0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{  0,	125},
{330,	125},
{523,	125},
{494,	125},
{440,	250},
{  0,	125},
{494,	125},
{523,	125},
{587,	125},
{659,	375},
{392,	125},
{699,	125},
{659,	125},
{587,	375},
{349,	125},
{659,	125},
{587,	125},
{523,	375},
{330,	125},
{587,	125},
{523,	125},
{494,	250},
{  0,	125},
{330,	125},
{659,	125},
{  0,	250},
{659,	125},
{1319,	125},
{  0,	250},
{623,	125},
{659,	125},
{  0,	250},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{  0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{  0,	125},
{330,	125},
{416,	125},
{494,	125},
{523,	250},
{  0,	125},
{330,	125},
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{  0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{  0,	125},
{330,	125},
{523,	125},
{494,	125},
{440,	500},
{  0,	0}};

const int notes1[][2] = {
    // First Phrase
    {659, 125},  // E5
    {0,   50},   // Gap
    {659, 125},  // E5
    {0,   50},   // Gap
    {659, 250},  // E5 (Longer)
    {0,   50},   // Gap
    {659, 125},  // E5
    {0,   50},   // Gap
    {659, 125},  // E5
    {0,   50},   // Gap
    {659, 250},  // E5 (Longer)
    {0,   50},   // Gap
    {659, 125},  // E5
    {0,   50},   // Gap
    {784, 125},  // G5
    {0,   50},   // Gap
    {523, 125},  // C5
    {0,   50},   // Gap
    {587, 125},  // D5
    {0,   50},   // Gap
    {659, 500},  // E5 (Longer)
    {0,   100},  // Gap

    // Second Phrase
    {698, 125},  // F5
    {0,   50},   // Gap
    {698, 125},  // F5
    {0,   50},   // Gap
    {698, 125},  // F5
    {0,   50},   // Gap
    {698, 125},  // F5
    {0,   50},   // Gap
    {659, 125},  // E5
    {0,   50},   // Gap
    {659, 125},  // E5
    {0,   50},   // Gap
    {659, 125},  // E5
    {0,   50},   // Gap
    {659, 125},  // E5
    {0,   50},   // Gap
    {587, 125},  // D5
    {0,   50},   // Gap
    {587, 125},  // D5
    {0,   50},   // Gap
    {659, 125},  // E5
    {0,   50},   // Gap
    {587, 250},  // D5 (Longer)
    {0,   50},   // Gap
    {784, 250},  // G5 (Longer)
    {0,   100},  // Gap

    // Repeat First Phrase
    {659, 125},  // E5
    {0,   50},   // Gap
    {659, 125},  // E5
    {0,   50},   // Gap
    {659, 250},  // E5 (Longer)
    {0,   50},   // Gap
    {659, 125},  // E5
    {0,   50},   // Gap
    {659, 125},  // E5
    {0,   50},   // Gap
    {659, 250},  // E5 (Longer)
    {0,   50},   // Gap
    {659, 125},  // E5
    {0,   50},   // Gap
    {784, 125},  // G5
    {0,   50},   // Gap
    {523, 125},  // C5
    {0,   50},   // Gap
    {587, 125},  // D5
    {0,   50},   // Gap
    {659, 500},  // E5 (Longer)
    {0,   100},  // Gap

    // Second Phrase Variation
    {698, 125},  // F5
    {0,   50},   // Gap
    {698, 125},  // F5
    {0,   50},   // Gap
    {698, 125},  // F5
    {0,   50},   // Gap
    {698, 125},  // F5
    {0,   50},   // Gap
    {659, 125},  // E5
    {0,   50},   // Gap
    {659, 125},  // E5
    {0,   50},   // Gap
    {659, 125},  // E5
    {0,   50},   // Gap
    {784, 125},  // G5
    {0,   50},   // Gap
    {784, 125},  // G5
    {0,   50},   // Gap
    {698, 125},  // F5
    {0,   50},   // Gap
    {587, 125},  // D5
    {0,   50},   // Gap
    {523, 750},  // C5 (Longer)
    {0,   100},  // Gap

    // End of song
    {0, 0}       // End marker
};



// Define macros for constants
#define LED_PIN           3
#define DELAY_DURATION_MS    200

#define PWM_PIN 5

//// Function for dummy delay by executing nops
//void ms_delay(int ms) {
//   while (ms-- > 0) {
//      volatile int x=1000;
//      while (x-- > 0)
//         __asm("nop");
//   }
//}



int main(void) {
    // Configure flash to add waitstates to avoid timing errors
    configureFlash();

    // Setup the PLL and switch clock source to the PLL
    configureClock();
    //configurePLL();
    // Turn on clock to GPIOA PORTA
    RCC->AHB2ENR |= (1 << 0);

    pinMode(PWM_PIN, GPIO_ALT);
    configureGPIOA();
    

    

    // Turn on clock to GPIOB
    RCC->AHB2ENR |= (1 << 1);

    


    // Set LED_PIN as output
    pinMode(LED_PIN, GPIO_OUTPUT);

    // Blink LED

    for (int i=0; notes[i][0] != 0 || notes[i][1] != 0; i++) {
        uint32_t freq = notes[i][0];   // Get frequency
        uint32_t delay = notes[i][1];  // Get duration in milliseconds
        play_note(freq, delay);        // Play the note (assuming `play_note` is implemented)
        //ms_delay(delay);               // Delay for the duration of the note
    }

    //while(1) {
    //    //initTIM2(500);
    //    play_note(500, 1000);
    //    ms_delay(DELAY_DURATION_MS);
    //    //initTIM2(0);
    //    ms_delay(DELAY_DURATION_MS);
    //    togglePin(LED_PIN);
    //}
    return 0;
}