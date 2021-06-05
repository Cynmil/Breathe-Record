#include <math.h>
#include "application.h"
#include <HttpClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN 2        // what pin the sensor is connected to
#define DHTTYPE DHT22   // DHT22
#define SOUND_SENSOR A0

unsigned int nextTime = 0;
HttpClient http;

http_header_t headers[] = {
    {"Content-Type", "application/json"},
    // {"Accept", "application/json"},
    {"Accept", "*/*"},
    {NULL, NULL}
};

http_request_t request;
http_response_t response;

DHT dht(DHTPIN, DHTTYPE); 

float hum;  //Stores humidity value
float temp; //Stores temperature value

int size = 100;
int sound_values[100];
int ind;
bool recorded = false;


void setup() {
    Serial.begin(9600);
    
    request.hostname = "34.219.31.155";
    request.port = 5000;
    request.path = "/api";
    dht.begin();
    
    ind = 0;
    pinMode(SOUND_SENSOR, INPUT);
}

void loop() {
    if (nextTime > millis()) {
        return;
    }
    
    hum = dht.readHumidity();
    temp = dht.readTemperature();
    
    // Get Request
    request.body = "{\"temp\":\"" + String(temp, 2) + "\", \"hum\":\"" + String(hum, 2) + "\"}";
    Serial.println(request.body);
    http.post(request, response, headers);
    Serial.print("Application>\tResponse status: ");
    Serial.println(response.status);
    
    Serial.print("Application>\tHTTP Response Body: ");
    Serial.println(response.body);
    
    nextTime = millis() + 500;
}

// The folow code was for soubd sensor calibration.
// We found out the analog output of (2200) approximately matches with (35) dB.
// it turns out the soun d sensor is not supposed to record the noise level so this cod eis obsolete

// int sound = 0;
// sound = analogRead(SOUNDPIN);

// if (ind < 20) {
//     sound_values[ind++] = sound;
// } else {
//     ind = 0;
//     sound_values[ind++] = sound;
//     recorded = true;
// }

// if (recorded) {
//     int sum = 0;
//     for (int i = 0; i < 20; i++) {
//         sum += sound_values[i];
//     }
//     double avg = sum / 20;
    
//     Serial.print("Average sound ouputs: ");
//     Serial.println(avg);
// }

// Serial.print("the sound output: ");
// Serial.println(sound);