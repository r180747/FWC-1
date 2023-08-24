/*==========================================================
Apr 9, 2021,
Released under GNU/GPL
https://www.gnu.org/licenses/gpl-3.0.en.html
/*==========================================================
 *
 *    File   : main.c
 *    Purpose: main for Pygmy LCD display
 *                                                          
 *=========================================================*/

#include "Fw_global_config.h"   // This defines application specific charactersitics
#include <avr/io.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "RtosTask.h"

/*    Include the generic headers required for QORC */
#include "eoss3_hal_gpio.h"
#include "eoss3_hal_rtc.h"
#include "eoss3_hal_timer.h"
#include "eoss3_hal_fpga_usbserial.h"
#include "ql_time.h"
#include "s3x_clock_hal.h"
#include "s3x_clock.h"
#include "s3x_pi.h"
#include "dbg_uart.h"

#include "cli.h"


extern const struct cli_cmd_entry my_main_menu[];


const char *SOFTWARE_VERSION_STR;


/*
 * Global variable definition
 */


extern void qf_hardwareSetup();
static void nvic_init(void);
#define GPIO_OUTPUT_MODE (1)
#define GPIO_INPUT_MODE (0)

typedef uint8_t byte; // changed the name

#define LCD_RS 10 // pin for LCD R/S 
#define LCD_E 8 // pin for LCD enable
#define DAT4 7 // pin for pygmy7
#define DAT5 6 // pin for pygmy6
#define DAT6 5 //pin for pygmy5
#define DAT7 4 //pin for pygmy4
//// The following defines are controller commands
#define CLEARDISPLAY 0x01
#define INC 0x04

void PyHal_GPIO_SetDir(uint8_t gpionum,uint8_t iomode);
int PyHal_GPIO_GetDir(uint8_t gpionum);
int PyHal_GPIO_Set(uint8_t gpionum, uint8_t gpioval);
int PyHal_GPIO_Get(uint8_t gpionum);
//declaring functions
void LCD_setup(void);
void PulseEnableLine ();
void SendNibble(byte data);
void SendByte (byte data);
void LCD_Cmd (byte cmd);
void LCD_Char (byte ch);
void LCD_Init();
void LCD_Clear() ;
void LCD_Message(const char *text) ;
void LCD_Integer(int data);



int main(void)
{
    uint32_t i=0,j=0,k=0;
    SOFTWARE_VERSION_STR = "qorc-onion-apps/qf_hello-fpga-gpio-ctlr";
    
    qf_hardwareSetup();
    nvic_init();

    dbg_str("\n\n");
    dbg_str( "##########################\n");
    dbg_str( "Quicklogic QuickFeather FPGA GPIO CONTROLLER EXAMPLE\n");
    dbg_str( "SW Version: ");
    dbg_str( SOFTWARE_VERSION_STR );
    dbg_str( "\n" );
    dbg_str( __DATE__ " " __TIME__ "\n" );
    dbg_str( "##########################\n\n");

    dbg_str( "\n\nHello GPIO!!\n\n");	// <<<<<<<<<<<<<<<<<<<<<  Change me!

    CLI_start_task( my_main_menu );
	HAL_Delay_Init();

       //lcd ready for display
    double inputNumber = 32.87; 
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Decimal:");
    lcd.setCursor(8,1);
    lcd.print(inputNumber);
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Binary: ");
    int intPart = int(inputNumber);
    float fractionalPart = inputNumber - intPart;
    lcd.setCursor(7,1);
    printBinary(intPart,fractionalPart);
    delay(2000);
    lcd.clear();
  while (intPart > 0)
    {
      binaryNum[i] = intPart % 2;
      intPart = intPart / 2;
      i++;
    }
 for (int j = i-1; j >= 0; j--)
    lcd.print(binaryNum[j]);
    lcd.print(".");
    
  for (int i = 0; i < 3; i++)
  {
    fractionPart *= 2;
    int data = int(fractionPart);
    lcd.print(data);
    fractionPart -= data;
  }
}

