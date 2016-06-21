/* 
Based on sketch from:
 08/2013 - Matteo Loglio (matlo.me) 	   
*/

#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h> 
#include <math.h>

YunServer server;
String startString;


void setup() {
  //initialize bridge
  Bridge.begin();
  
  //start local server
  server.listenOnLocalhost();
  server.begin();
}

float Thermistor(int RawADC) {
  int RefVolt = analogRead(10);
  float Temp;  // Dual-Purpose variable to save space.
  float Resistance = 10000.00*RawADC/(RefVolt-RawADC);  
  if (RefVolt > RawADC) {
    Temp = log(Resistance); // Saving the Log(resistance) so not to calculate  it 4 times later
    Temp = 1 / (0.000168019 + (.0002466068 * Temp) + (.000000092885 * Temp * Temp * Temp));
    Temp = Temp - 273.15;  // Convert K to Celsius                      
  }
  return Temp;                                     
}

void loop() {
  
  YunClient client = server.accept();
  float temp;


  if (client) {
    String command = client.readString();
    command.trim();
    if (command == "inputs") {

      
      //init JSON data
      client.println("Status:200");
      client.println("content-type:application/json");
      client.println();
      client.println("{");
      
      /*
      loop through the analog pins and for each pin,
      make a JSON line, like:
      "A0" : 450
      */
      for(int i = 0; i <= 5; i++) {  
	float temp;
	temp=Thermistor(analogRead(i));
      
        client.print("\"A");
        client.print(i);
        client.print("\":");        
        
        if (i == 5) {
          client.println(temp);
        } 
        else {
          client.print(temp);
          client.println(",");
        }
      }
      
      //close
      client.println("}");     
    }


    client.stop();
  }

  delay(50); // Poll every 50ms
}


