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

#include "ModbusMaster/ModbusMaster.h"
#include "ModbusMaster/ModbusRegister.h"
#include "ModbusMaster/LpcUart.h"

#include "DigitalIoPinMaster_class/DigitalIoPin.h"
#include "i2c_class/I2C.h"
#include "GUI_class/SimpleMenu.h"
#include "GUI_class/LiquidCrystal.h"
#include "GUI_class/IntegerEdit.h"
#include "GUI_class/DecimalEdit.h"
#include <atomic>

/** 7-bit I2C addresses of  sensor (data sheet page 4/10) */
#define I2C_PRES_ADDR  (0x40)




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

bool readPressureSensor(){
    //Config I2C
    I2C_config conf;
    I2C presSens(conf);
    //Initalize data container
    uint8_t data[3]={};
    //Command to read device through I2C
    uint8_t commandRead = 0xF1;
    //Write 1 byte to I2C, with commandRead
    presSens.write(I2C_PRES_ADDR, &commandRead, 1);
    //Read 3 bytes of reply, 2 data, 1 CRC
    presSens.read(I2C_PRES_ADDR, data, 3);
    //Combine the 2 data bytes to one 16bit int
    int16_t dataCombined= data[0];
    dataCombined = dataCombined << 8;
    dataCombined |= data[1];
    //Boolean for good check;
    bool CRCCheck = false;
    //Generator polynomial for CRC-8 check (data sheet 5/10)
    uint16_t CRC = 0x131;
    //Combine all 3 bytes to one 32 bit int
    int32_t CRCdata = 0x69b3;
    CRCdata = CRCdata << 8;
    CRCdata |= 0xf2;
    //loop lenght 3 bytes
    int k = 16 + 8;

    while (k != 0) {
        if ((CRCdata & (1 << k)) >> k == 1) {
            int tempk = k;
            for (int n = 8; n >= 0; n--) {
                //CRCdata bit
                int bit1 = (CRCdata & (1 << tempk)) >> tempk;
                //Generator Polynomial bit
                int bit2 = (CRC & (1 << n)) >> n;

                // Set bit with XOR bitwise if bits not 0 and 0
                if (!(bit1 == 0 && bit2 == 0)) {
                    CRCdata = ((1<<tempk) ^ CRCdata);
                }
                tempk--;
            }
        }
        k--;

    }
    if (CRCdata == 0) {
        CRCCheck = true;
        //altitude fix
        dataCombined = dataCombined *0.95/240;
        printf("%d\n", (int)dataCombined);
    }
    else {
        CRCCheck = false;
        printf("Read fail ");
        printf("value: %x dataCombine: %d\n CRC leftover: %d\n", data[2], dataCombined*0,95/240, (int)CRCdata);
    }

    return CRCCheck;

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


#if 0
void printRegister(ModbusMaster& node, uint16_t reg)
{
	uint8_t result;
	// slave: read 16-bit registers starting at reg to RX buffer
	result = node.readHoldingRegisters(reg, 1);

	// do something with data if read is successful
	if (result == node.ku8MBSuccess)
	{
		printf("R%d=%04X\n", reg, node.getResponseBuffer(0));
	}
	else {
		printf("R%d=???\n", reg);
	}
}

bool setFrequency(ModbusMaster& node, uint16_t freq)
{
	uint8_t result;
	int ctr;
	bool atSetpoint;
	const int delay = 500;

	node.writeSingleRegister(1, freq); // set motor frequency

	printf("Set freq = %d\n", freq/40); // for debugging

	// wait until we reach set point or timeout occurs
	ctr = 0;
	atSetpoint = false;
	do {
		Sleep(delay);
		// read status word
		result = node.readHoldingRegisters(3, 1);
		// check if we are at setpoint
		if (result == node.ku8MBSuccess) {
			if(node.getResponseBuffer(0) & 0x0100) atSetpoint = true;
		}
		ctr++;
	} while(ctr < 20 && !atSetpoint);

	printf("Elapsed: %d\n", ctr * delay); // for debugging

	return atSetpoint;
}


void abbModbusTest()
{
	ModbusMaster node(2); // Create modbus object that connects to slave id 2
	node.begin(9600); // set transmission rate - other parameters are set inside the object and can't be changed here

	printRegister(node, 3); // for debugging

	node.writeSingleRegister(0, 0x0406); // prepare for starting

	printRegister(node, 3); // for debugging

	Sleep(1000); // give converter some time to set up
	// note: we should have a startup state machine that check converter status and acts per current status
	//       but we take the easy way out and just wait a while and hope that everything goes well

	printRegister(node, 3); // for debugging

	node.writeSingleRegister(0, 0x047F); // set drive to start mode

	printRegister(node, 3); // for debugging

	Sleep(1000); // give converter some time to set up
	// note: we should have a startup state machine that check converter status and acts per current status
	//       but we take the easy way out and just wait a while and hope that everything goes well

	printRegister(node, 3); // for debugging

	int i = 0;
	int j = 0;
	const uint16_t fa[20] = { 1000, 2000, 3000, 3500, 4000, 5000, 7000, 8000, 10000, 15000, 20000, 9000, 8000, 7000, 6000, 5000, 4000, 3000, 2000, 1000 };

	while (1) {
		uint8_t result;

		// slave: read (2) 16-bit registers starting at register 102 to RX buffer
		j = 0;
		do {
			result = node.readHoldingRegisters(102, 2);
			j++;
		} while(j < 3 && result != node.ku8MBSuccess);
		// note: sometimes we don't succeed on first read so we try up to three times
		// if read is successful print frequency and current (scaled values)
		if (result == node.ku8MBSuccess) {
			printf("F=%4d, I=%4d  (ctr=%d)\n", node.getResponseBuffer(0), node.getResponseBuffer(1),j);
		}
		else {
			printf("ctr=%d\n",j);
		}

		Sleep(3000);
		i++;
		if(i >= 20) {
			i=0;
		}
		// frequency is scaled:
		// 20000 = 50 Hz, 0 = 0 Hz, linear scale 400 units/Hz
		setFrequency(node, fa[i]);
	}
}
#else
bool setFrequency(ModbusMaster& node, uint16_t freq)
{
	int result;
	int ctr;
	bool atSetpoint;
	const int delay = 500;

	ModbusRegister Frequency(&node, 1); // reference 1
	ModbusRegister StatusWord(&node, 3);

	Frequency = freq; // set motor frequency

	printf("Set freq = %d\n", freq/40); // for debugging

	// wait until we reach set point or timeout occurs
	ctr = 0;
	atSetpoint = false;
	do {
		Sleep(delay);
		// read status word
		result = StatusWord;
		// check if we are at setpoint
		if (result >= 0 && (result & 0x0100)) atSetpoint = true;
		ctr++;
	} while(ctr < 20 && !atSetpoint);

	printf("Elapsed: %d\n", ctr * delay); // for debugging

	return atSetpoint;
}


void abbModbusTest()
{
	ModbusMaster node(2); // Create modbus object that connects to slave id 2
	node.begin(9600); // set transmission rate - other parameters are set inside the object and can't be changed here

	ModbusRegister ControlWord(&node, 0);
	ModbusRegister StatusWord(&node, 3);
	ModbusRegister OutputFrequency(&node, 102);
	ModbusRegister Current(&node, 103);


	// need to use explicit conversion since printf's variable argument doesn't automatically convert this to an integer
	printf("Status=%04X\n", (int)StatusWord); // for debugging

	ControlWord = 0x0406; // prepare for starting

	printf("Status=%04X\n", (int)StatusWord); // for debugging

	Sleep(1000); // give converter some time to set up
	// note: we should have a startup state machine that check converter status and acts per current status
	//       but we take the easy way out and just wait a while and hope that everything goes well

	printf("Status=%04X\n", (int)StatusWord); // for debugging

	ControlWord = 0x047F; // set drive to start mode

	printf("Status=%04X\n", (int)StatusWord); // for debugging

	Sleep(1000); // give converter some time to set up
	// note: we should have a startup state machine that check converter status and acts per current status
	//       but we take the easy way out and just wait a while and hope that everything goes well

	printf("Status=%04X\n", (int)StatusWord); // for debugging

	int i = 0;
	const uint16_t fa[20] = { 1000, 2000, 3000, 3500, 4000, 5000, 7000, 8000, 10000, 15000, 20000, 9000, 8000, 7000, 6000, 5000, 4000, 3000, 2000, 1000 };

	while (1) {

		// just print the value without checking if we got a -1
		printf("F=%4d, I=%4d\n", (int) OutputFrequency, (int) Current);
		readPressureSensor();

		Sleep(3000);
		i++;
		if(i >= 20) {
			i=0;
		}
		// frequency is scaled:
		// 20000 = 50 Hz, 0 = 0 Hz, linear scale 400 units/Hz
		//setFrequency(node, fa[i]);
		setFrequency(node, fa[10]);
	}
}
#endif


void modbusTest()
{
	ModbusMaster node(2); // Create modbus object that connects to slave id 2


	while (1) {
		static uint32_t i;
		uint8_t j, result;
		uint16_t data[6];

		for(j = 0; j < 6; j++) {
			i++;
			// set word(j) of TX buffer to least-significant word of counter (bits 15..0)
			node.setTransmitBuffer(j, i & 0xFFFF);
		}
		// slave: write TX buffer to (6) 16-bit registers starting at register 0
		result = node.writeMultipleRegisters(0, j);

		// slave: read (6) 16-bit registers starting at register 2 to RX buffer
		result = node.readHoldingRegisters(2, 6);

		// do something with data if read is successful
		if (result == node.ku8MBSuccess)
		{
			for (j = 0; j < 6; j++)
			{
				data[j] = node.getResponseBuffer(j);
			}
			printf("%6d, %6d, %6d, %6d, %6d, %6d\n", data[0], data[1], data[2], data[3], data[4], data[5]);
		}
		Sleep(1000);
	}
}




/**
 * @brief	Main UART program body
 * @return	Always returns 1
 */
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
    IntegerEdit *Manu = new DecimalEdit(lcd, std::string("Manual"),0,100,10);

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
    counter=0;





    //this goes behind the BackEnd
	ModbusMaster node(2); // Create modbus object that connects to slave id 2
	node.begin(9600); // set transmission rate - other parameters are set inside the object and can't be changed here

	ModbusRegister ControlWord(&node, 0);
	ModbusRegister StatusWord(&node, 3);
	ModbusRegister OutputFrequency(&node, 102);
	ModbusRegister Current(&node, 103);


	// need to use explicit conversion since printf's variable argument doesn't automatically convert this to an integer
	printf("Status=%04X\n", (int)StatusWord); // for debugging

	ControlWord = 0x0406; // prepare for starting

	printf("Status=%04X\n", (int)StatusWord); // for debugging

	Sleep(1000); // give converter some time to set up
	// note: we should have a startup state machine that check converter status and acts per current status
	//       but we take the easy way out and just wait a while and hope that everything goes well

	printf("Status=%04X\n", (int)StatusWord); // for debugging

	ControlWord = 0x047F; // set drive to start mode

	printf("Status=%04X\n", (int)StatusWord); // for debugging

	Sleep(1000); // give converter some time to set up
	// note: we should have a startup state machine that check converter status and acts per current status
	//       but we take the easy way out and just wait a while and hope that everything goes well

	printf("Status=%04X\n", (int)StatusWord); // for debugging

	const uint16_t fa[20] = { 1000, 2000, 3000, 3500, 4000, 5000, 7000, 8000, 10000, 15000, 20000, 9000, 8000, 7000, 6000, 5000, 4000, 3000, 2000, 1000 };



	while(1){
		setFrequency(node, fa[10]);
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
