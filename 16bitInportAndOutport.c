/* Program uses a 0.5s timer to toggle the first 8 bits of the output port that 
control 8 LEDs.
The input port contains a 7 bits of data and a valid bit at bit 7. The main program
uses the valid bit to retrive the data and convert it to hex that is output 
to the last 8 bits of the output port that control the HEX DISPLAYS*/

#include "nios2_control.h"

#define TIMER_STATUS (volatile unsigned int*) 0x5000
#define TIMER_CONTROL (volatile unsigned int*) 0x5004
#define TIMER_START_LO (volatile unsigned int*) 0x5008
#define TIMER_START_HI (volatile unsigned int*) 0x500C

#define INPORT_DATA (volatile unsigned int*) 0x6A00
#define OUTPORT_DATA (volatile unsigned int*) 0x7E00

#define JTAG_UART_DATA (volatile unsigned int*) 0xA800
#define JTAG_UART_STATUS (volatile unsigned int*) 0xA804

unsigned int numCycles = 0;

void Init(void){
    *TIMER_START_HI = 25000000 >> 16;
    *TIMER_START_LO = 25000000 & 0xFFFF;
    *TIMER_CONTROL = 0x7;

    *OUTPORT_DATA = 0xF0; //start with left leds on

    NIOS2_WRITE_IENABLE(0x1);
    NIOS2_WRITE_STATUS(0x1);
}

void PrintChar(unsigned int ch){
    unsigned int st;
    do{
        st = *JTAG_UART_STATUS;
        st = st & 0xFFFF0000;
    }while (st == 0);
    *JTAG_UART_DATA = ch;
}

void PrintHexDigit(unsigned int n){
    if (n > 9) printChar(n - 10 + 'A');
    else PrintChar(n + '0');
}

unsigned int CheckChar(void){
    unsigned int info;
    unsigned int result;
    do{
        info = *JTAG_UART_DATA;
        if ((info & 0x8000) == 0) result = 0;
        else result = info & 0xFF;
        return result;
    }
}

void interrupt_handler(void){
    unsigned int ipending = NIOS2_READ_IPENDNG();
    if (ipending & 0x1){
        *TIMER_STATUS = 0;
        numCycles++;
        UpdateLEDs();
    }
}

void UpdateLEDs(void){
    unsigned int bits = *OUTPORT_DATA;
    unsigned int pattern = *OUTPORT_DATA ^ 0x00FF; //will xor the leds
    bits = bits & 0xFF00;
    *OUTPORT_DATA = bits | pattern;
}

int main(){
    Init();

    unsigned int currentData;
    unsigned int bits;
    unsigned int pattern;

    while(1){
        if (numCycles % 20 == 0){ //10 seconds have elapsed
            currentData = *INPORT_DATA & 0xFF00;
            bits = *OUTPORT_DATA & 0x00FF;
            pattern = currentData;
            *OUTPORT_DATA = pattern | bits;
        }
        if (*INPORT_DATA & 0x00)
    }
}
