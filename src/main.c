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



extern USART_InitType uart2_cfg ;
extern USART_InitType uart1_cfg ;

#define FALSE           0
#define TRUE            1
#define REGISTER_MODE   'R'
#define SEARCH_MODE     'A'
#define IDLE_MODE       'I'
#define MODULE_DELAY    1000000
#define DELAY           1000
u8 read1_sucess=0;
u8 read2_sucess=0;
u8 status = IDLE_MODE;

u8 id = 20;
extern u8 pageNo1;
extern u8 pageNo2;

void read_udr(void)
{
	status = MUSART_u8ReadDataRegisterAsynch(USART1);
	MUSART_voidClearFlags(USART1);
	NVIC_voidDisablePeripheral(37);

}
int main(void){


	RCC_voidInit();
	RCC_u8EnablePeripheralClock(RCC_APB1 ,RCC_EN_UART2 );
	RCC_u8EnablePeripheralClock(RCC_APB2 ,RCC_EN_UART1 );

	RCC_u8EnablePeripheralClock(RCC_AHB1 , RCC_AHB1_GPIOA);
	RCC_u8EnablePeripheralClock(RCC_AHB1 , RCC_AHB1_GPIOB);

	MSTK_vInit(); 	//HSI 16 MH / 8 = 2MH //TickTime = .5 us
	RCC_u8EnablePeripheralClock(RCC_APB2 , RCC_APB2_SYSCFG);

	GPIO_PinConfig uart_pinTx =
	{
			.Port = GPIO_PORTA,
			.Pin  = GPIO_PIN2 ,
			.Mode = GPIO_MODE_ALTERNATIVE,
			.OutputSpeed = GPIO_OUTPUT_SPEED_MEDIUM,
			.OutputType = GPIO_OUTPUT_PUSH_PULL,
			.alt_func = 7,
	};
	GPIO_PinConfig uart_pinRx =
	{
			.Port = GPIO_PORTA,
			.Pin  = GPIO_PIN3 ,
			.Mode = GPIO_MODE_ALTERNATIVE,
			.OutputSpeed = GPIO_OUTPUT_SPEED_MEDIUM,
			.PullState = GPIO_NO_PULL,
			//.OutputType = GPIO_OUTPUT_PUSH_PULL,
			.alt_func = 7,
	};

	GPIO_PinConfig uart_pinTx1 =
	{
			.Port = GPIO_PORTA,
			.Pin  = GPIO_PIN9 ,
			.Mode = GPIO_MODE_ALTERNATIVE,
			.OutputSpeed = GPIO_OUTPUT_SPEED_MEDIUM,
			.OutputType = GPIO_OUTPUT_PUSH_PULL,
			.alt_func = 7,
	};
	GPIO_PinConfig uart_pinRx1 =
	{
			.Port = GPIO_PORTA,
			.Pin  = GPIO_PIN10 ,
			.Mode = GPIO_MODE_ALTERNATIVE,
			.OutputSpeed = GPIO_OUTPUT_SPEED_MEDIUM,
			.PullState = GPIO_NO_PULL,
			//.OutputType = GPIO_OUTPUT_PUSH_PULL,
			.alt_func = 7,
	};
	GPIO_PinConfig  Engine =
	{
			.Port = GPIO_PORTA,
			.Pin = GPIO_PIN0,
			.Mode = GPIO_MODE_OUTPUT,
			.OutputSpeed = GPIO_OUTPUT_SPEED_LOW,
			.OutputType = GPIO_OUTPUT_PUSH_PULL
	};

	GPIO_PinConfig LCD[10] =
	{
			{.Port = GPIO_PORTB,.Pin  = GPIO_PIN5 ,.Mode = GPIO_MODE_OUTPUT,.OutputSpeed = GPIO_OUTPUT_SPEED_LOW,.OutputType = GPIO_OUTPUT_PUSH_PULL},
			{.Port = GPIO_PORTB,.Pin  = GPIO_PIN6 ,.Mode = GPIO_MODE_OUTPUT,.OutputSpeed = GPIO_OUTPUT_SPEED_LOW,.OutputType = GPIO_OUTPUT_PUSH_PULL},
			{.Port = GPIO_PORTB,.Pin  = GPIO_PIN7 ,.Mode = GPIO_MODE_OUTPUT,.OutputSpeed = GPIO_OUTPUT_SPEED_MEDIUM,.OutputType = GPIO_OUTPUT_PUSH_PULL},
			{.Port = GPIO_PORTB,.Pin  = GPIO_PIN8 ,.Mode = GPIO_MODE_OUTPUT,.OutputSpeed = GPIO_OUTPUT_SPEED_MEDIUM,.OutputType = GPIO_OUTPUT_PUSH_PULL},
			{.Port = GPIO_PORTB,.Pin  = GPIO_PIN9 ,.Mode = GPIO_MODE_OUTPUT,.OutputSpeed = GPIO_OUTPUT_SPEED_MEDIUM,.OutputType = GPIO_OUTPUT_PUSH_PULL},
			{.Port = GPIO_PORTB,.Pin  = GPIO_PIN10 ,.Mode = GPIO_MODE_OUTPUT,.OutputSpeed = GPIO_OUTPUT_SPEED_MEDIUM,.OutputType = GPIO_OUTPUT_PUSH_PULL},

	};
	for(u8 i = 0 ; i < 6 ; i++)
	{
		GPIO_voidInit(&LCD[i]);
	}
	GPIO_voidInit(&uart_pinTx);
	GPIO_voidInit(&uart_pinRx);
	GPIO_voidInit(&uart_pinTx1);
	GPIO_voidInit(&uart_pinRx1);
	GPIO_voidInit(&Engine);

	///////////////////////////////////////////////////////////////////////
	/*5-MUSART Initialization*/
	uart2_cfg.BaudRate = 9600 ;
	MUSART_vInit(&uart2_cfg,USART2);
	MUSART_vInit(&uart1_cfg,USART1);

	/*6-Enable USARTx*/
	MUSART_vEnable(USART2);
	MUSART_vEnable(USART1);
	MUSART1_vSetCallBack(read_udr);
	NVIC_voidEnablePeripheral(37);

	MUSART_RxIntSetStatus(USART1, DISABLE);
	//data = MUSART_u8ReceiveByteSynchBlocking(USART1);
	//////////////////////////////////////////////////////////////////////////
	//LCD_Init();


	while(1)
	{

		u8 flag = MUSART_u8ReceiveByteAsynch_(USART1 , &status);
		if(status == REGISTER_MODE && flag == 1)
		{
			MSTK_vSetBusyWait(DELAY);
			while(read2_sucess != TRUE)
			{
				MSTK_vSetBusyWait(DELAY);
				read1_sucess=read_finger_1();
				if(read1_sucess == TRUE)
				{

					MSTK_vSetBusyWait(MODULE_DELAY);
					read2_sucess=read_finger_2();
					if(read2_sucess == TRUE)
					{
						make_template();
						MSTK_vSetBusyWait(MODULE_DELAY);
						store(1,id);
						MUSART_vTransmitByteSynch(USART1,'S');
						id++;
						read2_sucess = FALSE;
						read1_sucess = FALSE;

						status = IDLE_MODE;

						break;
					}
					else
					{

						MSTK_vSetBusyWait(1000);
						MUSART_vTransmitByteSynch(USART1,'G');

					}
				}
				else
				{
					MSTK_vSetBusyWait(1000);

					MUSART_vTransmitByteSynch(USART1,'G');
				}
			}

		}
		else if(status == SEARCH_MODE && flag == 1)
		{

			MSTK_vSetBusyWait(DELAY);
			while(read1_sucess != TRUE)
			{
				read1_sucess=read_finger_1();
				if(read1_sucess == TRUE)
				{
					MSTK_vSetBusyWait(MODULE_DELAY);
					make_template();
					MSTK_vSetBusyWait(MODULE_DELAY);
					if( check_finger() == TRUE)
					{
						MUSART_vTransmitByteSynch(USART1,'S');
						GPIO_voidSetPinValue(Engine.Port , Engine.Pin , GPIO_HIGH);
						MSTK_vSetBusyWait(MODULE_DELAY);
						status = IDLE_MODE;
						NVIC_voidEnablePeripheral(37);
						status = IDLE_MODE;

						read1_sucess = FALSE;

						break;
					}
					else
					{

						MUSART_vTransmitByteSynch(USART1,'F');
						MSTK_vSetBusyWait(MODULE_DELAY);
						//NVIC_voidEnablePeripheral(37);
						status = IDLE_MODE;

						read1_sucess = FALSE;
						break;

					}
				}
			}
		}
		else if(status == IDLE_MODE)
		{
			//NVIC_voidEnablePeripheral(37);

		}




	}
	return 0;
}
