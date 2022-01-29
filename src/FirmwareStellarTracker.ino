#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 27, TXPin = 13;
static const int Sim800RXPin = 19, Sim800TXPin = 18;

static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

SoftwareSerial SoftwareSerialGps(RXPin, TXPin);
SoftwareSerial SoftwareSerialSim800(Sim800RXPin, Sim800TXPin);

String apn = "zap.vivo.com.br";                                    // APN
String apn_u = "vivo";                                             // APN-Username
String apn_p = "vivo";                                             // APN-Password
String url = "http://stellar-tracker.herokuapp.com/api/gps/teste"; // URL of Server

void setup()
{
    Serial.begin(9600);
    SoftwareSerialGps.begin(GPSBaud);
    SoftwareSerialSim800.begin(9600);
    delay(1000);

    // Configure SIM800
      gsm_config_gprs();
}

void gsm_config_gprs()
{
    Serial.println(" --- CONFIG GPRS --- ");
    gsm_send_serial("AT+SAPBR=3,1,Contype,GPRS");
    gsm_send_serial("AT+SAPBR=3,1,APN," + apn);
    if (apn_u != "")
    {
        gsm_send_serial("AT+SAPBR=3,1,USER," + apn_u);
    }
    if (apn_p != "")
    {
        gsm_send_serial("AT+SAPBR=3,1,PWD," + apn_p);
    }
}
void gsm_send_serial(String command)
{
    Serial.println("Send ->: " + command);
    SoftwareSerialSim800.println(command);
    long wtimer = millis();
    while (wtimer + 3000 > millis())
    {
        while (SoftwareSerialSim800.available())
        {
            Serial.write(SoftwareSerialSim800.read());
        }
    }
    Serial.println();
}

void gsm_http_post(String postdata)
{
    Serial.println(" --- Start GPRS & HTTP --- ");
    gsm_send_serial("AT+SAPBR=1,1");
    gsm_send_serial("AT+SAPBR=2,1");
    gsm_send_serial("AT+HTTPINIT");
    gsm_send_serial("AT+HTTPPARA=CID,1");
    gsm_send_serial("AT+HTTPPARA=URL," + url);
    gsm_send_serial("AT+HTTPPARA=CONTENT,application/x-www-form-urlencoded");
    gsm_send_serial("AT+HTTPDATA=192,5000");
    gsm_send_serial(postdata);
    gsm_send_serial("AT+HTTPACTION=1");
    gsm_send_serial("AT+HTTPREAD");
    gsm_send_serial("AT+HTTPTERM");
    gsm_send_serial("AT+SAPBR=0,1");
}

void loop()
{
    // This sketch displays information every time a new sentence is correctly encoded.

    while (SoftwareSerialGps.available() > 0)
    {

        gps.encode(SoftwareSerialGps.read());
        if (gps.location.isUpdated())
        {
            // Declare var for location
            double latitude, longitude;
            // Get location
            latitude = gps.location.lat();
            longitude = gps.location.lng();

            // Declare var for date and time
            int year, month, day, hour, minute, second, milliseconds;
            // Get date and time
            year = gps.date.year();
            month = gps.date.month();
            day = gps.date.day();
            hour = gps.time.hour();
            minute = gps.time.minute();

            // Declare var for altitude
            float altitude;
            // Get altitude
            altitude = gps.altitude.meters();

            // Declare var for speed
            float speed;
            // Get speed
            speed = gps.speed.kmph();

            // Declare var for course
            float course;
            // Get course
            course = gps.course.deg();

            // Declare var for satellites
            int satellites;
            // Get satellites
            satellites = gps.satellites.value();

            // Declare var for HDOP

            float hdop;
            // Get HDOP
            hdop = gps.hdop.hdop();



            // Print data
            Serial.print("Latitude: ");
            Serial.println(latitude);
            Serial.print(" Longitude: ");
            Serial.println(longitude);

            Serial.print(" Date: ");
            Serial.print(day);
            Serial.print("/");
            Serial.print(month);
            Serial.print("/");
            Serial.print(year);

            Serial.print(" Time: ");
            Serial.print(hour);
            Serial.print(":");
            Serial.println(minute);

            
            Serial.print(" Altitude: ");
            Serial.println(altitude);

            Serial.print(" Speed: ");
            Serial.println(speed);

            Serial.print(" Course: ");
            Serial.println(course);
            
            Serial.print(" Satellites: ");
            Serial.println(satellites);

            Serial.print(" HDOP: ");
            Serial.println(hdop);
            
            // End line mark
            Serial.println("####################");
            
            delay(50000);


            // Convert to string
            String lat = String(latitude, 6);
            String lon = String(longitude, 6);

            // Send to server
          //  gsm_http_post("latitude=" + lat + "&longitude=" + lon);

        }
    }
}