/*
 * stm32f446xx_gpio.h
 *
 *  Created on: April 13, 2021
 *      Author: Xuanjiao Zhu
 */

#ifndef INC_STM32F446XX_I2C_H_
#define INC_STM32F446XX_I2C_H_

#include "stm32f446xx.h"
/*
 * This is a Pin configuration structure.
 * It is filled by the application, and the driver code extracts the data and applies the settings.
 */
typedef struct
{
	uint32_t I2C_SCLSpeed;			/* Possible SCL speed are shown in @I2C_SCLSpeed */
	uint8_t	 I2C_DeviceAddress;		/* User define*/
	uint8_t	 I2C_ACKControl;		/* Possible ACK control are shown in @I2C_ACKControl */
	uint16_t I2C_FMDutyCycle;		/* Possible Fast Mode Duty cycle (t_HIGH / t_LOW) are shown in @I2C_FMDutyCycle */
}I2C_Config_t;

/*
 *	Handle structure for I2Cx peripheral
 */
typedef struct
{
	I2C_RegDef_t	*pI2Cx;
	I2C_Config_t	I2C_Config;
}I2C_Handle_t;

/*
 * @I2C_SCLSpeed
 */
#define I2C_SCL_SPEED_SM_1K		100000
#define I2C_SCL_SPEED_FM_2K		200000
#define I2C_SCL_SPEED_FM_4K		400000

/*
 * @I2C_ACKControl
 */
#define I2C_ACK_ENABLE			1
#define I2C_ACK_DISABLE			0

/*
 * @I2C_FMDutyCycle
 */
#define I2C_FMDUTY_2				0
#define I2C_FMDUTY_16_9			1

/*
 * I2C related status flags definitions
 */
#define I2C_TXE_FLAG		( 1 << I2C_SR1_TXE)
#define I2C_RXNE_FLAG		( 1 << I2C_SR1_RXNE)
/**********************************************************************************************
 *								 APIs supported by this driver
 **********************************************************************************************/

/*
 * Peripheral clock configuration
 */
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

/*
 * Init and De-Init
 */
void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInit(I2C_RegDef_t *pI2Cx);

/*
 * Data read and write
 */

/*
 * IRQ (Interrupt Request) Configuration and ISR (interrupt service routine) handling
 */
void I2C_IRQInterrputConfig(uint8_t IRQNumber, uint8_t EnorDi);
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);

/*
 * Other Peripheral Control APIs
 */
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx , uint32_t FlagName);

/*
 * Application callback
 */
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle,uint8_t AppEv);

#endif /* INC_STM32F446XX_I2C_H_ */
