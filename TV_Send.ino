#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h> 
#include <ArduinoJson.h>
// IR part-------------------------------------------------------------
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRsend.h>

#define IR_LED 4  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(IR_LED);  // Set the GPIO to be used to sending the message.

ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;
WiFiClient client;

#define MyApiKey "xxxxxxxx-xxxxx-xxxx-xxxxx-xxxxxxxxx" // TODO: Change to your sinric API Key. Your API Key is displayed on sinric.com dashboard
#define MySSID "xxxxxxxxxx" // TODO: Change to your Wifi network SSID
#define MyWifiPassword "xxxxxxxxxxxxxxxxxx" // TODO: Change to your Wifi network password

#define API_ENDPOINT "http://sinric.com"
#define HEARTBEAT_INTERVAL 300000 // 5 Minutes 


long unsigned int irSignal_Off     = 0xE0E040BF;

long unsigned int irSignal_0       = 0xE0E08877;
long unsigned int irSignal_1       = 0xE0E020DF;
long unsigned int irSignal_2       = 0xE0E0A05F;
long unsigned int irSignal_3       = 0xE0E0609F;
long unsigned int irSignal_4       = 0xE0E010EF;
long unsigned int irSignal_5       = 0xE0E0906F;
long unsigned int irSignal_6       = 0xE0E050AF;
long unsigned int irSignal_7       = 0xE0E030CF;
long unsigned int irSignal_8       = 0xE0E0B04F;
long unsigned int irSignal_9       = 0xE0E0708F;

long unsigned int irSignal_Mute    = 0xE0E0F00F;
long unsigned int irSignal_volUp   = 0xE0E0E01F;
long unsigned int irSignal_volDown = 0xE0E0D02F;

long unsigned int irSignal_ChUp    = 0xE0E048B7;
long unsigned int irSignal_ChDown  = 0xE0E008F7;
long unsigned int irSignal_ChList  = 0xE0E0D629;

long unsigned int irSignal_PreCh   = 0xE0E0C837;
long unsigned int irSignal_Minus   = 0xE0E0C43B;

long unsigned int irSignal_Up      = 0xE0E006F9;
long unsigned int irSignal_Down    = 0xE0E08679;
long unsigned int irSignal_Left    = 0xE0E0A659;
long unsigned int irSignal_Right   = 0xE0E046B9;
long unsigned int irSignal_Ok      = 0xE0E016E9;

long unsigned int irSignal_Return  = 0xE0E01AE5;
long unsigned int irSignal_Source  = 0xE0E0807F;

long unsigned int irSignal_Info    = 0xE0E0F807;
long unsigned int irSignal_Menu    = 0xE0E058A7;
long unsigned int irSignal_Tools   = 0xE0E0D22D;

long unsigned int irSignal_A       = 0xE0E036C9;
long unsigned int irSignal_B       = 0xE0E028D7;
long unsigned int irSignal_C       = 0xE0E0A857;
long unsigned int irSignal_D       = 0xE0E06897;

long unsigned int irSignal_Play    = 0xE0E0E21D;
long unsigned int irSignal_Stop    = 0xE0E0629D;
long unsigned int irSignal_Pause   = 0xE0E052AD;
long unsigned int irSignal_Rwd     = 0xE0E0A25D;
long unsigned int irSignal_Fwd     = 0xE0E012ED;

unsigned int irSerialCommand=999;

const unsigned long DELAY_BETWEEN_COMMANDS = 500;


uint64_t heartbeatTimestamp = 0;
bool isConnected = false;

void sendSignal(long unsigned int irSignal,char *irSignalCommand)
{
   irsend.sendSAMSUNG(irSignal, 32);
  Serial.print("Sende Taste: ");
  Serial.println(irSignalCommand);
  delay(DELAY_BETWEEN_COMMANDS);
}

void TogglePower(String deviceId) {
  if (deviceId == "xxxxxxxxxxxxxxxxxxxxxxxxxxx") // TODO: Change to your device Id for TV
  {  
     irsend.sendSAMSUNG(0xE0E040BF,32);  // Send a power signal to Samsung TV.
     delay(DELAY_BETWEEN_COMMANDS);
  } 
}

void ToggleMute(String deviceId) {
  if (deviceId == "xxxxxxxxxxxxxxxxxxxxxxxxxxxx") // TODO: Change to your device Id for TV
  {  
     irsend.sendSAMSUNG(0xE0E0F00F,32);  // Send a power signal to Samsung TV.
     delay(DELAY_BETWEEN_COMMANDS);
  } 
 }


