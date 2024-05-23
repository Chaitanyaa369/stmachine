/******************************************************************************
statemachine.c
CSE/EE 5385/7385 Microprocessor Architecture and Interfacing
ARM MCBSTM32C Final Project
******************************************************************************/

#include "stdio.h"
#include <stm32f10x_cl.h>
#include "GLCD.h"
#include <math.h>
#include <stdlib.h>

#define LED_NUM     8                   /* Number of user LEDs                */
const long led_mask[] = { 1<<8, 1<<9, 1<<10, 1<<11, 1<<12, 1<<13, 1<<14, 1<<15 };
int timer_counter=0;
int state =0;
//uint32_t delay_value=0;

void EXTI0_IRQHandler();
void EXTI9_5_IRQHandler();
void EXTI15_10_IRQHandler();
void TIM3_IRQHandler (void);

int main (void) {
  
  SystemInit();

  RCC->APB2ENR |= 1<<6;             /* Enable GPIOE clock */
  RCC->APB2ENR |= 1<<0;                /* Enable AFIO clock */
  RCC->APB2ENR |= 1<<13;               /* Enable GPIOC clock */

  GPIOE->CRH    = 0x33333333;          /* Configure the GPIOE pins 8-15 as push-pull outputs at 50MHz */

  AFIO ->EXTICR[0] = 0x0000;           /* Map EXTI0 line to PA0 */
  AFIO ->EXTICR[2] = 0x1000;           /* Map EXTI7 line to PC7 */
  AFIO ->EXTICR[3] = 0x0020;           /* Map EXTI13 line to PE13 */

  EXTI->RTSR |= 1<<0;                  /* Set Rising trigger (Rising edge) for EXTI line 0 */
  EXTI->RTSR |= 1<<7;                  /* Set Rising trigger (Rising edge) for EXTI line 7 */
  EXTI->RTSR |= 1<<13;                 /* Set Rising trigger (Rising edge) for EXTI line 13 */

  EXTI->IMR |= 1<<0;                   /* Enable EXTI line 0 */
  EXTI->IMR |= 1<<7;                   /* Enable EXTI line 7 */
  EXTI->IMR |= 1<<13;                  /* Enable EXTI line 13 */

  NVIC_EnableIRQ(EXTI0_IRQn);          /* Enable NVIC interrupt for EXTI line 0 */
  NVIC_EnableIRQ(EXTI9_5_IRQn);        /* Enable NVIC interrupt for EXTI lines 5 to 9 */
  NVIC_EnableIRQ(EXTI15_10_IRQn);      /* Enable NVIC interrupt for EXTI lines 10 to 15 */

  RCC->APB1ENR |= (1<<1);              /* Enable clock for TIM3 */

  TIM3->PSC = 50;                      /* Set prescaler for TIM3 */
  TIM3->ARR = (18000000UL / 1000UL) - 1; /* Set auto-reload value for TIM3 */
  TIM3->CR1 = 0;                       /* Reset command register 1 of TIM3 */
  TIM3->CR2 = 0;                       /* Reset command register 2 of TIM3 */
  TIM3->DIER = (1<<0);                 /* Enable update interrupt for TIM3 */
  TIM3->CR1 |= (1<<0);                 /* Enable TIM3 */

  NVIC_EnableIRQ(TIM3_IRQn);           /* Enable NVIC interrupt for TIM3 */

  GLCD_Init();                          /* Initialize graphical LCD display   */
  GLCD_Clear(White);                    /* Clear graphical LCD display        */
    /* Switch statements */
    // case 0: Display the text on the screen and set counter to 0.
    // case 1: Display the text on the screen and when counter becomes greater than 80, reset the counter and define the next state.
    // case 2: Display the text on the screen and when counter becomes greater than 150, reset the counter and define the next state.
    // case 3: Display the text on the screen and set counter to 0.
    // case 4: Display the text on the screen and when counter becomes greater than 200, reset the counter and define the next state.
  for (;;) {   
    switch (state)
        {
        case 0: // Initial state
            GLCD_SetBackColor(Red);
            GLCD_SetTextColor(White);
            GLCD_DisplayString(0,0,1,"   ARM RESET STATE 0  ");
            GLCD_SetBackColor(White);
            GLCD_SetTextColor(Blue);
            GLCD_DisplayString(3, 0, 1, "     Engine idle    ");
            GLCD_DisplayString(5, 0, 1, "   Waiting to Start  ");
            GLCD_DisplayString(7, 0, 1, "   Press wakeup key   ");
            timer_counter=0;
            break;
        case 1: // State after wakeup button pressed in state 0
      
            GLCD_SetTextColor(Black);
            GLCD_DisplayString(0, 0, 1, "ARM RUNNING STATE 1  ");
            GLCD_DisplayString(3, 0, 1, "                    ");
            GLCD_DisplayString(5, 0, 1, "      GEAR 1        ");
            GLCD_DisplayString(6, 0, 1, "                    ");
            GLCD_DisplayString(7, 0, 1, "                   ");
            if(timer_counter>80)
            {
              GLCD_Clear(White);
              timer_counter=0;
              state=2;

            }
            
            break;
        case 2: // State after user button pressed
            
            GLCD_SetTextColor(Black);
            GLCD_DisplayString(0, 0, 1, " ARM RUNNING STATE 2   ");
            GLCD_DisplayString(5, 0, 1, "      GEAR 2          ");
            GLCD_DisplayString(6, 0, 1, "                    ");
            GLCD_DisplayString(7, 0, 1, "                     ");
            if(timer_counter>150)
            {
              GLCD_Clear(White);
              timer_counter=0;
              state=3;
            }
            
            break;
        case 3: // State after tamper button pressed
            GLCD_SetBackColor(White);
            GLCD_SetTextColor(Red);
            
            GLCD_DisplayString(0,0,1,"  ARM RUNNING STATE 3    ");
            GLCD_DisplayString(5, 0, 1, "        Gear 3        ");
            GLCD_DisplayString(6, 0, 1, "    Adjust speed      ");
            GLCD_DisplayString(7, 0, 1, "  Press Tamper key   ");
            timer_counter=0;
            break;
        case 4: 
            GLCD_SetTextColor(Black);
            GLCD_DisplayString(0,0,1,"ARM RUNNING STATE 4");
            GLCD_DisplayString(5,0,1,"       Gear 4      ");
            GLCD_DisplayString(6, 0, 1, "                  ");
            GLCD_DisplayString(7, 0, 1, "                  ");
            if(timer_counter>200)
            {
              timer_counter=0;
              GLCD_Clear(White);

              state =0;
            }
            
            break;
        }       
          
  }
}

void EXTI0_IRQHandler() {           /* Wakeup button */
    EXTI->PR |= 1 <<0;
    if(state==0){
      GLCD_Clear(White);
      state=1;

    }
    else if(state==3)
    {
      for (int j = 0; j < LED_NUM; j++) {
                    GPIOE->BSRR = led_mask[j]; // turn on LED
                    // Add some delay
                    delay(3);
                    GPIOE->BRR = led_mask[j]; // turn off LED
                }
      GLCD_Clear(White);
      state=4;
      
    } 
    
}

void EXTI9_5_IRQHandler() {         /* User button */
    EXTI->PR |= 1 <<7;
    // use the condition when user is pressed
}

void EXTI15_10_IRQHandler() {        /* Tamper button */
    EXTI->PR |= 1 <<13;
    // use the condition when tamper is pressed
    if(state==3){
      GLCD_Clear(White);
      state=2;
    }
    
}

void TIM3_IRQHandler (void) { 
    if (TIM3->SR & (1<<0)) {                     /* UIF interrupt */
		// Increment counter
    timer_counter++;
	}
	TIM3->SR &= ~(1<<0);   
}