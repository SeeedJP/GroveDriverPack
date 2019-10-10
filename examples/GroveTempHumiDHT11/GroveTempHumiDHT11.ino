// BOARD Seeed Wio 3G
// GROVE D38 <-> Grove - Temperature & Humidity Sensor (DHT11) (SKU#101020011)

#include <GroveDriverPack.h>

#define INTERVAL    (2000)

GroveBoard Board;
GroveTempHumiDHT11 TempHumi(&Board.D38);

void setup() {
  delay(200);
  SerialUSB.begin(115200);

  Board.D38.Enable();
  TempHumi.Init();
}

void loop() {
  TempHumi.Read();
  
  SerialUSB.print("Current humidity = ");
  SerialUSB.print(TempHumi.Humidity);
  SerialUSB.print("%  ");
  SerialUSB.print("temperature = ");
  SerialUSB.print(TempHumi.Temperature);
  SerialUSB.println("C");

  delay(INTERVAL);
}
