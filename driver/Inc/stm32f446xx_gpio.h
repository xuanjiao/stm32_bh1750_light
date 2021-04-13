/*
 * stm32f446xx_gpio.h
 *
 *  Created on: Feb 25, 2021
 *      Author: Xuanjiao Zhu
 */

#ifndef INC_STM32F446XX_GPIO_H_
#define INC_STM32F446XX_GPIO_H_

#include "stm32f446xx.h"

/*
 * This is a Pin configuration structure.
 * It is filled by the application, and the driver code extracts the data and applies the settings.
 */
typedef struct
{
	uint8_t GPIO_PinNumber;			/* Possible pin numbers are shown in @GPIO_PIN_NUMBERS */
	uint8_t GPIO_PinMode;			/* Possible pin modes are shown in @GPIO_PIN_MODES */
	uint8_t	GPIO_PinSpeed;			/* Possible pin speed are shown in @GPIO_PIN_SPEED */
	uint8_t GPIO_PinPuPdControl;	/* Possible pin pull-up/ pull-down resistor setting are shown in @GPIO_PIN_PU_PD */
	uint8_t GPIO_PinOPType;			/* Possible pin output types are shown in @GPIO_PIN_OUT_TYPES */
	uint8_t GPIO_PinAltFunMode;
}GPIO_PinConfig_t;

/*
 * This is a Handle structure for a GPIO pin.
 */
typedef struct
{
	GPIO_RegDef_t*		pGPIOx;				/* This pointer holds the base address of the GPIO port which the ping belongs to*/
	GPIO_PinConfig_t	GPIO_PinConfig;		/* This pointer holds the GPIO pin configuration setting */
}GPIO_Handle_t;

/*
 * @GPIO_PIN_NUMBERS
 * GPIO pin numbers
 */
#define GPIO_PIN_NO_1			1
#define GPIO_PIN_NO_2			2
#define GPIO_PIN_NO_3			3
#define GPIO_PIN_NO_4			4
#define GPIO_PIN_NO_5			5
#define GPIO_PIN_NO_6			6
#define GPIO_PIN_NO_7			7
#define GPIO_PIN_NO_8			8
#define GPIO_PIN_NO_9			9
#define GPIO_PIN_NO_10			10
#define GPIO_PIN_NO_11			11
#define GPIO_PIN_NO_12			12
#define GPIO_PIN_NO_13			13
#define GPIO_PIN_NO_14			14
#define GPIO_PIN_NO_15			15

/*
 * @GPIO_PIN_MODE
 * GPIO pin possible modes
 */
#define GPIO_MODE_IN			0
#define GPIO_MODE_OUT			1
#define GPIO_MODE_ALTFN			2
#define GPIO_MODE_ANALOG		3
#define GPIO_MODE_IT_RT			4
#define GPIO_MODE_IT_FT			5
#define GPIO_MODE_IT_RFT		6

/*
 * @GPIO_PIN_SPEED
 * GPIO pin possible speed
 */
#define GPIO_SPEED_LOW			0
#define GPIO_SPEED_MEDIUM		1
#define GPIO_SPEED_FAST			2
#define GPIO_SPEED_HIGH			3

/*
 * @GPIO_PIN_PU_PD
 * GPIO pin pull-up/ pull-down resistor setting
 */
#define GPIO_PIN_NO_PU_PD		0
#define GPIO_PIN_PU				1
#define GPIO_PIN_PD				2

/*
 * @GPIO_PIN_OUTPUT_TYPES
 * GPIO pin possible output types
 */
#define GPIO_OUT_TYPE_PP		0
#define GPIO_OUT_TYPE_OD		1

/**********************************************************************************************
 *								 APIs supported by this driver
 **********************************************************************************************/

/*
 * Peripheral clock configuration
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi);

/*
 * Init and De-Init
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx); 	/*Use reset register to reset the GPIO port */

/*
 * Data read and write
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber, uint8_t Value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

/*
 * IRQ (Interrupt Request) Configuration and ISR (interrupt service routine) handling
 */
void GPIO_IRQInterrputConfig(uint8_t IRQNumber, uint8_t EnorDi);
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);
void GPIO_IRQHandling(uint8_t PinNumber);

#endif /* INC_STM32F446XX_GPIO_H_ */
