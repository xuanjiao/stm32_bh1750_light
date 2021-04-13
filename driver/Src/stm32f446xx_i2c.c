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
uint32_t RCC_GetPCLK1Value()
{
	uint32_t pclk1;
	uint8_t clksrc,hpre;

	clksrc = ( RCC->CFGR >> 2 ) & (uint16_t)0x03;

	if ( clksrc == 0 ) 			// System clock = HSI oscillator
	{
		pclk1 = 16000000;
	}else if ( clksrc == 1) 	// System clock = HSE oscillator
	{
		pck1 = 8000000;
	}else if ( clksrc == 2)		// System clock = PLL
	{
		// not use yet
	}

	hpre = (RCC->CFGR >> 4) & (uint16_t)0x0F;



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

	uint16_t ccr_reg  = 0, ccr_var = 0;

	pI2CHandle->pI2Cx->I2C_CR1 |= (  pI2CHandle->I2C_Config.I2C_ACKControl  << I2C_CR1_ACK);

	if (pI2CHandle->I2C_Config.I2C_SCLSpeed == I2C_SCL_SPEED_SM_1K)
	{
		ccr_reg |= ( 1 << I2C_CCR_FS);
		ccr_var = 2 * I2C_SCL_SPEED_SM_1K /

	}

	pI2CHandle->pI2Cx->I2C_CR1 = ccr_reg;
}







void I2C_DeInit(I2C_RegDef_t *pI2Cx);

void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pI2Cx->I2C_CR1 |= ( 1 << I2C_CR1_PE);
	}else
	{
		pI2Cx->I2C_CR1 &= ~( 1 << I2C_CR1_PE);
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
	if(pI2Cx->SR & FlagName)
	{
		return FLAG_SET;
	}else
		return FLAG_RESET;
}
