// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLgidP3BHI"
#define BLYNK_DEVICE_NAME "FARF v2"
#define BLYNK_AUTH_TOKEN "tRvjuogcctucnNdHHfxw-J97uVsmGOOD"

//---------------TDS Code---------------------------------------
#define SERIAL Serial
#define sensorPin 4
int sensorValue = 0;
float tdsValue = 0;
float Voltage = 0;

//-----------------------------------------------------
// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Cicada";
char pass[] = "6dnpnanv";

void setup() {
  Serial.begin(115200);
  //pass auth token, network ssid, pwd to blynk.
  Blynk.begin(auth, ssid, pass);

  //Sensor pin config
  pinMode(4,INPUT);
  pinMode(5,OUTPUT);

}

void loop() {
  Blynk.run();
  Blynk.virtualWrite(V2,"ONLINE");
  sensorValue = analogRead(sensorPin);  
  //Configured for ESP's 12 bit ADC
  Voltage = sensorValue*5/4096.0; //Convert the given reading to voltage
  tdsValue=(133.42*Voltage*Voltage*Voltage - 255.86*Voltage*Voltage + 857.39*Voltage)*0.5; //Convert voltage value to TDS value
  Serial.print("TDS Value = "); 
  Serial.print(tdsValue);
  Serial.println(" ppm");
  Blynk.virtualWrite(V0, tdsValue);

  if(tdsValue<=15){
    Blynk.virtualWrite(V5,"PROBE REMOVED!");
    Blynk.virtualWrite(V3,"--");
    Blynk.virtualWrite(V1,LOW);
    Blynk.virtualWrite(V4,LOW);
    }
  else if(tdsValue<100 && tdsValue>15){
    Blynk.virtualWrite(V5,"HYPOTONICITY DETECTED!"); //e.g. poor quality RO
    Blynk.virtualWrite(V3,"BAD");
    Blynk.virtualWrite(V1,HIGH);
    Blynk.virtualWrite(V4,LOW);
//Buzzer test
//    digitalWrite(5,HIGH);
//    delay(500);
//    digitalWrite(5,LOW);
//    delay(500);
    }
  else if(tdsValue>100 && tdsValue<300){ //e.g. ideal municipality tap water
    Blynk.virtualWrite(V3,"EXCELLENT");
    Blynk.virtualWrite(V1,LOW);
    Blynk.virtualWrite(V4,HIGH);
    }
   else if(tdsValue>300 && tdsValue<500){ //actual tap water in Udhna, Surat City
    Blynk.virtualWrite(V5,"MILDLY HYPERTONIC!");
    Blynk.virtualWrite(V3,"GOOD");
    Blynk.virtualWrite(V1,LOW);
    Blynk.virtualWrite(V4,HIGH);
    
    }
  else if(tdsValue>500){
    Blynk.virtualWrite(V5,"HYPERTONICITY DETECTED!"); //not suitable for drinking
    Blynk.virtualWrite(V3,"CRITICAL");
    Blynk.virtualWrite(V4,LOW);
    Blynk.virtualWrite(V1,HIGH);
    }
  
  delay(1000);
  
}
