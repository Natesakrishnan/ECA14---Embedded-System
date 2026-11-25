// SEVEN SEGMENT LED DISPLAY INTERFACE IN C
// Program to count 0-9 and display it in 7-segment display
// DS3 -> P0.13: Enable = 0, Disable = 1
// DS4 -> P0.12: Enable = 0, Disable = 1
//
// Segment connection:
// DP G F E D C B A
// P0.23 P0.22 P0.21 P0.20 P0.19 P0.18 P0.17 P0.16

#include <LPC214X.H>

#define DS3      (1U << 13)
#define DS4      (1U << 12)
#define SEG_CODE (0xFFU << 16)

unsigned char const seg_dat[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66,
    0x6D, 0x7D, 0x07, 0x7F, 0x67
};

void delayms(int n)
{
    int i, j;

    for (i = 0; i < n; i++) {
        for (j = 0; j < 5035; j++) {
            ;
        }
    }
}

int main(void)
{
    unsigned char count;

    PINSEL0 = 0;  // Configure P0.0-P0.15 as GPIO
    PINSEL1 = 0;  // Configure P0.16-P0.31 as GPIO

    IODIR0 = SEG_CODE | DS3 | DS4;

    IOSET0 = SEG_CODE | DS3;  // Disable DS3
    IOCLR0 = DS4;              // Enable DS4

    count = 0;

    // Display initial count
    IOCLR0 = SEG_CODE;
    IOSET0 = ((unsigned int)seg_dat[count] << 16);

    while (1) {
        delayms(1000);

        count++;

        if (count > 9) {
            count = 0;
        }

        // Display count value
        IOCLR0 = SEG_CODE;
        IOSET0 = ((unsigned int)seg_dat[count] << 16);
    }
}
