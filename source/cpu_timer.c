// Nguyen Trung Tinh
#include "F2806x_Device.h"
#include "F2806x_Examples.h"


struct CPUTIMER_VARS CpuTimer0;
struct CPUTIMER_VARS CpuTimer1;
struct CPUTIMER_VARS CpuTimer2;


// init cputimers:



void InitCpuTimer(void)
{
    CpuTimer0.RegsAddr = &CpuTimer0Regs;
    // initialize timer period to maximum
    CpuTimer0Regs.PRD.all  = oxFFFFFFFF;
    // initialize pre-scale counter to divide by 1
    CpuTimer0Regs.TPR.all = 0;
    CpuTimer0Regs.TPRH.all =0;
    // make sure timer is stop
    CpuTimer0Regs.TCR.bit.TSS = 1;
    CpuTimer0Regs.TCR.bit.TRB = 1;
}

void ConfigCpuTimer(struct CPUTIMER_VARS *Timer, float Freg, float Period)
{
    Uint32 PeriodInClocks;

    // initialze timer period
    Timer->CPUFreqInMHz = Freq;
    Timer->PeriodInUSec = Period;
    PeriodInClocks = (long)(Freg*Period);
    Timer->RegsAddr->PRD.all = PeriodInClocks - 1; // Counter decrements PRD+1 times each period

    // Set pre-scale counter to divide by 1 (SYSCLKOUT):
    Timer->RegsAddr->TPR.all  = 0;
    Timer->RegsAddr->TPRH.all  = 0;

    // Initialize timer control register:
    Timer->RegsAddr->TCR.bit.TSS = 1;      // 1 = Stop timer, 0 = Start/Restart Timer
    Timer->RegsAddr->TCR.bit.TRB = 1;      // 1 = reload timer
    Timer->RegsAddr->TCR.bit.SOFT = 0;
    Timer->RegsAddr->TCR.bit.FREE = 0;     // Timer Free Run Disabled
    Timer->RegsAddr->TCR.bit.TIE = 1;      // 0 = Disable/ 1 = Enable Timer Interrupt

    // Reset interrupt counter:
    Timer->InterruptCount = 0;

}















