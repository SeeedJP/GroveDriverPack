// BOARD Seeed Wio 3G
// GROVE I2C <-> Grove - I2C Color Sensor V2 (SKU#101020341)

#include <GroveDriverPack.h>

#define INTERVAL    (100)

WioCellular Wio;

GroveBoard Board;
GroveI2CColorSensor2 ColorSensor(&Board.I2C);

void setup() {
  delay(200);
  SerialUSB.begin(115200);

  Wio.Init();
  Wio.PowerSupplyGrove(true);
  delay(500);

  Board.I2C.Enable();
  ColorSensor.Init();
}

void loop() {
  ColorSensor.Read();
  SerialUSB.print(ColorSensor.R);
  SerialUSB.print(" ");
  SerialUSB.print(ColorSensor.G);
  SerialUSB.print(" ");
  SerialUSB.print(ColorSensor.B);
  SerialUSB.print(" ");
  SerialUSB.println(ColorSensor.C);
  
  delay(INTERVAL);
}
