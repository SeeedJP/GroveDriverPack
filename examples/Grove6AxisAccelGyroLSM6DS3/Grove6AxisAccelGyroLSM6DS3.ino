// BOARD Seeed Wio
// GROVE I2C <-> Grove - 6-Axis Accelerometer&Gyroscope (SKU#105020012)

#include <GroveDriverPack.h>

#define INTERVAL    (100)

WioCellular Wio;

GroveBoard Board;
Grove6AxisAccelGyroLSM6DS3 LSM6DS3(&Board.I2C);

void setup() {
  delay(200);
  SerialUSB.begin(115200);

  Wio.Init();
  Wio.PowerSupplyGrove(true);
  delay(500);

  Board.I2C.Enable();
  LSM6DS3.Init();
}

void loop() {
  LSM6DS3.Read();
  SerialUSB.print("Accel: ");
  SerialUSB.print(LSM6DS3.AccelX);
  SerialUSB.print(" ");
  SerialUSB.print(LSM6DS3.AccelY);
  SerialUSB.print(" ");
  SerialUSB.print(LSM6DS3.AccelZ);
  SerialUSB.print("  ");
  SerialUSB.print("Gyro: ");
  SerialUSB.print(LSM6DS3.GyroX);
  SerialUSB.print(" ");
  SerialUSB.print(LSM6DS3.GyroY);
  SerialUSB.print(" ");
  SerialUSB.println(LSM6DS3.GyroZ);

  delay(INTERVAL);
}
