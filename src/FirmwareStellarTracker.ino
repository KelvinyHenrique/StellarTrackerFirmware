#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 27, TXPin = 13;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
    Serial.begin(9600);
    ss.begin(GPSBaud);
}

void loop()
{
    // This sketch displays information every time a new sentence is correctly encoded.

    while (ss.available() > 0)
    {

        gps.encode(ss.read());
        if (gps.location.isUpdated())
        {
            // Declare var for location
            double latitude, longitude;
            // Get location
            latitude = gps.location.lat();
            longitude = gps.location.lng();
        }
    }
}