static void nvic_init(void)
 {
    // To initialize system, this interrupt should be triggered at main.
    // So, we will set its priority just before calling vTaskStartScheduler(), not the time of enabling each irq.
    NVIC_SetPriority(Ffe0_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(SpiMs_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(CfgDma_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(Uart_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(FbMsg_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
 }    


//needed for startup_EOSS3b.s asm file
void SystemInit(void)
{

}

//gpionum --> 0 --> 31 corresponding to the IO PADs
//gpioval --> 0 or 1
#define FGPIO_DIRECTION_REG (0x40024008)
#define FGPIO_OUTPUT_REG (0x40024004)
#define FGPIO_INPUT_REG (0x40024000)
//Set GPIO(=gpionum) Mode: Input(iomode = 0) or Output(iomode = 1)
//Before Set/Get GPIO value, the direction must be correctly set
void PyHal_GPIO_SetDir(uint8_t gpionum,uint8_t iomode)
{
    uint32_t tempscratch32;

    if (gpionum > 31)
        return;

    tempscratch32 = *(uint32_t*)(FGPIO_DIRECTION_REG);
    if (iomode)
        *(uint32_t*)(FGPIO_DIRECTION_REG) = tempscratch32 | (0x1 << gpionum);
    else
        *(uint32_t*)(FGPIO_DIRECTION_REG) = tempscratch32 & (~(0x1 << gpionum));

}


//Get current GPIO(=gpionum) Mode: Input(iomode = 0) or Output(iomode = 1)
int PyHal_GPIO_GetDir(uint8_t gpionum)
{
    uint32_t tempscratch32;
    int result = 0;

    if (gpionum > 31)
        return -1;

    tempscratch32 = *(uint32_t*)(FGPIO_DIRECTION_REG);

    result = ((tempscratch32 & (0x1 << gpionum)) ? GPIO_OUTPUT_MODE : GPIO_INPUT_MODE);

    return result;
}

//Set GPIO(=gpionum) to 0 or 1 (= gpioval)
//The direction must be set as Output for this GPIO already
//Return value = 0, success OR -1 if error.
int PyHal_GPIO_Set(uint8_t gpionum, uint8_t gpioval)
{
    uint32_t tempscratch32;

    if (gpionum > 31)
        return -1;

    tempscratch32 = *(uint32_t*)(FGPIO_DIRECTION_REG);

    //Setting Direction moved out as separate API, we will only check
    //*(uint32_t*)(FGPIO_DIRECTION_REG) = tempscratch32 | (0x1 << gpionum);
    if (!(tempscratch32 & (0x1 << gpionum)))
    {
        //Direction not Set to Output
        return -1;
    }
    
    tempscratch32 = *(uint32_t*)(FGPIO_OUTPUT_REG);

    if(gpioval > 0)
    {
        *(uint32_t*)(FGPIO_OUTPUT_REG) = tempscratch32 | (0x1 << gpionum);
    }
    else
    {
        *(uint32_t*)(FGPIO_OUTPUT_REG) = tempscratch32 & ~(0x1 << gpionum);
    }    

    return 0;
}
//Get GPIO(=gpionum): 0 or 1 returned (or in erros -1)
//The direction must be set as Input for this GPIO already
int PyHal_GPIO_Get(uint8_t gpionum)
{
    uint32_t tempscratch32;
    uint32_t gpioval_input;

    if (gpionum > 31)
        return -1;

    tempscratch32 = *(uint32_t*)(FGPIO_INPUT_REG);
    gpioval_input = (tempscratch32 >> gpionum) & 0x1;

    return ((int)gpioval_input);
}
//declaring functions

void LCD_setup(void)

{

    //Set GPIO direction
    PyHal_GPIO_SetDir(DAT4,1);
    PyHal_GPIO_SetDir(DAT5,1);
    PyHal_GPIO_SetDir(DAT6,1);
    PyHal_GPIO_SetDir(DAT7,1);    
    PyHal_GPIO_SetDir(LCD_RS,1);
    PyHal_GPIO_SetDir(LCD_E,1);  

}
void PulseEnableLine ()
{
 PyHal_GPIO_Set(LCD_E,1); // take LCD enable line high
 HAL_DelayUSec(40); // wait 40 microseconds
 PyHal_GPIO_Set(LCD_E,0); // take LCD enable line low
}
void SendNibble(byte data)
{
 PyHal_GPIO_Set(DAT4,0);
 PyHal_GPIO_Set(DAT5,0);
 PyHal_GPIO_Set(DAT6,0);
 PyHal_GPIO_Set(DAT7,0);// 1100.0011 = clear 4 data lines
 if (data & _BV(4)) PyHal_GPIO_Set(DAT4,1);
 if (data & _BV(5)) PyHal_GPIO_Set(DAT5,1);
 if (data & _BV(6)) PyHal_GPIO_Set(DAT6,1);
 if (data & _BV(7)) PyHal_GPIO_Set(DAT7,1);
 PulseEnableLine(); // clock 4 bits into controller
}
void SendByte (byte data)
{
 SendNibble(data); // send upper 4 bits
 SendNibble(data<<4); // send lower 4 bits
 //PyHal_GPIO_Set(DAT7,0);// turn off boarduino LED
}
void LCD_Cmd (byte cmd)
{
 PyHal_GPIO_Set(LCD_RS,0); // R/S line 0 = command data
 SendByte(cmd); // send it
}
void LCD_Char (byte ch)
{
 PyHal_GPIO_Set(LCD_RS,1); // R/S line 1 = character data
 SendByte(ch); // send it
}
void LCD_Init()
{
 LCD_Cmd(0x33); // initialize controller
 LCD_Cmd(0x32); // set to 4-bit input mode
 LCD_Cmd(0x28); // 2 line, 5x7 matrix
 LCD_Cmd(0x0C); // turn cursor off (0x0E to enable)
 LCD_Cmd(0x06); // cursor direction = right
 LCD_Cmd(0x01); // start with clear display
 HAL_DelayUSec(3000);    // wait for LCD to initialize
}
void LCD_Clear() // clear the LCD display
{
 LCD_Cmd(CLEARDISPLAY);
 HAL_DelayUSec(3000);   // wait for LCD to process command
}

void LCD_Message(const char *text) // display string on LCD
{
 //while (*text) // do until /0 character
 LCD_Char(*text++); // send char & update char pointer
}

void LCD_Integer(int data)
// displays the integer value of DATA at current LCD cursor position
{
 char st[8] = ""; // save enough space for result
 itoa(data,st,10); // 
 LCD_Message(st); // display in on LCD
}
