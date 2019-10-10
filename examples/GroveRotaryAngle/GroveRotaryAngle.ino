// BOARD Seeed Wio 3G
// GROVE A6 <-> Grove - Rotary Angle Sensor (SKU#101020017)

#include <GroveDriverPack.h>

#define INTERVAL    (100)

WioCellular Wio;

GroveBoard Board;
GroveRotaryAngle RotaryAngle(&Board.A6);

void setup() {
  delay(200);
  SerialUSB.begin(115200);

  Wio.Init();
  Wio.PowerSupplyGrove(true);
  delay(500);

  Board.A6.Enable();
  RotaryAngle.Init();
}

void loop() {
  RotaryAngle.Read();

  SerialUSB.println(RotaryAngle.Volume);

  delay(INTERVAL);
}
