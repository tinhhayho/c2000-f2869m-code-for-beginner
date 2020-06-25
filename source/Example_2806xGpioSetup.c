//###########################################################################
// Description:
//! \addtogroup f2806x_example_list
//! <h1>GPIO Setup (gpio_setup)</h1>
//!
//! This example Configures the 2806x GPIO into two different configurations
//! This code is verbose to illustrate how the GPIO could be setup.In a real
//! application, lines of code can be combined for improved code size and 
//! efficiency.
//!
//! This example only sets-up the GPIO.. nothing is actually done with
//! the pins after setup.
//!
//! In general:
//!   - All pullup resistors are enabled.  For ePWMs this may not be desired.
//!   - Input qual for communication ports (eCAN, SPI, SCI, I2C) is asynchronous
//!   - Input qual for Trip pins (TZ) is asynchronous
//!   - Input qual for eCAP and eQEP signals is synch to SYSCLKOUT
//!   - Input qual for some I/O's and interrupts may have a sampling window
//
//
//###########################################################################
// $TI Release: F2806x C/C++ Header Files and Peripheral Examples V151 $
// $Release Date: February  2, 2016 $
// $Copyright: Copyright (C) 2011-2016 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

// Select the example to compile in.  Only one example should be set as 1
// the rest should be set as 0.

#define EXAMPLE1 1  // Basic pinout configuration example
#define EXAMPLE2 0  // Communication pinout example

// Prototype statements for functions found within this file.
void Gpio_setup1(void);
__interrupt void cpu_timer0_isr(void);

void main(void)
{

// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the F2806x_SysCtrl.c file.
   InitSysCtrl();

// Step 2. Initalize GPIO:
// This example function is found in the F2806x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
   // InitGpio(); Skipped for this example

// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
   DINT;

// Initialize PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.
// This function is found in the F2806x_PieCtrl.c file.
   InitPieCtrl();

// Disable CPU interrupts and clear all CPU interrupt flags:
   IER = 0x0000;
   IFR = 0x0000;
   

// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in F2806x_DefaultIsr.c.
// This function is found in F2806x_PieVect.c.
   InitPieVectTable();
   // re-mapping lai vector interrupt
   EALLOW;
   PieVectTable.TINT0 = &cpu_timer0_isr;
   EDIS;
// Step 4. Initialize all the Device Peripherals:
   InitCpuTimer();
// This function is found in F2806x_InitPeripherals.c
// InitPeripherals();
// 90Mhz CPU Freg, 1 second Period (in uSeconds)
   ConfigCpuTimer(&CpuTimer0, 80, 1000000);

   // kich hoat interrupt peripheral level
   CpuTimer0Regs.TCR.all = 0x4000;
   // kich hoat interrupt PIE level
   PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
   // kich hoat interrupt CPU level
   IER |= M_INT1;
   // enable globa interrupt and higher priority real-time debug events:
   EINT;
   ERTM;
    Gpio_setup1();
    while(1);

}

void Gpio_setup1(void)
{

    EALLOW;

   // Make GPIO34 an input
   GpioCtrlRegs.GPBPUD.bit.GPIO34 = 0;  // Enable pullup on GPIO34
   GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0; // GPIO34 = GPIO34
   GpioDataRegs.GPBSET.bit.GPIO34 = 1; // output latch = high
   GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;  // GPIO34 = output
   //
   GpioCtrlRegs.GPBPUD.bit.GPIO39 = 0;
   GpioCtrlRegs.GPBMUX1.bit.GPIO39 = 0;
   GpioDataRegs.GPBCLEAR.bit.GPIO39 = 1; // output latch = low
   GpioCtrlRegs.GPBDIR.bit.GPIO39 = 1; // gpio39 = output
   EDIS;
}
__interrupt void cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;
    GpioDataRegs.GPBTOGGLE.bit.GPIO34 =1;
    GpioDataRegs.GPBTOGGLE.bit.GPIO39 =1;
    // thong bao ngat nay de co the tiep tuc nhan them ngat moi
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
//===========================================================================
// No more.
//===========================================================================

