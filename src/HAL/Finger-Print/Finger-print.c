#include "../../LIB/BIT_MATH.h"
#include "../../LIB/STD_TYPES.h"
#include "../../MCAL/UART/UART_prv.h"
#include "../../MCAL/UART/UART_int.h"
#include "../../MCAL/GPIO/GPIO_Interface.h"
//#include <string.h>
//#include<stdbool.h>
u8 arr[100] = {0};
u8 arr2[100] = {0};

u8 arr3[100] = {0};



u8 read_finger_1(void)          //for char_buffer1
{
	int i=0;
	unsigned char  k=1,ch=1;
	MUSART_vTransmitByteSynch(USART2,0xef);
	MUSART_vTransmitByteSynch(USART2,0x01);
	MUSART_vTransmitByteSynch(USART2,0xff);
	MUSART_vTransmitByteSynch(USART2,0xff);
	MUSART_vTransmitByteSynch(USART2,0xff);
	MUSART_vTransmitByteSynch(USART2,0xff);
	MUSART_vTransmitByteSynch(USART2,0x01);
	MUSART_vTransmitByteSynch(USART2,0x00);
	MUSART_vTransmitByteSynch(USART2,0x03);
	MUSART_vTransmitByteSynch(USART2,0x01);
	MUSART_vTransmitByteSynch(USART2,0x00);
	MUSART_vTransmitByteSynch(USART2,0x05);


	for(i=0;i<10;i++)//0-9
	{
		k=MUSART_u8ReceiveByteSynchNonBlocking(USART2);//0-9 //10
		arr[i] = k;
		if(i==9)//10
		{
			ch=k;
			k=MUSART_u8ReceiveByteSynchNonBlocking(USART2);//10
			k=MUSART_u8ReceiveByteSynchNonBlocking(USART2);
			if(ch==0x00||ch==0x21)
			{
				/*generate character file from image char file img2tz #1*/


				MUSART_vTransmitByteSynch(USART2,0xef);
				MUSART_vTransmitByteSynch(USART2,0x01);
				MUSART_vTransmitByteSynch(USART2,0xff);
				MUSART_vTransmitByteSynch(USART2,0xff);
				MUSART_vTransmitByteSynch(USART2,0xff);
				MUSART_vTransmitByteSynch(USART2,0xff);
				MUSART_vTransmitByteSynch(USART2,0x01);
				MUSART_vTransmitByteSynch(USART2,0x00);
				MUSART_vTransmitByteSynch(USART2,0x04);
				MUSART_vTransmitByteSynch(USART2,0x02);
				MUSART_vTransmitByteSynch(USART2,0x01);
				MUSART_vTransmitByteSynch(USART2,0x00);
				MUSART_vTransmitByteSynch(USART2,0x08);

				i=0;
				for(i=0;i<10;i++)
				{
					k=MUSART_u8ReceiveByteSynchNonBlocking(USART2);
					if(i==9)
					{
						ch=k;
						k=MUSART_u8ReceiveByteSynchNonBlocking(USART2);
						k=MUSART_u8ReceiveByteSynchNonBlocking(USART2);
						if(ch==0x00||ch==0x21)
						{

							return 1;
						}
					}
				}
			}
		}
	}
	return 0;
}
u8 read_finger_2(void)          //for char_buffer2
{
	int i=0;
	char k=1,ch=1;

	/***** To collect finger image GenImg *****/
	MUSART_vTransmitByteSynch(USART2,0xef);
	MUSART_vTransmitByteSynch(USART2,0x01);
	MUSART_vTransmitByteSynch(USART2,0xff);
	MUSART_vTransmitByteSynch(USART2,0xff);
	MUSART_vTransmitByteSynch(USART2,0xff);
	MUSART_vTransmitByteSynch(USART2,0xff);
	MUSART_vTransmitByteSynch(USART2,0x01);
	MUSART_vTransmitByteSynch(USART2,0x00);
	MUSART_vTransmitByteSynch(USART2,0x03);
	MUSART_vTransmitByteSynch(USART2,0x01);
	MUSART_vTransmitByteSynch(USART2,0x00);
	MUSART_vTransmitByteSynch(USART2,0x05);
	for(i=0;i<10;i++)
	{
		k=MUSART_u8ReceiveByteSynchNonBlocking(USART2);
		arr2[i] = k;
		if(i==9)
		{
			ch=k;
			k=MUSART_u8ReceiveByteSynchNonBlocking(USART2);
			k=MUSART_u8ReceiveByteSynchNonBlocking(USART2);
			if(ch==0x000||ch==0x21)
			{

				k=1;
				//	_delay_ms(500);//1000

				/*To generate character file from image Img2Tz char file #2*/
				MUSART_vTransmitByteSynch(USART2,0xef);
				MUSART_vTransmitByteSynch(USART2,0x01);
				MUSART_vTransmitByteSynch(USART2,0xff);
				MUSART_vTransmitByteSynch(USART2,0xff);
				MUSART_vTransmitByteSynch(USART2,0xff);
				MUSART_vTransmitByteSynch(USART2,0xff);
				MUSART_vTransmitByteSynch(USART2,0x01);
				MUSART_vTransmitByteSynch(USART2,0x00);
				MUSART_vTransmitByteSynch(USART2,0x04);
				MUSART_vTransmitByteSynch(USART2,0x02);
				MUSART_vTransmitByteSynch(USART2,0x02);
				MUSART_vTransmitByteSynch(USART2,0x00);
				MUSART_vTransmitByteSynch(USART2,0x09);
				i=0;
				for(i=0;i<10;i++)
				{
					k=MUSART_u8ReceiveByteSynchNonBlocking(USART2);
					if(i==9)
					{
						ch=k;
						k=MUSART_u8ReceiveByteSynchNonBlocking(USART2);
						k=MUSART_u8ReceiveByteSynchNonBlocking(USART2);
						if(ch==0x00||ch==0x21)
						{
							//_delay_ms(500);//1000
							return 1;
						}



					}

				}

			}

		}

	}
	return 0;

}
void make_template(void)
{

	int i=0;
	char k=1,ch=1;

	MUSART_vTransmitByteSynch(USART2,0xef);
	MUSART_vTransmitByteSynch(USART2,0x01);
	MUSART_vTransmitByteSynch(USART2,0xff);
	MUSART_vTransmitByteSynch(USART2,0xff);
	MUSART_vTransmitByteSynch(USART2,0xff);
	MUSART_vTransmitByteSynch(USART2,0xff);
	MUSART_vTransmitByteSynch(USART2,0x01);
	MUSART_vTransmitByteSynch(USART2,0x00);
	MUSART_vTransmitByteSynch(USART2,0x03);
	MUSART_vTransmitByteSynch(USART2,0x05);
	MUSART_vTransmitByteSynch(USART2,0x00);
	MUSART_vTransmitByteSynch(USART2,0x09);


	for(i=0;i<10;i++)
	{
		k=MUSART_u8ReceiveByteSynchNonBlocking(USART2);

		if(i==9)
		{
			ch=k;
			k=MUSART_u8ReceiveByteSynchNonBlocking(USART2);
			k=MUSART_u8ReceiveByteSynchNonBlocking(USART2);
		}
	}
}
void store(unsigned char charBuf ,unsigned int ID)
{
	unsigned char  i=0,sum=14+ID;
	unsigned char k=1,ch=1;
	MUSART_vTransmitByteSynch(USART2,239);
	MUSART_vTransmitByteSynch(USART2,1);
	MUSART_vTransmitByteSynch(USART2,255);
	MUSART_vTransmitByteSynch(USART2,255);
	MUSART_vTransmitByteSynch(USART2,255);
	MUSART_vTransmitByteSynch(USART2,255);
	MUSART_vTransmitByteSynch(USART2,1);
	MUSART_vTransmitByteSynch(USART2,0);
	MUSART_vTransmitByteSynch(USART2,6);
	MUSART_vTransmitByteSynch(USART2,6);
	MUSART_vTransmitByteSynch(USART2,charBuf);
	MUSART_vTransmitByteSynch(USART2,(unsigned char )(ID>>8));
	MUSART_vTransmitByteSynch(USART2,(unsigned char )ID);
	MUSART_vTransmitByteSynch(USART2,0);//C
	MUSART_vTransmitByteSynch(USART2,sum);//C
	for(i=0;i<10;i++)
	{
		k=MUSART_u8ReceiveByteSynchNonBlocking(USART2);
		if(i==9)
		{
			ch=k;
			k=MUSART_u8ReceiveByteSynchNonBlocking(USART2);
			k=MUSART_u8ReceiveByteSynchNonBlocking(USART2);
			if(ch==0x00||ch==0x21)
			{

			}


		}
	}
}
u8 matchScore1=0;
u8 matchScore2=0;