void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      isConnected = false;    
      Serial.printf("[WSc] Webservice disconnected from sinric.com!\n");
      break;
    case WStype_CONNECTED: {
      isConnected = true;
      Serial.printf("[WSc] Service connected to sinric.com at url: %s\n", payload);
      Serial.printf("Waiting for commands from sinric.com ...\n");        
      }
      break;
    case WStype_TEXT: {
        Serial.printf("[WSc] get text: %s\n", payload);
           
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject((char*)payload); 
        String deviceId = json ["deviceId"];     
        String action = json ["action"];
        
        if(action == "setPowerState") { 
            String value = json ["value"];
            if(value == "ON" || value == "OFF" ) {
                TogglePower(deviceId);
                }
        }

        else if(action == "ChangeChannel") {
          String ChannelName=json ["value"]["channelMetadata"]["name"];
          String ChannelNumber=json ["value"]["channel"]["number"];

        for (unsigned int i = 0; i < ChannelNumber.length(); i++) {
          switch (ChannelNumber[i]) {
            case '1':
              sendSignal(irSignal_1, "1");
              break;
            case '2':
              sendSignal(irSignal_2, "2");
              break;
            case '3':
              sendSignal(irSignal_3, "3");
              break;
            case '4':
              sendSignal(irSignal_4, "4");
              break;
            case '5':
              sendSignal(irSignal_5, "5");
              break;
            case '6':
              sendSignal(irSignal_6, "6");
              break;
            case '7':
              sendSignal(irSignal_7, "7");
              break;
            case '8':
              sendSignal(irSignal_8, "8");
              break;
            case '9':
              sendSignal(irSignal_9, "9");
              break;
            case '0':
              sendSignal(irSignal_0, "0");
              break;
          }
          delay(DELAY_BETWEEN_COMMANDS);
          Serial.println("[WSc] channel: " + ChannelNumber);
        }
        delay(DELAY_BETWEEN_COMMANDS);
        sendSignal(irSignal_Ok, "Ok");
        //return true;
        
     }
       
        else if (action == "SetMute") {
          
         bool MuteAction=json ["value"]["mute"];
          if(MuteAction==true || MuteAction==false){ 
             ToggleMute(deviceId);
           }
          
        }

        if(action == "AdjustVolume") { 
            int value = json ["value"]["volume"];
              if (value < 0) {
                Serial.print("Volume down: ");
                Serial.println(value);

                  for (int i = 0; i < -value; i++ ) {
                    sendSignal(irSignal_volDown, "Down");
                    delay(DELAY_BETWEEN_COMMANDS);
                  }
                  
              } else if (value > 0) {
                Serial.print("Volume up: ");
                Serial.println(value);
                
                  for (int i = 0; i < value; i++ ) {
                    sendSignal(irSignal_volUp, "Up");
                    delay(DELAY_BETWEEN_COMMANDS);
                  }
                  
                }
              }

        else if (action == "test") {
            Serial.println("[WSc] received test command from sinric.com");
        }
        
      }
      break;
    case WStype_BIN:
      Serial.printf("[WSc] get binary length: %u\n", length);
      break;
  }
}

void setup() {
  Serial.begin(115200);
    irsend.begin();

 WiFiMulti.addAP(MySSID, MyWifiPassword);
  Serial.println();
  Serial.print("Connecting to Wifi: ");
  Serial.println(MySSID);  

  // Waiting for Wifi connect
  while(WiFiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  if(WiFiMulti.run() == WL_CONNECTED) {
    Serial.println("");
    Serial.print("WiFi connected. ");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }

  // server address, port and URL
  webSocket.begin("iot.sinric.com", 80, "/");

  // event handler
  webSocket.onEvent(webSocketEvent);
  webSocket.setAuthorization("apikey", MyApiKey);
  
  // try again every 5000ms if connection has failed
  webSocket.setReconnectInterval(5000);   // If you see 'class WebSocketsClient' has no member named 'setReconnectInterval' error update arduinoWebSockets
}

void loop() {
  webSocket.loop();
  
  if(isConnected) {
      uint64_t now = millis();
      
      // Send heartbeat in order to avoid disconnections during ISP resetting IPs over night. Thanks @MacSass
      if((now - heartbeatTimestamp) > HEARTBEAT_INTERVAL) {
          heartbeatTimestamp = now;
          webSocket.sendTXT("H");          
      }
  }   
}
