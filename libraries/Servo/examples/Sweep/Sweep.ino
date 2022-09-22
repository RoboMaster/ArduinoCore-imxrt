#include <Arduino.h>
#include <Servo.h>

Servo myservo;

int pos = 0;

void setup()
{
    myservo.attach(IO8);
}

void loop()
{
    for (pos = 0; pos <= 180; pos += 1)
    {
        myservo.write(pos); // tell servo to go to position in variable 'pos'
        delay(15);          // waits 15ms for the servo to reach the position
    }
    for (pos = 180; pos >= 0; pos -= 1)
    {
        myservo.write(pos); // tell servo to go to position in variable 'pos'
        delay(15);          // waits 15ms for the servo to reach the position
    }
}
