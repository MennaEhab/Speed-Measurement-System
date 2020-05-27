/*
 * SPI__.c
 *
 * Created: 2/10/2020 6:54:24 PM
 *  Author: MENA
 */ 

#include "SPI__.h"
#include "timers.h"

extern void _SPIInitMaster(SPI_clk clockSPI ,SPI_mode phasePolarityMode,SPI_DataOrder dataorder ){
		//// clear status register
		SPSR = ZERO ;
		
		/// clear control mode
		
		SPCR = ZERO ;
		
		// master or slave bit 4 
		SPCR |= Master_SPI ; 
		
		//set clock
		 
		SPCR|=clockSPI ;
		
		/// data order update
		
		SPCR|=dataorder ;
		
		// set phase bit2 and polarity bit3
		
		 SPCR |= phasePolarityMode;
		
		/* set interrupt  */
		SPCR |= INTERRUPT_SPI ;
		 
////// GPIO set output and input 
 // ss bit4
gpioPinDirection(SS_PORT ,SS_PIN,OUTPUT) ;

//MOSI bit5

gpioPinDirection(MOSI_PORT ,MOSI_PIN,OUTPUT) ;

//MISO bit6

gpioPinDirection(MISO_PORT ,MISO_PIN,INPUT) ;

//SCK bit 7

gpioPinDirection(SCK_PORT ,SCK_PIN,OUTPUT) ;
		
	/// enable SPI bit 6
	SPCR |= Enable_SPI ;
	
}

extern void _SPIInitSlave(SPI_clk clockSPI,SPI_mode phasePolarityMode,SPI_DataOrder dataorder  ){
	//// clear status register
	SPSR = ZERO ;
	
	/// clear control mode
	
	SPCR = ZERO ;
	
	
	// master or slave bit 4
	SPCR |= Slave_SPI ;
	
	/// data order update
	
	SPCR|=dataorder ;
	
	//set clock
	
	SPCR|=clockSPI ;
	
	// set phase and polarity
	SPCR |=phasePolarityMode ;
	
	/* set interrupt  */
	SPCR |= INTERRUPT_SPI ;
	
	////// GPIO set output and input
 // ss bit4
 gpioPinDirection(SS_PORT ,SS_PIN,INPUT) ;

 //MOSI bit5

 gpioPinDirection(MOSI_PORT ,MOSI_PIN,INPUT) ;

 //MISO bit6

 gpioPinDirection(MISO_PORT ,MISO_PIN,OUTPUT) ;

 //SCK bit 7

 gpioPinDirection(SCK_PORT ,SCK_PIN,INPUT) ;
	
	/// enable SPI bit 6
	SPCR |= Enable_SPI ;
	
}

extern void _SPITrancevier(uint8_t* data){
	
	if(SPCR&Master_SPI){
		
		/*Select the slave By pull down the SS pin */
		
		gpioPinWrite(GPIOB,BIT4,0);
		
		timer0DelayMs(1);

	}
	
	SPDR = *data;

	while (!(SPSR&(1<<7)));

	*data = SPDR;
	
}



