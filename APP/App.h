#ifndef APP_H_
#define APP_H_
#include "timers.h"
#include "pushButton.h"
#include "pushButtonConfig.h"
#include "interrupts.h"
#include "_uart.h"
#include "SPI__.h"
#include "led.h"
#include "ledConfig.h"

extern void Start (void) ;
extern void Slave (void) ;
extern void Master (void) ;
extern void Stop(void);
extern void CalcTime(float* time);



#endif /* APP_H_ */