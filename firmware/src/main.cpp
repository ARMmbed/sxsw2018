#include "mbed.h"
#include "ttn_config.h"
#include "CayenneLPP.h"

mDot* dot = NULL;

DigitalOut led(LED1, 0);

int main() {
    // Custom event handler for automatically displaying RX data
    RadioEvent events;

    // See ttn_config.h for the credentials used
    connect_to_ttn(&events);

    while (true) {
        printf("\n");

        // join network if not joined
        if (!dot->getNetworkJoinStatus()) {
            join_network();
        }

        // after joining turn the LED on
        led = 1;

        float v = (float)rand()/(float)(RAND_MAX/5);
        printf("[INFO] Sending %.2f\n", v);

        // We're using CayenneLPP format, a compact data format
        CayenneLPP payload(50);
        payload.addAnalogInput(1, v);

        // send the data to the network
        send_data(&payload);

        // calculate how long we need to sleep (based on duty cycle and other regulations)
        uint32_t sleep_time = calculate_actual_sleep_time(10 /* minimum 10 seconds sleep time */);

        printf("[INFO] Going to sleep for %lu seconds (or until WAKE pin is triggered)\n", sleep_time);

        // sleep until the wake pin gets triggered, or until sleep_time
        sleep_wake_rtc_or_interrupt(sleep_time);

        printf("\n"); // flush buffer because we just came out of sleep
    }

    return 0;
}
