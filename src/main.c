#include "LIB/STD_TYPES.h"
#include "LIB/BIT_MATH.h"
#include "MCAL/RCC/RCC_interface.h"
#include "MCAL/GPIO/GPIO_Interface.h"
#include "MCAL/UART/UART_prv.h"
#include "MCAL/UART/UART_int.h"
#include "MCAL/STK/STK_int.h"
#include "HAL/Finger-Print/Finger-print.h"
#include "HAL/LCD/LCD.h"

extern USART_InitType uart2_cfg ;

#define FALSE           0
#define TRUE            1
#define REGISTER_MODE   1
#define SEARCH_MODE     0
#define MODULE_DELAY    1000000
#define DELAY           1000
u8 read1_sucess=0;
u8 read2_sucess=0;
u8 status = 0;

u8 id = 20;
extern u8 pageNo1;
extern u8 pageNo2;
int main(void){


	u8 search=1;
	RCC_voidInit();
	RCC_u8EnablePeripheralClock(RCC_APB1 ,RCC_EN_UART2 );
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


	GPIO_PinConfig  register_button = {
			.Port = GPIO_PORTB,
			.Pin = GPIO_PIN0,
			.Mode = GPIO_MODE_INPUT,
			.OutputSpeed = GPIO_OUTPUT_SPEED_LOW,
			.PullState  = GPIO_PULL_UP,

	};
	GPIO_PinConfig  search_button = {
			.Port = GPIO_PORTB,
			.Pin = GPIO_PIN1,
			.Mode = GPIO_MODE_INPUT,
			.OutputSpeed = GPIO_OUTPUT_SPEED_LOW,
			.PullState  = GPIO_PULL_UP,

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

	GPIO_voidInit(&register_button);
	GPIO_voidInit(&search_button);

	///////////////////////////////////////////////////////////////////////
	/*5-MUSART Initialization*/
	uart2_cfg.BaudRate = 9600 ;
	MUSART_vInit(&uart2_cfg,USART2);
	/*6-Enable USARTx*/
	MUSART_vEnable(USART2);
	//////////////////////////////////////////////////////////////////////////
	LCD_Init();


	while(1)
	{
		//_delay_ms(1000);

		if(GPIO_u8GetPinValue(register_button.Port,register_button.Pin) == GPIO_LOW )
		{
			while(GPIO_u8GetPinValue(register_button.Port,register_button.Pin) == GPIO_LOW);
			status = TRUE;
		}
		if(status == REGISTER_MODE)
		{

			LCD_GoTo(0,0);
			LCD_WriteString("Enter your finger");
			LCD_GoTo(1,0);
			LCD_WriteString("to register");
			MSTK_vSetBusyWait(DELAY);
			while(read2_sucess != TRUE)
			{
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
						LCD_Clear();
						LCD_GoTo(0,0);
						LCD_WriteString("Registered successfully!!");
						LCD_GoTo(1,0);
						LCD_WriteString("id[");
						LCD_WriteNumber(id);
						LCD_WriteString("]");
						MSTK_vSetBusyWait(2000000);
						id++;
						read2_sucess = FALSE;
						status = SEARCH_MODE;
						break;
					}
					else
					{
						LCD_Clear();
						LCD_GoTo(0,0);
						MSTK_vSetBusyWait(1000);
						LCD_WriteString("Enter again");

					}
				}
				else
				{

				}
			}

		}
		else if(status == SEARCH_MODE)
		{
			LCD_Clear();

			LCD_GoTo(0,0);
			LCD_WriteString("Enter your finger print");
			MSTK_vSetBusyWait(DELAY);
			read1_sucess=read_finger_1();
			if(read1_sucess == TRUE)
			{
				LCD_Clear();
				LCD_GoTo(0,0);
				LCD_WriteString("searching.....");
				MSTK_vSetBusyWait(MODULE_DELAY);
				make_template();
				MSTK_vSetBusyWait(MODULE_DELAY);
				if( check_finger() == TRUE)
				{
					LCD_Clear();
					LCD_GoTo(0,0);
					LCD_WriteString("welcome");
					LCD_GoTo(1,0);
					LCD_WriteString("id[");
					LCD_WriteNumber(pageNo2);
					LCD_WriteString("]");
					MSTK_vSetBusyWait(MODULE_DELAY);
					LCD_Clear();
				}
				else
				{
					LCD_Clear();
					LCD_GoTo(0,0);
					LCD_WriteString("not exist");
					MSTK_vSetBusyWait(MODULE_DELAY);

				}
			}
		}




	}
	return 0;
}
