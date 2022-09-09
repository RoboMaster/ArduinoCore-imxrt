#ifndef Servo_h
#define Servo_h

#include <inttypes.h>

#define Servo_VERSION 2 // software version of this library

// #define MIN_PULSE_WIDTH     544   // the shortest pulse sent to a servo
// #define MAX_PULSE_WIDTH     2400  // the longest pulse sent to a servo
// #define DEFAULT_PULSE_WIDTH 1500  // default pulse width when servo is attached

#define MIN_PULSE_WIDTH 500  // the shortest pulse sent to a servo
#define MAX_PULSE_WIDTH 2500 // the longest pulse sent to a servo

#define DEFAULT_PULSE_WIDTH 1500 // default pulse width when servo is attached

#define REFRESH_INTERVAL 20000 // minumim time to refresh servos in microseconds

#define SERVO_PWM_FREQ_HZ 50

class Servo
{
  public:
    Servo();
    uint8_t attach(
        int pin,
        int value = DEFAULT_PULSE_WIDTH); // attach the given pin to the next free channel, sets pinMode, set angle value, returns channel number or 0 if failure
    uint8_t attach(int pin, int min, int max, int value = DEFAULT_PULSE_WIDTH); // as above but also sets min and max values for writes.
    void detach();
    void write(int value);             // if value is < 200 its treated as an angle, otherwise as pulse width in microseconds
    void writeMicroseconds(int value); // Write pulse width in microseconds
    int read();                        // returns current pulse width as an angle between 0 and 180 degrees
    int readMicroseconds();            // returns current pulse width in microseconds for this servo (was read_us() in first release)
    bool attached();                   // return true if this servo is attached, otherwise false
  private:
    int8_t min;              // minimum is this value times 4 added to MIN_PULSE_WIDTH
    int8_t max;              // maximum is this value times 4 added to MAX_PULSE_WIDTH
    int servoPin;            // servo pin
    uint8_t isServoAttached; // maximum is this value times 4 added to MAX_PULSE_WIDTH
};

#endif
