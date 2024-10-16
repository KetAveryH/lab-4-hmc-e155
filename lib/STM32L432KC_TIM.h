// STM32L432KC_TIM.h
// Header for TIM functions

#ifndef STM32L4_TIM_H
#define STM32L4_TIM_h


#include <stdint.h>
#define __IO voltatile;

#define TIM6_BASE (0x40001000UL) // base address of TIM6
#define TIM2_BASE (0x40000000UL) // base address of TIM2


typedef struct
{
  __IO uint32_t CR1;     /*!< TIM6/TIM7 control register 1,                                            Address offset: 0x00 */
  __IO uint32_t CR2;     /*!< TIM6/TIM7 control register 2,                                            Address offset: 0x04 */
  uint32_t RESERVED0;         /*!< Reserved,                                                                Address offset: 0x08 */
  __IO uint32_t DIER;    /*!< TIM6/TIM7 DMA/Interrupt enable register,                                 Address offset: 0x0C */
  __IO uint32_t SR       /*!< TIM6/TIM7 status register,                                               Address offset: 0x10 */
  __IO uint32_t EGR      /*!< TIM6/TIM7 event generation register,                                     Address offset: 0x14 */
  uint32_t RESERVED1;        /*!< Reserved,                                                                Address offset: 0x18 */
  uint32_t RESERVED2;        /*!< Reserved,                                                                Address offset: 0x1C */
  uint32_t RESERVED3;        /*!< Reserved,                                                                Address offset: 0x20 */
  __IO uint32_t CNT;     /*!< TIM6/TIM7 counter,                                                       Address offset: 0x24 */
  __IO uint32_t PSC;     /*!< TIM6/TIM7 prescaler,                                                     Address offset: 0x28 */
  __IO uint32_t ARR;     /*!< TIM6/TIM7 auto-reload register,                                          Address offset: 0x2C */
} TIM6_TypeDef;

#define TIM6 ((TIM6_TypeDef *) TIM6_BASE)

typedef struct
{
  __IO uint32_t CR1;      /*!< TIM2 control register 1,                                        Address offset: 0x00 */
  __IO uint32_t CR2;      /*!< TIM2 control register 2,                                        Address offset: 0x04 */
  __IO uint32_t SMCR;     /*!< TIM2 slave mode control register,                               Address offset: 0x08 */
  __IO uint32_t DIER;     /*!< TIM2 DMA/Interrupt enable register,                             Address offset: 0x0C */
  __IO uint32_t SR;       /*!< TIM2 status register,                                           Address offset: 0x10 */
  __IO uint32_t EGR;      /*!< TIM2 event generation register,                                 Address offset: 0x14 */
  __IO uint32_t CCMR1;    /*!< TIM2 capture/compare mode register 1,                           Address offset: 0x18 */
  __IO uint32_t CCMR2;    /*!< TIM2 capture/compare mode register 2,                           Address offset: 0x1C */
  __IO uint32_t CCER;     /*!< TIM2 capture/compare enable register,                           Address offset: 0x20 */
  __IO uint32_t CNT;      /*!< TIM2 counter,                                                   Address offset: 0x24 */
  __IO uint32_t PSC;      /*!< TIM2 prescaler,                                                 Address offset: 0x28 */
  __IO uint32_t ARR;      /*!< TIM2 auto-reload register,                                      Address offset: 0x2C */
uint32_t RESERVED0;       /*!< Reserved,                                                       Address offset: 0x30 */
  __IO uint32_t CCR1;     /*!< TIM2 capture/compare register 1,                                Address offset: 0x34 */
  __IO uint32_t CCR2;     /*!< TIM2 capture/compare register 2,                                Address offset: 0x38 */
  __IO uint32_t CCR3;     /*!< TIM2 capture/compare register 3,                                Address offset: 0x3C */
  __IO uint32_t CCR4;     /*!< TIM2 capture/compare register 4,                                Address offset: 0x40 */
uint32_t RESERVED1;       /*!< Reserved,                                                       Address offset: 0x44 */
  __IO uint32_t DCR;      /*!< TIM2 DMA control register,                                      Address offset: 0x48 */
  __IO uint32_t DMAR;     /*!< TIM2 DMA address for full transfer,                             Address offset: 0x4C */
  __IO uint32_t OR1;      /*!< TIM2 option register 1,                                         Address offset: 0x50 */
uint32_t RESERVED2;       /*!< Reserved,                                                       Address offsetL 0x54 */
uint32_t RESERVED3;       /*!< Reserved,                                                       Address offsetL 0x58 */
uint32_t RESERVED4;       /*!< Reserved,                                                       Address offsetL 0x5C */
  __IO uint32_t OR2;      /*!< TIM2 option register 2,                                         Address offset: 0x60 */

} TIM2_TypeDef;

#define TIM2 ((TIM2_TypeDef *) TIM2_BASE)



void initTIM2(TIM2_TypeDef * TIM2);
void initTIM6(TIM6_TypeDef * TIM6);
void play_frequency(TIM2_TypeDef * TIM2, uint32_t frequency);
void delay_millis(TIM6_TypeDef * TIM6, uint32_t ms);

void play_note(uint32_t frequency, uint32_t ms);

#endif