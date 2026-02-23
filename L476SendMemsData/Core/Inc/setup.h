/*
 * setup.h
 *
 *  Created on: 23. 2. 2026
 *      Author: Amper
 */

#ifndef INC_SETUP_H_
#define INC_SETUP_H_

#include "stm32l476xx.h"

#define GPIO_MODE_IN 0U
#define GPIO_MODE_OUT 1U
#define GPIO_MODE_AF 2U
#define GPIO_MODE_ANLG 3U

#define GPIO_PP_NONE 0U
#define GPIO_PP_PU 1U
#define GPIO_PP_PD 2U

#define GPIO_OSP_LOW 0U
#define GPIO_OSP_MED 1U
#define GPIO_OSP_HI 2U
#define GPIO_OSP_VHI 3U

void setup_gpio();
void setup_uart();


#endif /* INC_SETUP_H_ */
