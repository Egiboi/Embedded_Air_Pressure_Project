/*
===============================================================================
 Name        : main.c
 Author      : Team 5
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

// TODO: insert other include files here
#include "DigitalIoPinMaster/DigitalIoPin.h"

#include "..\i2c_class\I2C.h"
#if defined(BOARD_NXP_LPCXPRESSO_1549)
/** 7-bit I2C addresses of  sensor (data sheet page 4/10) */
#define I2C_PRES_ADDR  (0x40)
#endif


bool readPressureSensor(){
	I2C_config conf;
	I2C presSens(conf);
	uint8_t data[3]={};
	uint8_t commandRead = 0xF1;
	presSens.write(I2C_PRES_ADDR, &commandRead, 1);
	presSens.read(I2C_PRES_ADDR, data, 3);
	int16_t dataCombined= data[0];
	dataCombined = dataCombined << 8;
	dataCombined |= data[1];
	dataCombined = dataCombined *0.95/240;
	printf("%d\n",dataCombined);
	printf("%d\n",(int)data[2]);
	return true;

}


// TODO: insert other definitions and declarations here




int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
    // Set the LED to the state of "On"
    Board_LED_Set(0, true);
#endif
#endif

    // TODO: insert code here




    // Force the counter to be placed into memory
    volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
        i++ ;

        // "Dummy" NOP to allow source level single
        // stepping of tight while() loop
        __asm volatile ("nop");
    }
    return 0 ;
}
