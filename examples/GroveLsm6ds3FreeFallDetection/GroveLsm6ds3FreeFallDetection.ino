// BOARD Seeed Wio
// GROVE I2C <-> Grove - 6-Axis Accelerometer&Gyroscope (SKU#105020012)

#include <GroveDriverPack.h>

#define INTERVAL    (100)

WioCellular Wio;

GroveBoard Board;
GroveAccelerometerLSM6DS3 LSM6DS3(&Board.I2C);

void setup() {
  delay(200);
  SerialUSB.begin(115200);

  Wio.Init();
  Wio.PowerSupplyGrove(true);
  delay(500);

  Board.I2C.Enable();
  if (!LSM6DS3.Init())
  {
    SerialUSB.println("Sensor not found.");
  }
  else
  {
    LSM6DS3.Setup(LSM6DS3.MODE_FREEFALL_DETECTION);
    SerialUSB.println("Sensor ready.");
  }
}

void loop() {
  if (LSM6DS3.WaitForFreeFallEvent(3000, 100))
  {
    SerialUSB.println("Free Fall detected.");
  }
}
