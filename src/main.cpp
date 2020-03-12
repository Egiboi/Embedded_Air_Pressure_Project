/*
===============================================================================
 Name        : main.c
 Author      : $(author)
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


/** 7-bit I2C addresses of  sensor (data sheet page 4/10) */
#define I2C_PRES_ADDR  (0x40)



#include <cr_section_macros.h>

// TODO: insert other include files here

// TODO: insert other definitions and declarations here

#include <cstring>
#include <cstdio>
#include <atomic>


#include "BackEnd/BackEnd.h"
#include "FrontEnd/FrontEnd.h"
#include "DigitalIoPinMaster_class/DigitalIoPin.h"
#include "GUI_class/SimpleMenu.h"
#include "GUI_class/LiquidCrystal.h"
#include "GUI_class/IntegerEdit.h"
#include "GUI_class/DecimalEdit.h"


/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/


/*****************************************************************************
 * Public functions
 ****************************************************************************/
static volatile uint32_t systicks;



/* this function is required by the modbus library */
uint32_t millis() {
	return systicks;
}


static SimpleMenu *menuStatic;


static volatile std::atomic_int counterBack, counterSleep, counterChangedValue, counterDefaultRunScreen;
static volatile std::atomic_bool back, bool1, bool2, bool3;
#ifdef __cplusplus
extern "C" {
#endif
/**
 *
 * @brief Handle interrupt from SysTick timer
 * @return Nothing
 */



void SysTick_Handler(void)
{
	systicks++;
	if(counterDefaultRunScreen<30000)counterDefaultRunScreen++;
	if(counterBack < 5000) counterBack++;
	if (counterSleep > 0) counterSleep--;
	else if(counterBack == 5000&&back==FALSE){
		counterBack=0;
		counterChangedValue =0;
		back = TRUE;
	}

}
void Sleep(int ms)
{
	counterSleep = ms;
	while(counterSleep > 0) {
		__WFI();
	}
}

void PIN_INT0_IRQHandler(void){
	if(bool1==FALSE){
		bool1=TRUE;
		counterBack=0;
	}

	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(0));
}
void PIN_INT1_IRQHandler(void){
	if(bool2==FALSE){
		bool2=TRUE;
		counterChangedValue++;
		counterBack=0;
	}

	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(1));
}
void PIN_INT2_IRQHandler(void){
	if(bool3==FALSE){
		bool3=TRUE;
		counterBack=0;
	}
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(2));
}

#ifdef __cplusplus
}
#endif


int main(void)
{

#if defined (__USE_LPCOPEN)
	// Read clock settings and update SystemCoreClock variable
	SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
	// Set up and initialize all required blocks and
	// functions related to the board hardware
	Board_Init();
	Chip_RIT_Init(LPC_RITIMER);
	// Set the LED to the state of "On"
	Board_LED_Set(0, true);
#endif
#endif

	//LCD defined
	DigitalIoPin d7(0,7,FALSE), d6(0,6,FALSE),d5(0,5,FALSE), d4(1,8,FALSE), en(1,6,FALSE), rs(0,8,FALSE);
	LiquidCrystal *lcd = new LiquidCrystal(&rs, &en, &d4, &d5, &d6, &d7);
	lcd->begin(16,2);
	lcd->setCursor(0,0);

	//Uart define
	LpcPinMap none = {-1, -1}; // unused pin has negative values in it
	LpcPinMap txpin = { 0, 18 }; // transmit pin that goes to debugger's UART->USB converter
	LpcPinMap rxpin = { 0, 13 }; // receive pin that goes to debugger's UART->USB converter
	LpcUartConfig cfg = { LPC_USART0, 115200, UART_CFG_DATALEN_8 | UART_CFG_PARITY_NONE | UART_CFG_STOPLEN_1, false, txpin, rxpin, none, none };
	LpcUart dbgu(cfg);




	SimpleMenu menu;
	menuStatic= &menu;

	IntegerEdit *Auto = new DecimalEdit(lcd, std::string("Automatic"), 0,120,10);
	IntegerEdit *Manu = new DecimalEdit(lcd, std::string("Manual"),0,100,5);

	menu.addItem(new MenuItem(Auto));
	menu.addItem(new MenuItem(Manu));
	Auto->setValue(60);
	Manu->setValue(50);



	/* Set up SWO to PIO1_2 */
	Chip_SWM_MovablePortPinAssign(SWM_SWO_O, 1, 2); // Needed for SWO printf

	/* Enable and setup SysTick Timer at a periodic rate */
	SysTick_Config(SystemCoreClock / 1000);

	counterChangedValue=0;
	//menu.event(MenuItem::show); // display first menu item
	//go to default runScreen first
	counterDefaultRunScreen=30000;
	back=FALSE;
	counterBack = 0;

	BackEnd interface;
	FrontEnd frontend;

	/* Set pin back to GPIO (on some boards may have been changed to something else by Board_Init()) */
	Chip_PININT_Init(LPC_GPIO_PIN_INT);
	//Prepping pins for interrupting definitions
	interface.prepPinForSet(0, 16);
	interface.prepPinForSet(1, 3);
	interface.prepPinForSet(0, 0);
	/* Enable PININT clock */
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_PININT);
	/* Reset the PININT block */
	Chip_SYSCTL_PeriphReset(RESET_PININT);
	//Setting pins for interrupting
	interface.setPinInterrupt(0, 16, 0);
	interface.setPinInterrupt(1, 3, 1);
	interface.setPinInterrupt(0, 0, 2);


	bool1=false, bool2=false, bool3=false;

	uint16_t i = 0;
	i = (uint16_t) Manu -> getValue() / 5; //manual mode

	while(1) {
		interface.setFrequency(frontend.defaultRun(interface.readPressureSensor(), i));
		interface.readPressureSensor();
		printf("Fan speed is: %d\n", (int)i);
		printf("Pressure level is: %d\n", (int)interface.getPressureSensor());

		if(bool1){
			menuStatic->event(MenuItem::up);
			while(!Chip_GPIO_GetPinState(LPC_GPIO, 0, 16)){
			}
			counterDefaultRunScreen=0;
			Sleep(100);
			bool1=FALSE;
		}

		else if(bool2){
			bool tempbool1=Auto -> getFocus();
			bool tempbool2=Manu -> getFocus();
			menuStatic->event(MenuItem::ok);
			counterDefaultRunScreen=0;
			Sleep(100);
			if(counterChangedValue>=2){

				if (tempbool1) {
					frontend.setPressureTarget((uint16_t) Auto -> getValue()); //sets pressuretarget value
					frontend.setMode(1);
				}

				else if (tempbool2) {
					frontend.setMode(2);
					i = (uint16_t) Manu -> getValue() / 5;
				}


				i = (uint16_t) Manu -> getValue() / 5;
				menuStatic->print();
				counterChangedValue=0;
				counterDefaultRunScreen=30000;
			}
			while(!Chip_GPIO_GetPinState(LPC_GPIO, 1, 3)){
			}
			bool2=FALSE;

		}
		else if(bool3){
			menuStatic->event(MenuItem::down);
			while(!Chip_GPIO_GetPinState(LPC_GPIO, 0, 0));
			counterDefaultRunScreen=0;
			Sleep(100);
			bool3=FALSE;
		}else if(back==TRUE){
			menuStatic->event(MenuItem::back);
			back=FALSE;
		}
		if(counterDefaultRunScreen==30000){
			frontend.defaultDisplay(lcd, (int)i, (int)interface.getPressureSensor());
			counterDefaultRunScreen=27000;
			counterBack=0;
		}

		Sleep(50);

	}

	return 1;
}
