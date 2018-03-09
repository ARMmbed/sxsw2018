#include "mbed.h"
#include "ttn_config.h"
#include "CayenneLPP.h"
#include "dust_sensor.h"

mDot* dot = NULL;

DigitalOut led(LED1, 0);
DustSensor dust(GPIO3);

float dust_concentration = 0.0f;

void dust_sensor_cb(int lpo, float ratio, float concentration) {
    dust_concentration = concentration;

    printf("[INFO] Measured concentration = %.2f pcs/0.01cf\n", concentration);
}

int main() {
    // Custom event handler for automatically displaying RX data
    RadioEvent events;

    // See ttn_config.h for the credentials used
    connect_to_ttn(&events);

    while (true) {
        // join network if not joined
        if (!dot->getNetworkJoinStatus()) {
            join_network();
        }

        // after joining turn the LED on
        led = 1;

        printf("[INFO] Measuring dust...\n");
        dust.measure(&dust_sensor_cb);

        while (dust.is_busy()) {
            wait_ms(1000);
        }

        printf("[INFO] Sending %.2f\n", dust_concentration);

        // We're using CayenneLPP format, a compact data format
        CayenneLPP payload(50);
        payload.addAnalogInput(1, dust_concentration / 100.0f); // save space

        // send the data to the network
        send_data(&payload);

        // calculate how long we need to sleep (based on duty cycle and other regulations)
        uint32_t sleep_time = calculate_actual_sleep_time(5 /* minimum 5 seconds sleep time */);

        printf("[INFO] Going to sleep for %lu seconds (or until WAKE pin is triggered)\n", sleep_time);

        // sleep until the wake pin gets triggered, or until sleep_time
        sleep_wake_rtc_or_interrupt(sleep_time);

        printf("\n"); // flush buffer because we just came out of sleep
    }

    return 0;
}
