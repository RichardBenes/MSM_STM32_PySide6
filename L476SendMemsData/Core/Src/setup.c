/*
 * setup.c
 *
 *  Created on: 23. 2. 2026
 *      Author: Amper
 */

#include "setup.h"

void setup_gpio() {

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;

	/* B2 - red
	* E8 - green */

	GPIOB->MODER &= ~GPIO_MODER_MODE2;
	GPIOB->MODER |= GPIO_MODE_OUT << GPIO_MODER_MODE2_Pos; /* Set B2 as general purpose out */

	GPIOE->MODER &= ~GPIO_MODER_MODE8;
	GPIOE->MODER |= GPIO_MODE_OUT << GPIO_MODER_MODE8_Pos; /* Set E8 as general purpose out */

	/* A0, A1, A2, A3 and A5 are joystick button inputs */

	GPIOA->MODER &= ~GPIO_MODER_MODE0 /* Set to GPIO input mode */
		  & ~GPIO_MODER_MODE1
		  & ~GPIO_MODER_MODE2
		  & ~GPIO_MODER_MODE3
		  & ~GPIO_MODER_MODE5;

	GPIOA->PUPDR |= (GPIO_PP_PD << GPIO_PUPDR_PUPD0_Pos) /* Enable pull-downs */
		  | (GPIO_PP_PD << GPIO_PUPDR_PUPD1_Pos) | (GPIO_PP_PD << GPIO_PUPDR_PUPD2_Pos)
		  | (GPIO_PP_PD << GPIO_PUPDR_PUPD3_Pos) | (GPIO_PP_PD << GPIO_PUPDR_PUPD5_Pos);
}

void setup_uart() {

	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;

	/* Setup UART
	* PD5 - USART2_TX - AF7
	* PD6 - USART2_RX - AF7
	*  */
	GPIOD->MODER &= ~GPIO_MODER_MODE5 & ~GPIO_MODER_MODE6; /* Set as alternate function */
	GPIOD->MODER |= (GPIO_MODE_AF << GPIO_MODER_MODE5_Pos) | (GPIO_MODE_AF << GPIO_MODER_MODE6_Pos);

	/* Set PD5 and PD6 as AF7 */
	GPIOD->AFR[0] |= (7U << GPIO_AFRL_AFSEL5_Pos) | (7U << GPIO_AFRL_AFSEL6_Pos);

	/* Set PD5 and PD6 to very high output speed */
	GPIOD->OSPEEDR |= (GPIO_OSP_VHI << GPIO_OSPEEDR_OSPEED5_Pos)
		  | (GPIO_OSP_VHI << GPIO_OSPEEDR_OSPEED6_Pos);

	/* Baud rate
	* Oversampling - by default it's 16, let's keep it.
	* I want to send around 32B of data 120 times per second
	* That is 32 B * 120 s^-1 = 3840 B/s
	* 3840 B/s * 8 b/B = 30 720 b/s
	* Considering one start bit and one stop bit per every byte
	* it is ( 32 B * 120 s^-1 * ( 8 + 2 ) b/B) = 38 400 b/s
	* Let's use 230 400 b/s - that is 23 040 B/s - 720 32B messages per second
	* And it's also a standard value.
	*
	* UART2 is on APB1, so it gets clock frequency of f_ck = 80 MHz
	* Since I use default oversampling, it's BAUD = f_ck / USARTDIV
	* Therefore USARTDIV = f_ck / BAUD = 80 MHz / 230 400 b/s = 347.2222
	* Rounded down to USARTDIV = 347, the BAUD is
	* 80 MHz / 347 = 230 547.55 b/s ... hopefully that's close enough. */
	USART2->BRR = 347U;
	USART2->CR1 |= USART_CR1_UE;
	USART2->CR1 |= USART_CR1_TE;
}

