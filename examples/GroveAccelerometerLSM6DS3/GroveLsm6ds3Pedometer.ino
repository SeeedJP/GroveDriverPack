// BOARD Seeed Wio
// GROVE I2C <-> Grove - 6-Axis Accelerometer&Gyroscope (SKU#105020012)

#include <GroveDriverPack.h>
#include "GroveAccelerometerLSM6DS3.h"

#define INTERVAL    (500)

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
    LSM6DS3.Setup(LSM6DS3.MODE_STEP_COUNTER);
    SerialUSB.println("Sensor ready.");
  }
}

void loop() {
  uint16_t steps = LSM6DS3.GetStepCounter();

  SerialUSB.print("Step: ");
  SerialUSB.println(steps);

  if (steps > 100)
  {
    LSM6DS3.ClearStepCounter();
  }

  delay(INTERVAL);
}
