#include <Wire.h>

#define I2C_SLAVE_ADDRESS 12
#define PAYLOAD_SIZE 2

int sensorPin = A0;
int sensorValue;
int maxResolutionBits = 10; // Bits number Depends of analogRead resolution
float maxVoltage = 5;
float minVoltage = 0;
float level;

void setup()
{
    Serial.begin(9600);

    Wire.begin(I2C_SLAVE_ADDRESS);
    Wire.onRequest(requestEvents);

    pinMode(sensorPin, INPUT);
}

float calculEquation(float value, float x, float y)
{
    // equation: level = ( ( (value - x) / (-y) ) * 100 ) / 190
    return (((value - x) / (-y)) * 100) / 190;
}

float customMap(float value) // Work only with R2 = 50 ohm
{
    if (value > 853)
    {
        return calculEquation(value, 1024, 17.1);
    }
    else if (value > 731)
    {
        return calculEquation(value, 975, 12.2);
    }
    else if (value > 640)
    {
        return calculEquation(value, 914, 9.14);
    }
    else if (value > 569)
    {
        return calculEquation(value, 853, 7.11);
    }
    else if (value > 512)
    {
        return calculEquation(value, 796, 5.69);
    }
    else if (value > 465)
    {
        return calculEquation(value, 745, 4.65);
    }
    else if (value > 427)
    {
        return calculEquation(value, 698, 3.88);
    }
    else if (value > 394)
    {
        return calculEquation(value, 656, 3.28);
    }
    else if (value > 366)
    {
        return calculEquation(value, 619, 2.81);
    }
    else if (value > 341)
    {
        return calculEquation(value, 585, 2.44);
    }
    else if (value > 269)
    {
        return calculEquation(value, 518, 1.79);
    }
    else if (value > 213)
    {
        return calculEquation(value, 444, 1.17);
    }
    else
    {
        return 0;
    }
}

void loop()
{
    sensorValue = analogRead(sensorPin);
    Serial.println(sensorValue);
    level = constrain(customMap(sensorValue), 0, 100); // level min 0 max 100
    Serial.println(level);
    delay(500);
}

void requestEvents()
{
    String to_send;
    to_send = String("{\"data\": ");
    to_send += level; // Fill data with value (6 char max)
    to_send += String("}");
    Serial.println(to_send);
    Wire.write(to_send.c_str());
}