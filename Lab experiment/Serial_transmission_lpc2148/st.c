#include <lpc214x.h>

void UART0_Init(void)
{
    PLL0CON = 0;
    PLL0FEED = 0xAA;
    PLL0FEED = 0x55;

    VPBDIV = 1;              // Fpclk = 12 MHz

    // DLM, DLL = Fpclk / (19200*16) = 39 = 0x27

    PINSEL0 |= 0x5;          // Select UART0 RXD/TXD
                             // P0.0 ? TXD0
                             // P0.1 ? RXD0

    U0FCR = 0;               // Disable FIFOs

    U0LCR = 0x83;            // 8N1 format, enable Divisor Latch

    U0DLL = 0x27;            // 19200 baud @ PCLK = 12 MHz

    U0DLM = 0x00;

    U0LCR = 0x03;            // Disable Divisor Latch
}

/*------------------------------------------------------------------*/

/* Function to send one character to Serial Port */

void sout(unsigned char dat1)
{
    while(!(U0LSR & 0x20));  // Wait for Tx Buffer Empty

    U0THR = dat1;             // Send character through UART
}

/*------------------------------------------------------------------*/

int main(void)
{
    int dat;

    UART0_Init();

    do
    {
        if(U0LSR & 0x01)      // Check Receiver Data Ready
        {
            dat = U0RBR;       // Receive data from serial port

            sout(dat);         // Send data back to serial port
        }

    } while(1);
}