u8 pageNo1=0;
u8 pageNo2=0;
u8 check_finger(void)
{
	int i=0;
	u8 k=1,ch=1;


	MUSART_vTransmitByteSynch(USART2,0xef);
	MUSART_vTransmitByteSynch(USART2,0x01);
	MUSART_vTransmitByteSynch(USART2,0xff);
	MUSART_vTransmitByteSynch(USART2,0xff);
	MUSART_vTransmitByteSynch(USART2,0xff);
	MUSART_vTransmitByteSynch(USART2,0xff);
	MUSART_vTransmitByteSynch(USART2,0x01);
	MUSART_vTransmitByteSynch(USART2,0x00);
	MUSART_vTransmitByteSynch(USART2,0x08);
	MUSART_vTransmitByteSynch(USART2,0x04);
	MUSART_vTransmitByteSynch(USART2,0x01);
	MUSART_vTransmitByteSynch(USART2,0x00);
	MUSART_vTransmitByteSynch(USART2,0x0A);
	MUSART_vTransmitByteSynch(USART2,0x00);
	MUSART_vTransmitByteSynch(USART2,0x5A);
	MUSART_vTransmitByteSynch(USART2,0x00);
	MUSART_vTransmitByteSynch(USART2,0x72);

	for(i=0;i<10;i++)
	{
		k=MUSART_u8ReceiveByteSynchNonBlocking(USART2);
		arr3[i] = k;
		if(i==9)
		{
			ch=k;
			pageNo1=MUSART_u8ReceiveByteSynchNonBlocking(USART2);
			pageNo2=MUSART_u8ReceiveByteSynchNonBlocking(USART2);


			matchScore1=MUSART_u8ReceiveByteSynchNonBlocking(USART2);
			matchScore2=MUSART_u8ReceiveByteSynchNonBlocking(USART2);
			k=MUSART_u8ReceiveByteSynchNonBlocking(USART2);
			k=MUSART_u8ReceiveByteSynchNonBlocking(USART2);
			if(ch==0x21||ch==0x00)////page no
			{

				return 1;

			}
			else
			{

			}

		}

	}
	return 0;
}
