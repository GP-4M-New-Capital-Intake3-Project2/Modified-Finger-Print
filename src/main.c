/************************************************************************
 * Author           :Sabry Elsayed
 * File description :main.c
 * Date             :20/10/2023
 * Target           :STM32F401
 * Version          :v0.1
 ************************************************************************/

#include "LIB/STD_TYPES.h"
#include "LIB/BIT_MATH.h"
#include "MCAL/RCC/RCC_interface.h"
#include "MCAL/GPIO/GPIO_Interface.h"
#include "MCAL/UART/UART_prv.h"
#include "MCAL/UART/UART_int.h"
#include "MCAL/STK/STK_int.h"
#include "MCAL/NVIC/NVIC_interface.h"
#include "HAL/Finger-Print/Finger-print.h"
#include "HAL/LCD/LCD.h"
/*************************************************************************/
//To indicate program mode in REGISTER_MODE
#define REGISTER_MODE   'R'
//To indicate program mode in SEARCH_MODE
#define SEARCH_MODE     'A'
//To indicate program mode in IDLE_MODE
#define IDLE_MODE       'I'

/*************************used with uart init************************************/
extern USART_InitType uart2_cfg ;
extern USART_InitType uart1_cfg ;

extern GPIO_PinConfig uart_pinTx ;
extern GPIO_PinConfig uart_pinRx ;

extern GPIO_PinConfig uart_pinTx1 ;
extern GPIO_PinConfig uart_pinRx1 ;

u8 Returen_RX;
u8 garbage;
/*******************************************************************************/
//To display user finger print that was already registered
extern u8 id;
//to store user finger print id that exist into finger print module flash memory
u16 ID ;
//To store program state
u8 status = IDLE_MODE;

int main(void){

	/****************************Enable RCC for UART1 , UART2 , PORTA , PORTB******************/
	RCC_voidInit();
	RCC_u8EnablePeripheralClock(RCC_APB1 ,RCC_EN_UART2 );
	RCC_u8EnablePeripheralClock(RCC_APB2 ,RCC_EN_UART1 );
	RCC_u8EnablePeripheralClock(RCC_AHB1 , RCC_AHB1_GPIOA);
	RCC_u8EnablePeripheralClock(RCC_AHB1 , RCC_AHB1_GPIOB);
	RCC_u8EnablePeripheralClock(RCC_APB2 , RCC_APB2_SYSCFG);
	/*******************************************************************************************/
	//Initialize SYSTIC Timer
	MSTK_vInit(); 	//HSI 16 MH / 8 = 2MH //TickTime = .5 us

	//To configure engine pin
	GPIO_PinConfig  Engine =
	{
			.Port = GPIO_PORTA,
			.Pin = GPIO_PIN0,
			.Mode = GPIO_MODE_OUTPUT,
			.OutputSpeed = GPIO_OUTPUT_SPEED_LOW,
			.OutputType = GPIO_OUTPUT_PUSH_PULL
	};

	//To configure register_button pin
	GPIO_PinConfig  register_button = {
			.Port = GPIO_PORTB,
			.Pin = GPIO_PIN0,
			.Mode = GPIO_MODE_INPUT,
			.OutputSpeed = GPIO_OUTPUT_SPEED_LOW,
			.PullState  = GPIO_PULL_UP,

	};

	//To configure search_button pin
	GPIO_PinConfig  search_button = {
			.Port = GPIO_PORTB,
			.Pin = GPIO_PIN1,
			.Mode = GPIO_MODE_INPUT,
			.OutputSpeed = GPIO_OUTPUT_SPEED_LOW,
			.PullState  = GPIO_PULL_UP,

	};

/**********************To initialize GPIO PINS for UART1 , UART2 , ****************************/
 /*********************Engine pin , register and search buttons pins *************************/

	GPIO_voidInit(&uart_pinTx);
	GPIO_voidInit(&uart_pinRx);
	GPIO_voidInit(&uart_pinTx1);
	GPIO_voidInit(&uart_pinRx1);
	GPIO_voidInit(&register_button);
	GPIO_voidInit(&search_button);
	GPIO_voidInit(&Engine);
/**********************************************************************************************/


	/*MUSART Initialization*/
	MUSART_vInit(&uart2_cfg,USART2);
	MUSART_vInit(&uart1_cfg,USART1);

	/*Enable USARTx*/
	MUSART_vEnable(USART2);
	MUSART_vEnable(USART1);

	/*Initialize LCD*/
	LCD_Init();
	LCD_GoTo(0,0);
	LCD_WriteString((u8*)"Welcome");


	while(1)
	{

		//Check if there any received data over uart1 from GUI
		 Returen_RX = MUSART_u8ReceiveByteAsynch_(USART1 , &status);
		/****************Check if UART1 received REGISTER_MODE*********************/
		if(status == REGISTER_MODE && Returen_RX == 1)
		{

            /***********to make sure user is register successfully***************/
			while(FP_Register() != OK)
			{
				//To send the gui by the current status ,G->ask user to try again
				MUSART_vTransmitByteSynch(USART1,'G');
				MUSART_u8ReceiveByteAsynch_(USART1 , &garbage);
			}
			/**************Register process succeeded***************************/

			//To send the gui by the current status ,S->inform user that registeration succeeded
			MUSART_vTransmitByteSynch(USART1,'S');
			MUSART_u8ReceiveByteAsynch_(USART1 , &garbage);
			status = IDLE_MODE;

		}
		/****************Check if UART1 received SEARCH_MODE*********************/
		else if(status == SEARCH_MODE && Returen_RX == 1)
		{

			/****************Check if user finger print is exist in data base*****/
			if(FP_Search(&ID) == OK)
			{
				/*************user finger print is exist**************************/

				//To send the gui by the current status ,S->inform user that his finger print exist
				GPIO_voidSetPinValue(Engine.Port , Engine.Pin ,GPIO_HIGH);
				MUSART_vTransmitByteSynch(USART1,'S');
				MUSART_u8ReceiveByteAsynch_(USART1 , &garbage);

			}
			else
			{
			/*************user finger print is not exist**************************/
				//To send the gui by the current status ,S->inform user that his finger print is not exist
				MUSART_vTransmitByteSynch(USART1,'F');
				MUSART_u8ReceiveByteAsynch_(USART1 , &garbage);

			}
			status = IDLE_MODE;

		}
		/*************************to return to idle mode*************************/
		else if(status == IDLE_MODE)
		{


		}




	}
	return 0;
}
