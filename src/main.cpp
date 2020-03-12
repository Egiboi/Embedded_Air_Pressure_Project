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

//#include "ModbusMaster/ModbusMaster.h"
//#include "ModbusMaster/ModbusRegister.h"
//#include "ModbusMaster/LpcUart.h"
#include "BackEnd/BackEnd.h"

#include "DigitalIoPinMaster_class/DigitalIoPin.h"
//#include "i2c_class/I2C.h"
#include "GUI_class/SimpleMenu.h"
#include "GUI_class/LiquidCrystal.h"
#include "GUI_class/IntegerEdit.h"
#include "GUI_class/DecimalEdit.h"
#include <atomic>


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


static volatile std::atomic_int counter, counter1, counter2;
static volatile std::atomic_bool back, bool1, bool2, bool3;
#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Handle interrupt from SysTick timer
 * @return Nothing
 */



void SysTick_Handler(void)
{
	systicks++;
	if(counter < 5000) counter++;
	if (counter1 > 0) counter1--;
	else if(counter == 5000&&back==FALSE){
		counter=0;
		counter2 =0;
		back = TRUE;
	}

}
void Sleep(int ms)
{
	counter1 = ms;
	while(counter1 > 0) {
		__WFI();
	}
}

void PIN_INT0_IRQHandler(void){
	if(bool1==FALSE){
		bool1=TRUE;
		counter=0;
	}
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(0));
}
void PIN_INT1_IRQHandler(void){
	if(bool2==FALSE){
		bool2=TRUE;
		counter2++;
		counter=0;
	}
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(1));
}
void PIN_INT2_IRQHandler(void){
	if(bool3==FALSE){
		bool3=TRUE;
		counter=0;
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




	//interrupt and button define
	Chip_PININT_Init(LPC_GPIO_PIN_INT);

	/* Set pin back to GPIO (on some boards may have been changed to something
		   else by Board_Init()) */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 17,(IOCON_MODE_PULLUP |IOCON_DIGMODE_EN | IOCON_MODE_INACT) );
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 11,(IOCON_MODE_PULLUP |IOCON_DIGMODE_EN | IOCON_MODE_INACT) );
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 9,(IOCON_MODE_PULLUP |IOCON_DIGMODE_EN | IOCON_MODE_INACT) );
	/* Configure GPIO pin as input */
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, 16);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 1, 3);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, 0);

	/* Enable PININT clock */
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_PININT);

	/* Reset the PININT block */
	Chip_SYSCTL_PeriphReset(RESET_PININT);

	/* Configure interrupt channel for the GPIO pin in INMUX block */
	Chip_INMUX_PinIntSel(0, 0, 16);
	Chip_INMUX_PinIntSel(1, 1, 3);
	Chip_INMUX_PinIntSel(2, 0, 0);

	/* Configure channel interrupt as edge sensitive and falling edge interrupt */
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(0));
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(1));
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(2));
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(0));
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(1));
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(2));
	Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH(0));
	Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH(1));
	Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH(2));
	NVIC_ClearPendingIRQ(PIN_INT0_IRQn);
	NVIC_EnableIRQ(PIN_INT0_IRQn);
	NVIC_ClearPendingIRQ(PIN_INT1_IRQn);
	NVIC_EnableIRQ(PIN_INT1_IRQn);
	NVIC_ClearPendingIRQ(PIN_INT2_IRQn);
	NVIC_EnableIRQ(PIN_INT2_IRQn);

	SimpleMenu menu;
	menuStatic= &menu;

	IntegerEdit *Auto = new DecimalEdit(lcd, std::string("Automatic"), 0,120,10);
	IntegerEdit *Manu = new DecimalEdit(lcd, std::string("Manual"),0,100,5);

	menu.addItem(new MenuItem(Auto));
	menu.addItem(new MenuItem(Manu));
	Auto->setValue(60);
	Manu->setValue(50);

	DigitalIoPin sw1(0,16,true,true,true);
	DigitalIoPin sw2(1,3,true,true,true);
	DigitalIoPin sw3(0,0,true,true,true);

	/* Set up SWO to PIO1_2 */
	Chip_SWM_MovablePortPinAssign(SWM_SWO_O, 1, 2); // Needed for SWO printf

	/* Enable and setup SysTick Timer at a periodic rate */
	SysTick_Config(SystemCoreClock / 1000);

	counter2=0;
	menu.event(MenuItem::show); // display first menu item
	back=FALSE;
	counter = 0;

	BackEnd interface;

	while(1){
		//setFrequency(node, fa[10]);
		interface.setFrequency(21);
		interface.readPressureSensor();
		printf("%d\n", (int)interface.getPressureSensor());

		if(bool1){
			menuStatic->event(MenuItem::up);
			while(sw1.read());
			NVIC_ClearPendingIRQ(PIN_INT0_IRQn);
			bool1=FALSE;
		}

		else if(bool2){
			menuStatic->event(MenuItem::ok);
			while(sw2.read());
			NVIC_ClearPendingIRQ(PIN_INT1_IRQn);
			bool2=FALSE;
			if(counter2>=2){
				menuStatic->print();
				counter2=0;
			}

		}
		else if(bool3){
			menuStatic->event(MenuItem::down);
			while(sw3.read());
			NVIC_ClearPendingIRQ(PIN_INT2_IRQn);
			bool3=FALSE;
		}else if(back==TRUE){
			menuStatic->event(MenuItem::back);
			back=FALSE;
		}

	}

	//Board_LED_Set(0, false);
	//Board_LED_Set(1, true);
	//printf("Started\n"); // goes to ITM console if retarget_itm.c is included
	//dbgu.write("Hello, world\n");

	//abbModbusTest();
	//modbusTest();
	return 1;
}
