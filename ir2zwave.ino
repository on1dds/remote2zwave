#include <SPI.h>
#include <Ethernet.h>
#include <string.h>
#include <IRremote.h>

/*
 * Dit programma zorgt ervoor dat met de Logitech afstandsbediening de lichten gestuurd worden
 * Hier is weinig tijd ingestoken. Adressen zijn fixed.
 * De appliance probeert DHCP, als dat niet lukt wordt een fixed IP gebruikt.
 * 
 * De gebruikte codes zijn 0 t.e.m. 7 van de Kenwood Receiver remote
 * 
 */

int RECV_PIN = 8;
int STATUS_PIN = 9;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server[] = "192.168.1.3";    // name address for Google (using DNS)
IPAddress ip(192, 168, 1, 177);
EthernetClient client;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  Serial.begin(9600);
  pinMode(STATUS_PIN,OUTPUT);
  
  irrecv.enableIRIn(); // Start the receiver
  
  // 
  if (Ethernet.begin(mac) == 0)
    Ethernet.begin(mac, ip);
  
  delay(1000);

}

void loop() {
  if(getIRCode()){
    Serial.println(results.value, HEX);

    switch(results.value){
      case 0x1D00817e:    // trap aan
        setlight(14,1);
        break;
      case 0x1D0001fe:    // trap uit
        setlight(14,0);
        break;
      case 0x1D0041be:    // dim 0
        setlight(36,0);
        break;
      case 0x1D00c13e:    // dim 10
        setdim(36,10);
        break;
      case 0x1D0021de:    // dim 20
        setdim(36,20);
        break;
      case 0x1D00a15e:    // dim 33
        setdim(36,33);
        break;
      case 0x1D00619e:    // dim 50
        setdim(36,50);
        break;
      case 0x1D00e11e:    // dim 100
        setlight(36,1);
        break;
    }
  }
}

void setlight(int dev, int val){
  if (client.connect(server, 3480)) {
    // Make a HTTP request:
    client.print("GET /data_request?id=lu_action&output_format=xml&DeviceNum=");
    client.print(String(dev, DEC));
    client.print("&serviceId=urn:upnp-org:serviceId:SwitchPower1&action=SetTarget&newTargetValue=");
    client.print(String(val, DEC));
    client.println(" HTTP/1.1");    
    client.println("Host: www.google.com");
    client.println("Connection: close");
    client.println();
  }
  delay(80);
  
  // purge read buffer
  while (client.available()) char c = client.read();
  if (!client.connected())   client.stop();
}

void setdim(int dev, int val){
  if (client.connect(server, 3480)) {
    // Make a HTTP request:
    client.print("GET /data_request?id=lu_action&output_format=xml&DeviceNum=");
    client.print(String(dev, DEC));
    client.print("&serviceId=urn:upnp-org:serviceId:Dimming1&action=SetLoadLevelTarget&newLoadlevelTarget=");
    client.print(String(val, DEC));
    client.println(" HTTP/1.1");    
    client.println("Host: www.google.com");
    client.println("Connection: close");
    client.println();
  }
  delay(80);
  
  // purge read buffer
  while (client.available()) char c = client.read();
  if (!client.connected())   client.stop();
}

boolean getIRCode(){
  if (irrecv.decode(&results)) {
    digitalWrite(STATUS_PIN,HIGH);

    irrecv.resume(); // Receive the next value
    delay(10);
    digitalWrite(STATUS_PIN,LOW);
    if(results.value != 0xFFFFFFFF)
      return true;
  }
  return false;
}




