#include "mbed.h"

// Declare the pin mapped to LED1 as a digital output
DigitalOut led(LED1);

int main() {
    while (1) {
        // toggle the LED (when it's on, turn it off, and vice versa)
        led = !led;

        // print something back to the serial console
        printf("Blink! LED is now %d\n", led.read());

        // wait for 100 ms.
        wait_ms(100);
    }
}
