/* Sketch to get data from CAN bus on Corsa
 *  Bus speed = 500kbps
 *  CAN high = Yellow
 *  CAN low = black
 */

#include <mcp_can.h>
#include <SPI.h>

// ECU PID indices
#define ENGINE_LOAD               0
#define COOLANT_TEMP              1
#define FUEL_PRESSURE             2
#define ENGINE_RPM                3
#define VEHICLE_KPH               4
#define TIMING_ADVANCE            5
#define INTAKE_AIR_TEMP           6
#define AIR_FLOW_RATE             7
#define THROTTLE_POS              8
#define RUNTIME                   9
#define FUEL_LEVEL                10
#define ABSOLUTE_PRESSURE         11
#define OIL_TEMP                  12
#define FUEL_INJECTION_TIMING     13
#define FUEL_RATE                 14
#define TORQUE_PERCENT            15
#define REFERENCE_TORQUE          16

MCP_CAN CAN0(10);

unsigned char len = 0;
unsigned char data_in[9];
long RPM = 0;
long unsigned int rxId = 0;
unsigned char led_pins[11] = {15,14,9,8,7,6,5,4,3,2};
int led_level = 0;

void setup() {
  Serial.begin(115200);
  CAN0.begin(CAN_500KBPS, MCP_8MHz); 
  pinMode(16, INPUT); 

  // setup LED pins
  for(int n = 0; n<10; n++){
    pinMode(led_pins[n], OUTPUT);
    digitalWrite(led_pins[n], LOW);
  }
}

void loop() {
  // request RPM data
  RPM = requestECUdata(REFERENCE_TORQUE);
  //printCANMsg(rxId, len, data_in);
  Serial.print("RPM = ");
  Serial.println(RPM);
  // show on leds
  led_level = (RPM * 10)/4000;
  setOutput(led_level);
  delay(100);
}

void printCANMsg(long unsigned int ID, unsigned char data_count, unsigned char data[9]){
  // print out data  
  Serial.print("ID: ");
  Serial.print(ID, HEX);
  Serial.print("  Data: ");
  for(int i = 0; i<data_count; i++){
    if(data[i] < 0x10){
      Serial.print("0");
    }
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.print("\t");
}

void setOutput(int level){
  if(level > 10){
    level = 10;
  }
  for(int n = 0; n<= level - 1; n++){
    digitalWrite(led_pins[n], HIGH);
  }
  for(int n = level; n<10; n++){
    digitalWrite(led_pins[n], LOW);
  }
}
