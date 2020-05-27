/*
 * App.c
 *
 * Created: 2/12/2020 3:53:57 PM
 *  Author: MENA
 */ 

#include "App.h"
#define DEBOUND_TIME 0.25
#define  NO_OVF_FOR_DEBOUND_TIME 16 

volatile float g_Total_distance = 0 ; 
volatile uint16_t  g_no_OVF = 0 ;

uint8_t g_speed = 0 ;

void Slave (void){
	/* initiate started*/
	uint8_t startedFlag =FALSE ;
	
	/* init*/
	UART_Config uart_config ;
	
	uart_config.BaudRate = 9600;
	uart_config.DataByte =_8BIT_DATA;
	uart_config.InterruptStatus = _NO_POLLING;
	uart_config.Parity = _DIS_PARITY;
	uart_config.StopBits = _1_STOP_BIT;
	uart_config.Speed= _DOUBLE_SPEED;
	
	timer0Init(T0_NORMAL_MODE,T0_OC0_DIS,T0_PRESCALER_1024,102,0,T0_INTERRUPT_NORMAL);
	_SPIInitSlave(Fosc4 , mode1 ,MSB) ;
	_uartInit(uart_config);
	pushButtonInit(BTN_0);
	pushButtonInit(BTN_1);

	
	/* init end*/
	
	
	/* poll on buttom */
	
	while (1){
	if(pushButtonGetStatus(BTN_0) == Pressed) {
	
		startedFlag = TRUE ;
		Start() ;
		
	}/// end if button pressed
	
	
	if (startedFlag == TRUE)
	{ 
		// recieve speed continously 
		_SPITrancevier(&g_speed) ;
		timer2DelayMs(50);

		
	}
	if(pushButtonGetStatus(BTN_1) == Pressed){
		
		timer2DelayMs(200);
		if (startedFlag == TRUE)
		{
			Stop();
		}
		
	
		startedFlag = FALSE;
	}
	
	}// end while
	
}

void Start (void) {
	
	/* get speed  from Trancevier*/
	
	_SPITrancevier(&g_speed) ;
	
	  /*  start timer */
	  
		timer0Start();
	
	
}

void Stop(void){
	float time;
	timer0Stop();
	CalcTime(&time);
	
	
	
	g_Total_distance +=(time*g_speed);
	
		

	_uartTransmitStr("total distance : ");
	_uartTransmitInt(g_Total_distance);
	_uartTransmitChar('\n');
	
	//_uartTransmitStr("total time : ");
	//_uartTransmitInt(g_Total_distance/g_speed);
	//_uartTransmitChar('\n');
	//_uartTransmitStr("total speed : ");
	//_uartTransmitInt(g_speed);
	//_uartTransmitChar('\n');

	
	
	g_Total_distance=0;

	
}

void CalcTime(float* time){
	
	uint8_t counts;
	
	counts = timer0Read();
	
	
	*(time) = (counts+(_8_BIT_RESOLUTION*g_no_OVF)) * (1024.0/F_CPU);
	
	
	
}
void Master (void) {
	uint8_t speed ;
	uint8_t speed_send_SPI ;
	UART_Config uart_config ;
	
	
	
	uart_config.BaudRate = 9600;
	uart_config.DataByte =_8BIT_DATA;
	uart_config.InterruptStatus = _NO_POLLING;
	uart_config.Parity = _DIS_PARITY;
	uart_config.StopBits = _1_STOP_BIT;
	uart_config.Speed= _DOUBLE_SPEED;
	
	_uartInit(uart_config);
	_SPIInitMaster(Fosc4 , mode1 ,MSB) ;
	pushButtonInit(BTN_0);
	pushButtonInit(BTN_1);
	/*Led_Init(LED_0);
	Led_Init(LED_1);
	Led_Init(LED_3);*/
	// recieve sped from pc 1
	
	_uartRecieveChar(&speed);
	_uartTransmitStr("Speed= ");
	_uartTransmitChar(speed);
	_uartTransmitChar('\n');
	//if(speed== '1')
			
	g_speed = speed-'0';

while(1){
	timer2DelayMs(250);
		if(pushButtonGetStatus(BTN_0) == Pressed) {
		if(g_speed<9){
			g_speed++;
			speed++;
			
		}
		//Led_Off(LED_1);
		_uartTransmitStr("Speed= ");
		_uartTransmitChar(speed);
		_uartTransmitChar('\n');
	
	}

	if(pushButtonGetStatus(BTN_1) == Pressed) {
		if(g_speed>0){
			g_speed--;
			speed--;
		}
		
		_uartTransmitStr("Speed= ");
		_uartTransmitChar(speed);
		_uartTransmitChar('\n');
		//Led_On(LED_1);
	
	}
		
		speed_send_SPI = g_speed ;
		_SPITrancevier(&speed_send_SPI);

	}

}


_ISR__(TIMER0_OVF_vect){
	
	// timer stop 
	timer0Stop();
	/*increment g_no_OVF*/
	g_no_OVF ++;
	/* 
	 *update total distance if g_no_OVF = 16 
	 *add to TCNT 100
	 *start timer
	 * 
	 */
	if (g_no_OVF == NO_OVF_FOR_DEBOUND_TIME)
	{
		g_Total_distance += (g_speed*DEBOUND_TIME) ;
		g_no_OVF = ZERO ;
		timer0Set(102);
	}
	
	// timer start 
	
	timer0Start();

	
}
