/*
 * stm32f446xx_gpio.c
 *
 *  Created on: Feb 25, 2021
 *      Author: Xuanjiao Zhu
 */

#include "stm32f446xx_gpio.h"

/*********************************************************************
 * @fn      		  - GPIO_PeriClockControl
 *
 * @brief             - This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]         - base address of the GPIO peripheral
 * @param[in]         - ENABLE or DISABLE macros
 *
 * @return            -  none
 *
 * @Note              -  none
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi){

	if(EnorDi == ENABLE){

		if(pGPIOx == GPIOA){
			GPIOA_PCLK_EN();
		}else if(pGPIOx == GPIOB){
			GPIOB_PCLK_EN();
		}else if(pGPIOx == GPIOC){
			GPIOC_PCLK_EN();
		}else if(pGPIOx == GPIOD){
			GPIOD_PCLK_EN();
		}else if(pGPIOx == GPIOE){
			GPIOE_PCLK_EN();
		}else if(pGPIOx == GPIOF){
			GPIOF_PCLK_EN();
		}else if(pGPIOx == GPIOG){
			GPIOG_PCLK_EN();
		}else if(pGPIOx == GPIOH){
			GPIOH_PCLK_EN();
		}
	}else{

		if(pGPIOx == GPIOA){
			GPIOA_PCLK_DI();
		}else if(pGPIOx == GPIOB){
			GPIOB_PCLK_DI();
		}else if(pGPIOx == GPIOC){
			GPIOC_PCLK_DI();
		}else if(pGPIOx == GPIOD){
			GPIOD_PCLK_DI();
		}else if(pGPIOx == GPIOE){
			GPIOE_PCLK_DI();
		}else if(pGPIOx == GPIOF){
			GPIOF_PCLK_DI();
		}else if(pGPIOx == GPIOG){
			GPIOG_PCLK_DI();
		}else if(pGPIOx == GPIOH){
			GPIOH_PCLK_DI();
		}
	}
}



/*********************************************************************
 * @fn      		  - GPIO_Init
 *
 * @brief             - This function initialize the GPIO port
 *
 * @param[in]         - pointer to a GPIO handle structure
 *
 * @return            -  none
 *
 * @Note              -  none
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle){

	// Enable the GPIO peripheral clock
	GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);

	uint32_t temp = 0; // Store the configuration data

	// Configure the pin mode
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode < GPIO_MODE_ANALOG)
	{
		temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		pGPIOHandle->pGPIOx->MODE &= ~( 0x3 << 2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); // Clear the current value
		pGPIOHandle->pGPIOx->MODE |= temp;	// Setting
	}else
	{
		// Interrupt rising edge and falling edge setting
		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT)
		{
			EXTI->EXTI_RTSR |= ( 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); // Rising trigger enable
			EXTI->EXTI_FTSR &= ~( 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);	// Falling trigger disable
		}else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT)
		{
			EXTI->EXTI_RTSR &= ~(  1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); // Rising trigger disable
			EXTI->EXTI_FTSR |= ( 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);	// Falling trigger enable
		}else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT)
		{
			EXTI->EXTI_RTSR |= ( 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); // Rising trigger enable
			EXTI->EXTI_FTSR |= ( 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);	// Falling trigger enable
		}


		// Configure GPIO port selection in SYSCFG
		SYSCFG_PCLK_EN();

		uint8_t temp1,temp2,port_code;
		temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;
		temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;

		port_code = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
		SYSCFG->SYSCFG_EXTICR[temp1] &= ~( 0xF << 4 * temp2 );	// Clear the current value
		SYSCFG->SYSCFG_EXTICR[temp1] |= ( port_code << 4 * temp2);

		// Enable interrupt on pin using IMR (Interrupt mask register)
		EXTI->EXTI_IMR |= ( 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

	}

	// Configure the pin speed
	temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->OSPEEDER &= ~( 0x3 << 2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed); // Clear the current value
	pGPIOHandle->pGPIOx->OSPEEDER |= temp;	// Setting

	// Configure the pin pull-down/pull-up resistor
	temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->PUPDR &= ~( 0x3 << 2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); // Clear the current value
	pGPIOHandle->pGPIOx->PUPDR |= temp;		// Setting

	// Configure the pin output type
	temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->OTYPER &= ~( 0x1 << 2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); // Clear the current value
	pGPIOHandle->pGPIOx->OTYPER |= temp;	// Setting

	// Configure the pin alternate function selection when the pin mode is alternate function mode.
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
	{
		uint8_t temp1, temp2;
		temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8;		// Select from alternate function low register or alternate function high register
		temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8; 	// Select the pin x configuration bits
		pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xF << (4 * temp2 ));		// Clear the current value
		temp  = pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << ( 4 * temp2 );
		pGPIOHandle->pGPIOx->AFR[temp1] |= temp;		// Setting
	}

}

/*********************************************************************
 * @fn      		  - GPIO_DeInit
 *
 * @brief             - This function use reset register to reset the GPIO port
 *
 * @param[in]         - base address of the GPIO port
 *
 * @return            -  none
 *
 * @Note              -  none
 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
	if(pGPIOx == GPIOA){
		GPIOA_REG_RESET();
	}else if(pGPIOx == GPIOB){
		GPIOB_REG_RESET();
	}else if(pGPIOx == GPIOC){
		GPIOC_PCLK_EN();
	}else if(pGPIOx == GPIOD){
		GPIOD_REG_RESET();
	}else if(pGPIOx == GPIOE){
		GPIOE_REG_RESET();
	}else if(pGPIOx == GPIOF){
		GPIOF_REG_RESET();
	}else if(pGPIOx == GPIOG){
		GPIOG_REG_RESET();
	}else if(pGPIOx == GPIOH){
		GPIOH_REG_RESET();
	}
}


/*********************************************************************
 * @fn      		  - GPIO_ReadFromInputPin
 *
 * @brief             - This function read the state of a GPIO input pin
 *
 * @param[in]         - base address of the GPIO port
 * @param[in]         - the input pin to be read
 *
 * @return            - the input pin value
 *
 * @Note              - none
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	uint8_t value;
	value = (uint8_t)(pGPIOx->IDR >> PinNumber) & (0x00000001);
	return value;
}

/*********************************************************************
 * @fn      		  - GPIO_ReadFromInputPort
 *
 * @brief             - This function read the state of a GPIO port
 *
 * @param[in]         - base address of the GPIO port
 *
 * @return            - the input port value
 *
 * @Note              -  none
 */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t value;
	value = (uint16_t) pGPIOx->IDR;
	return value;
}

