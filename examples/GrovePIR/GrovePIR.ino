// BOARD Seeed Wio LTE M1/NB1(BG96)
// GROVE D38 <-> Grove - Adjustable PIR Motion Sensor (SKU#101020617)

#include <GroveDriverPack.h>

#define INTERVAL    (100)

GroveBoard Board;
GrovePIR Sensor(&Board.D38);

void setup() {
  delay(200);
  SerialUSB.begin(115200);

  Board.D38.Enable();
  Sensor.Init();
}

void loop() {
  bool state = Sensor.IsOn();
  SerialUSB.print(state ? '*' : '.');
  
  delay(INTERVAL);
}
