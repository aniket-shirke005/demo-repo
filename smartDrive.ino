#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

//Firebase settings
#define FIREBASE_HOST "smartdrive-8af6d-default-rtdb.firebaseio.com"  //should not have http:// and the end  /
#define FIREBASE_AUTH "6B3BhmJC9fyIX05rV8iYkw8QOOXijPRqKvBReUKo"  // go to settings in firebase,select project settings and service account, secrets auth key
#define WIFI_SSID "Pointdexter" //name of the wifi
#define WIFI_PASSWORD "punishergt007" //password for the wifi
int trigPin = D7;
int echoPin = D5;

// The amount of time the ultrassonic wave will be travelling for
long duration = 0;
// Define the distance variable
double distance = 0;

void setup()
{

    // Connect to Wi-Fi
    Serial.print("Wi-Fi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting...");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(10);
    }
    Serial.println();
    Serial.print("Connected to: ");
    Serial.println(WiFi.localIP());

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

    // Ultrasonic sensor, set echo as Input and trigger as Output
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    Serial.begin(115200);
}

void loop()
{

    getDistance();
    // Prints the distance value to the serial monitor
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  
    delay(10);
}

void getDistance()
{
    // Clear trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(10);

    // trigPin HIGH por 10ms
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    //Reads echoPin, returns the travel time of the sound wave in ms
    duration = pulseIn(echoPin, HIGH);

    // Calculating the distance, in centimeters, using the formula described in the first section.
    distance = duration * 0.034 / 2;
  
    // Sends the distance value to Firebase
    Firebase.setFloat("distance", distance);
    if(distance <=10){
      Firebase.set("critical" ,1);
    }else{
      Firebase.set("critical",0);
    }

}
