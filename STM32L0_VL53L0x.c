#include <stm32l0xx_ll_bus.h>
#include <stm32l0xx_ll_gpio.h>
#include <stm32l0xx_ll_utils.h>
#include <stm32l0xx_ll_i2c.h>
#include <stm32l0xx_ll_usart.h>


//---------------| CL |------------------------
#include  "CL_CONFIG.h"
#include "CL_printMsg.h"
#include "CL_systemClockUpdate.h"
//---------------| Protoypes |------------------------
void init_i2c(void);
void i2c_write(uint8_t *data, uint8_t len);

void blinkLed(uint8_t delay, uint8_t count);
void initLed(void);

void initUART(void);

int main(void)
{	
	setClockTo32Mhz();
	LL_InitTick(32000000, 1000);
	initUART();
	initLed();
	init_i2c();
	

	CL_printMsg("init Success\n");
	uint8_t nombre[] = "EdwinFairchild";
	i2c_write(nombre, 14);
	
	for (;;)
	{
		
		CL_printMsg("Howdy\n", SystemCoreClock);
	
		LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_3);
		LL_mDelay(100);
		LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3);
		LL_mDelay(100);
	}
}//-------------------------------------------------------------------
void init_i2c(void)
{
	//clocks
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1); 
	
	//  [PA9]  = [D1] --> I2C1_SCL 
	//  [PA10] = [D0] --> I2C1_SDA 
	LL_GPIO_InitTypeDef gpio;
	
	gpio.Pin = LL_GPIO_PIN_9 | LL_GPIO_PIN_10;
	gpio.Mode = LL_GPIO_MODE_ALTERNATE;
	gpio.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	gpio.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	gpio.Pull = LL_GPIO_PULL_UP;
	gpio.Alternate = LL_GPIO_AF_1;
	LL_GPIO_Init(GPIOA, &gpio);

	// I2C1
	LL_I2C_InitTypeDef i2c ;
	i2c.Timing = 0x00707CBB; 
	i2c.TypeAcknowledge = LL_I2C_ACK;

	LL_I2C_StructInit(&i2c);
	LL_I2C_EnableAutoEndMode(I2C1);	
	LL_I2C_Init(I2C1, &i2c);	
	
	LL_I2C_Enable(I2C1);
	
}//-------------------------------------------------------------------
void i2c_write(uint8_t *data , uint8_t len)
{	
	LL_I2C_SetTransferSize(I2C1, len);
	LL_I2C_SetSlaveAddr(I2C1, 0x52);
	LL_I2C_GenerateStartCondition(I2C1);
	
	for (int i =  0; i < len; i++)
	{	
		while (!(I2C1->ISR & I2C_ISR_TXE) ) ;
		LL_I2C_TransmitData8(I2C1, *data++);		
	}
	
	//autoend enabled
	//LL_I2C_GenerateStopCondition(I2C1);
}
void initUART(void)
{
	/*  For use with Virtual Com port of the L0 Nucleo
	 *  [PA2] [USART_2_TX : AF 4 ]<--->[PA15] [USAR_2__RX : AF 4 ]
	 *	  
	*/
	
	// Clocks
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
	
	// GPIO 
	LL_GPIO_InitTypeDef uartPins;
	LL_GPIO_StructInit(&uartPins);	
	
	uartPins.Pin		= LL_GPIO_PIN_2;//| LL_GPIO_PIN_15; 
	uartPins.Mode		= LL_GPIO_MODE_ALTERNATE;
	uartPins.Alternate	= LL_GPIO_AF_4;		
	LL_GPIO_Init(GPIOA, &uartPins);	
	
	// USART 2
	LL_USART_InitTypeDef usart2;
	LL_USART_StructInit(&usart2); //defualt values work
	
	usart2.BaudRate = 115200U;	
	
	LL_USART_Init(USART2, &usart2);
	LL_USART_Enable(USART2);
	
	
	

}
void blinkLed(uint8_t delay, uint8_t count)
{
	for (int i = 0; i < count; i++)
	{		
		LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3);
		LL_mDelay(1000);
		LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_3);
		LL_mDelay(1000);
	}
}//-------------------------------------------------------------------
void initLed(void)
{
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_3, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_3, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_3, LL_GPIO_SPEED_FREQ_LOW);
	
}//-------------------------------------------------------------------
