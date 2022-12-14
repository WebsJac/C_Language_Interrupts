/*Program uses 2 timers, one to cycle LEDs and one to tell
main program to compare past and present inport values.
Prints >, = or < based on comparison and updates a running
count, sum and average of all inport values*/

#include "nios2_control.h"

#define TIMER0_STATUS (volatile unsigned int*) 0x5000
#define TIMER0_CONTROL (volatile unsigned int*) 0x5004
#define TIMER0_START_HI (volatile unsigned int*) 0x5008
#define TIMER0_START_LO (volatile unsigned int*) 0x500C

#define TIMER0_STATUS (volatile unsigned int*) 0x6000
#define TIMER0_CONTROL (volatile unsigned int*) 0x6004
#define TIMER0_START_HI (volatile unsigned int*) 0x6008
#define TIMER0_START_LO (volatile unsigned int*) 0x600C

#define INPORT_DATA (volatile unsigned int*) 0x6700
#define OUTPORT_DATA (volatile unsigned int*) 0x6800

#define JTAG_UART_DATA (volatile unsigned int*) 0x6A00
#define JTAG_UART_STATUS (volatile unsigned int*) 0x6A04

unsigned int timer1Flag = 0;
unsigned int previousData;
unsigned int average;

void Init(void){
    *TIMER0_START_HI = 6250000 >> 16;
    *TIMER0_START_LO = 6250000 & 0xFFFF;
    *TIMER0_CONTROL = 0x7;

    *TIMER1_START_HI = 25000000 >> 16; 
    *TIMER1_START_LO = 25000000 & 0xFFFF;
    *TIMER1_CONTROL = 0x7;

    *OUTPORT_DATA = 0x80;
    previousData = *INPORT_DATA;

    NIOS2_WRITE_IENABLE(0x60);
    NIOS2_WRITE_STATUS(0x1);
}

void interrupt_handler(void){
    unsigned int ipending = NIOS2_READ_IPENDING();

    if (ipending & 0x20){ //timer 0 interrupt
        *TIMER0_STATUS = 0; //clear interrupt
        HandleTimer0();
    }
    if (ipending & 0x40){ //timer 1 interrupt
        *TIMER1_STATUS = 0;
        HandleTimer1();
    }
}

void PrintChar(unsigned int ch){
    unsigned int st;
    do { 
        st = *JTAG_UART_STATUS;
        st = st & 0xFFFF0000;
    }while (st == 0);
    *JTAG_UART_DATA = ch;
}

void HandleTimer0(void){
    if (*OUTPORT_DATA == 0x1) *OUTPORT_DATA = 0x8; //wrap around
    else *OUTPORT_DATA = *OUTPORT_DATA >> 1;
}

void HandleTimer1(void){
    timer1Flag = 1; //set flag
}

int main(void){
    Init();

    unsigned int count = 1;
    unsigned int sum = previousData;

    while(1){
        if (timer1Flag = 1){
            timer0Flag = 0;
            if (*INPORT_DATA > previousData) PrintChar('>');
            else if (*INPORT_DATA == previousData) PrintChar('=');
            else PrintChar('<');

            count++;
            if (sum + *INPORT_DATA < sum) sum = 2^32; //overflow case
            else sum += *INPORT_DATA;
            average = sum/count;

            previousData = *INPORT_DATA;
            PrintChar('\n');
        }
    }
}
