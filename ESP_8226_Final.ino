#include <ESP8266WiFi.h>

const char* ssid = "";//type your ssid
const char* password = "";//type your password

const int relayPin = D5; // GPIO5 of ESP8266
WiFiServer ESPserver(80);//Service Port

void setup() 
{
Serial.begin(115200);
pinMode(relayPin, OUTPUT);
digitalWrite(relayPin, HIGH);

Serial.println();
Serial.println();
Serial.print("Connecting to: ");
Serial.println(ssid);

WiFi.begin(ssid, password);
delay(5000);

/*
 The following four line of the 
 code will assign a Static IP Address to 
 the ESP Module. If you do not want this, 
 comment out the following four lines.  
 */

IPAddress ip(192,168,1,254);   
IPAddress gateway(192,168,1,1);   
IPAddress subnet(255,255,255,0);   
WiFi.config(ip, gateway, subnet);
delay(5000);

while (WiFi.status() != WL_CONNECTED) 
{
delay(100);
Serial.print("*");
}
Serial.println("");
Serial.println("WiFi connected");

// Start the server
ESPserver.begin();
Serial.println("Server started");

// Print the IP address
Serial.print("The URL to control ESP8266: ");
Serial.print("http://");
Serial.print(WiFi.localIP());
}

void loop() 
{
// Check if a client has connected
WiFiClient client = ESPserver.available();
if (!client) 
{
return;
}

// Wait until the client sends some data
Serial.println("New Client");
while(!client.available())
{
delay(1);
}

// Read the first line of the request
String request = client.readStringUntil('\r');
Serial.println(request);
client.flush();

// Match the request

int value = LOW;
if (request.indexOf("/RELAYON") != -1) 
{
Serial.println("ON");
digitalWrite(relayPin, HIGH);
value = HIGH;
} 
if (request.indexOf("/RELAYOFF") != -1)
{
Serial.println("OFF");
digitalWrite(relayPin, LOW);
value = LOW;
}

// Return the response
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println(""); //  IMPORTANT
client.println("<!DOCTYPE HTML>");
client.println("<html>");

client.print("Status of the LED: ");

if(value == LOW) 
{
client.print("ON");  
} 
else 
{
client.print("OFF");
}

delay(1);
//client.stop();
Serial.println("Client disconnected");
Serial.println("");
}
