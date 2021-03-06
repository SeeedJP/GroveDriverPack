// BOARD Seeed Wio 3G
// GROVE I2C <-> Grove - 3-Axis Digital Accelerometer(±16g) (SKU#101020054)

#include <GroveDriverPack.h>

#define INTERVAL    (100)

WioCellular Wio;

GroveBoard Board;
GroveAccelerometer16G Accel(&Board.I2C);

void setup() {
  delay(200);
  SerialUSB.begin(115200);

  Wio.Init();
  Wio.PowerSupplyGrove(true);
  delay(500);

  Board.I2C.Enable();
  if (!Accel.Init())
  {
    SerialUSB.println("Sensor not found.");
  }
}

void loop() {
  Accel.Read();

  SerialUSB.print(Accel.X);
  SerialUSB.print(' ');
  SerialUSB.print(Accel.Y);
  SerialUSB.print(' ');
  SerialUSB.println(Accel.Z);

  delay(INTERVAL);
}