/*********************************************************************
 * @fn      		  - GPIO_WriteToOutputPin
 *
 * @brief             - This function set or clear the GPIO pin
 *
 * @param[in]         - base address of the GPIO port
 * @param[in]         - the output pin to be written
 * @param[in]         - the value to be written to the selected bit
 *
 * @return            -  none
 *
 * @Note              -  none
 */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value)
{
	if(Value == GPIO_PIN_SET)
	{
		pGPIOx->ODR |= ( 0x1 << PinNumber);
	}else
	{
		pGPIOx->ODR &= ~( 0x1 << PinNumber);
	}
}

/*********************************************************************
 * @fn      		  - WriteToOutputPort
 *
 * @brief             - This function set or clear all pins of the GPIO port
 *
 * @param[in]         - base address of the GPIO port
 * @param[in]         - the value to be written to the GPIO port
 *
 * @return            -  none
 *
 * @Note              -  none
 */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value)
{
	pGPIOx->ODR &= Value;
}

/*********************************************************************
 * @fn      		  - GPIO_ToggleOutputPin
 *
 * @brief             - This function toggle the GPIO pin
 *
 * @param[in]         - base address of the GPIO port
 * @param[in]         - the pin to be toggled
 *
 * @return            -  none
 *
 * @Note              -  none
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	pGPIOx->ODR ^= ( 0x1 << PinNumber);
}


/*********************************************************************
 * @fn      		  - GPIO_IRQInterrputConfig
 *
 * @brief             - This function enable the interrupt in processor's perspective
 *
 * @param[in]         -	the IRQ number
 * @param[in]         - ENABLE or DISABLE macros
 *
 * @return            -  none
 *
 * @Note              -  none
 */
void GPIO_IRQInterrputConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
	if( EnorDi == ENABLE)
	{
		if( IRQNumber < 32)
		{
			*NVIC_ISER0 |= ( 1 << IRQNumber);

		}else if (IRQNumber >= 32 && IRQNumber < 64)
		{
			*NVIC_ISER1 |= ( 1 << IRQNumber % 32);

		}else if (IRQNumber >= 64 && IRQNumber < 96)
		{
			*NVIC_ISER2 |= ( 1 << IRQNumber % 64);

		}else if (IRQNumber >= 96 && IRQNumber < 128)
		{
			*NVIC_ISER3 |= ( 1 << IRQNumber % 96);

		}else{}
	}else
	{
		if( IRQNumber < 32)
		{
			*NVIC_ICER0 |= ( 1 << IRQNumber);

		}else if (IRQNumber >= 32 && IRQNumber < 64)
		{
			*NVIC_ICER1 |= ( 1 << IRQNumber % 32);

		}else if (IRQNumber >= 64 && IRQNumber < 96)
		{
			*NVIC_ICER2 |= ( 1 << IRQNumber % 64);

		}else if (IRQNumber >= 96 && IRQNumber < 128)
		{
			*NVIC_ICER3 |= ( 1 << IRQNumber % 96);

		}else{}
	}
}

/*********************************************************************
 * @fn      		  - GPIO_IRQPriorityConfig
 *
 * @brief             - This function configure the IRQ Priority
 *
 * @param[in]         - the IRQ number
 * @param[in]         - the IRQ priority
 *
 * @return            -  none
 *
 * @Note              -  none
 */

void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority)
{
	uint8_t temp1, temp2;

	// Find out the IPR (Interrupt Priority Registers)
	temp1 = IRQNumber / 4;
	temp2 = IRQNumber % 4;

	*(NVIC_IPR_BASEADDR + temp1) |= IRQPriority <<  (8 * temp2 + 8 - NO_PR_BITS_IMPLEMENTED);
}

/*********************************************************************
 * @fn      		  - GPIO_IRQHandling
 *
 * @brief             - This function clear the corresponding bit in EXTI pending register
 *
 * @param[in]         - the pin number
 *
 * @return            -  none
 *
 * @Note              -  none
 */
void GPIO_IRQHandling(uint8_t PinNumber)
{

	if( EXTI->EXTI_PR & ( 1 << PinNumber) ) // Check if the corresponding bit in the pending register is set
	{
		EXTI->EXTI_PR |= ( 1 << PinNumber); // Clear the EXTI pending register bit
	}
}


