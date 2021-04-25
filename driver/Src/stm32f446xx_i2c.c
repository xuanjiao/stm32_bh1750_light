#include "stm32f446xx.h"

uint8_t AHB_prescale[] = {2,4,8,16,64,128,256,512};
uint8_t APB_prescale[] = {2,4,8,16};

/*********************************************************************
 * @fn      		  - I2C_
 *
 * @brief             - This function
 *
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none
 */

/*********************************************************************
 * @fn      		  - RCC_GetPCLK1Value
 *
 * @brief             - This function calculate the clock rate of APB1 bus
 *
 * @param[in]         - none
 *
 * @return            - pclk1 clock rate in Hz
 *
 * @Note              - none
 */
uint32_t RCC_GetPCLK1Value()
{
	uint32_t sysclk,pclk1 = 0;
	uint8_t clksrc,temp1,temp2,ahb1p,apb1p;

	clksrc = ( RCC->CFGR >> 2 ) & 0x03;

	if ( clksrc == 0 ) 			// System clock = HSI oscillator
	{
		sysclk = 16000000;
	}else if ( clksrc == 1) 	// System clock = HSE oscillator
	{
		sysclk = 8000000;
	}else if ( clksrc == 2)		// System clock = PLL
	{
		// not use yet
	}

	// Calculate AHB clock prescaler
	temp1 = (RCC->CFGR >> 4) & 0x0F;
	if ( temp1 < 0x08 )
	{
		ahb1p = 1;
	}else
	{
		ahb1p = AHB_prescale[temp1 - 8]; // AHB clock = System clock /AHB prescaler
	}

	// Calculate APB1 clock prescaler
	temp2 = (RCC->CFGR >> 10) & 0x07;
	if ( temp2 < 0x04 )
	{
		apb1p = 1;
	}else
	{
		apb1p = APB_prescale[temp2 - 4]; 	// APB1 clock = System clock / APB1 prescaler
	}
	pclk1 = ( sysclk / ahb1p) / apb1p;

	return pclk1;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
}

/*********************************************************************
 * @fn      		  - I2C_Init
 *
 * @brief             - This function is to initialize the I2C setting
 *
 * @param[in]         - Pointer to I2C handle structure
 *
 * @return            - none
 *
 * @Note              - none
 */
void I2C_Init(I2C_Handle_t *pI2CHandle)
{

	uint16_t tempreg, ccr;

	// Control register 1
	tempreg = (  pI2CHandle->I2C_Config.I2C_ACKControl  << I2C_CR1_ACK);
	pI2CHandle->pI2Cx->CR1 |= tempreg;

	// Control register 2
	tempreg = 0;
	tempreg |= RCC_GetPCLK1Value()/1000000U;
	pI2CHandle->pI2Cx->CR2 |= tempreg & (0x3F);

	// Own address register 1
	tempreg = 0;
	tempreg |= pI2CHandle->I2C_Config.I2C_DeviceAddress << 1; // 7-bit slave address
	tempreg |= (1 << 14); 	// Bit 14 should always be kept at 1 by software
	pI2CHandle->pI2Cx->OAR1 = tempreg;

	// CCR register
	tempreg = 0;
	if (pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM_1K)
	{
		// Standard mode
		ccr = RCC_GetPCLK1Value() / (2 * pI2CHandle->I2C_Config.I2C_SCLSpeed);
		tempreg |= ccr & (0xFFF);
	}else
	{
		// Fast mode
		tempreg |= (1 << I2C_CCR_FS);
		tempreg |= (I2C_Config.I2C_FMDutyCycle << I2C_CCR_DUTY);
		if (pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FMDUTY_2)
		{
			ccr = RCC_GetPCLK1Value() / (3 * pI2CHandle->I2C_Config.I2C_SCLSpeed);
		}else
		{
			ccr = RCC_GetPCLK1Value() / (25 * pI2CHandle->I2C_Config.I2C_SCLSpeed);
		}
		tempreg |= ccr & (0xFFF);
	}
		pI2CHandle->pI2Cx->CCR = tempreg;





}

void I2C_DeInit(I2C_RegDef_t *pI2Cx);

void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pI2Cx->CR1 |= ( 1 << I2C_CR1_PE);
	}else
	{
		pI2Cx->CR1 &= ~( 1 << I2C_CR1_PE);
	}

}

/*********************************************************************
 * @fn      		  - I2C_GetFlagStatus
 *
 * @brief             - This function get the I2C status in SR register
 *
 * @param[in]         - the pin number
 *
 * @return            -  none
 *
 * @Note              -  none
 */
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx , uint32_t FlagName)
{
	if( pI2Cx->SR1 & FlagName )
	{
		return FLAG_SET;
	}else
		return FLAG_RESET;
}
