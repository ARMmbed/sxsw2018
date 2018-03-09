#ifndef _DUST_SENSOR_H
#define _DUST_SENSOR_H

#include "mbed.h"

class DustSensor : public NonCopyable<DustSensor> {
public:
    DustSensor(PinName pin) : _input(pin), _cb(NULL), _busy(false) {

    }

    bool measure(Callback<void(int, float, float)> cb) {
        if (_busy) {
            return false;
        }

        _cb = cb;

        _busy = true;

        m.attach(callback(this, &DustSensor::calculate), 30.0f);
        t.reset(); // reset mesurement every 30 seconds
        _input.fall(callback(this, &DustSensor::start)); // start measuring when signal is low
        _input.rise(callback(this, &DustSensor::stop));  // stop measuring when signal is high

        return true;
    }

    bool is_busy() {
        return _busy;
    }

private:
    // Start Timer
    void start(void) {
        t.start();
    }

    // Stop Timer
    void stop(void) {
        t.stop();
    }

    // calculate sensor value
    void calculate() {
        // run measurements
        int lpo = t.read_us();
        float ratio = t.read_us() / (30000 * 10.0);
        float concentration = 1.1 * pow(ratio,3) -3.8 * pow(ratio, 2) + 520 * ratio + 0.62;
        _cb(lpo, ratio, concentration);

        _busy = false;
    }

    InterruptIn _input;
    Callback<void(int, float, float)> _cb;

    bool _busy;

    Timer t;                    // measure amount of time signal is low
    Timeout m;                   // every 30 seconds calculate measurement
};

#endif // _DUST_SENSOR_H