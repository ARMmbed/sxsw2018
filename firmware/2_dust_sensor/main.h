#include "mbed.h"
#include "math.h"
#include "dust_sensor.h"

void dust_sensor_cb(int lpo, float ratio, float concentration) {
    printf("lpo = %d, r = %f, c = %f pcs/0.01cf\n", lpo, ratio, concentration);
}

int main() {
    DustSensor dust(GPIO3);   // Change your pin name here

    while (1) {
        printf("Start measuring...\n");
        dust.measure(&dust_sensor_cb);

        // sleep while the dust sensor is measuring...
        while (dust.is_busy()) {
            wait(1.0f);
        }
    }
}
