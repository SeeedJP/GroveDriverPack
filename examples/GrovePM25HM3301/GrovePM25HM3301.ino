// BOARD Seeed Wio LTE M1/NB1(BG96)
// GROVE I2C <-> Grove - Laser PM2.5 Air Quality Sensor for Arduino - HM3301 (SKU#101020613)

#include <GroveDriverPack.h>
#include <climits>

#define INTERVAL    (2000)

WioCellular Wio;

GroveBoard Board;
GrovePM25HM3301 PM(&Board.I2C);

void setup() {
  delay(200);
  SerialUSB.begin(115200);

  Wio.Init();
  Wio.PowerSupplyGrove(true);
  delay(1500);

  Board.I2C.Enable();
  if (!PM.Init())
  {
    SerialUSB.println("Sensor not found.");
  }

  SerialUSB.print("PM1.0_concentration(STD)");
  SerialUSB.print('\t');
  SerialUSB.print("PM2.5_concentration(STD)");
  SerialUSB.print('\t');
  SerialUSB.print("PM10_concentration(STD)");
  SerialUSB.print('\t');
  SerialUSB.print("PM1.0_concentration(ENV)");
  SerialUSB.print('\t');
  SerialUSB.print("PM2.5_concentration(ENV)");
  SerialUSB.print('\t');
  SerialUSB.print("PM10_concentration(ENV)");
  SerialUSB.print('\t');
  SerialUSB.print("Number_of_particles_0.3um");
  SerialUSB.print('\t');
  SerialUSB.print("Number_of_particles_0.5um");
  SerialUSB.print('\t');
  SerialUSB.print("Number_of_particles_1.0um");
  SerialUSB.print('\t');
  SerialUSB.print("Number_of_particles_2.5um");
  SerialUSB.print('\t');
  SerialUSB.print("Number_of_particles_5.0um");
  SerialUSB.print('\t');
  SerialUSB.print("Number_of_particles_10um");
  SerialUSB.println();
}

void loop() {
  PM.Read();
  if (PM.Concentration_1_Standard != INT_MAX)
  {
    SerialUSB.print(PM.Concentration_1_Standard);
    SerialUSB.print('\t');
    SerialUSB.print(PM.Concentration_2_5_Standard);
    SerialUSB.print('\t');
    SerialUSB.print(PM.Concentration_10_Standard);
    SerialUSB.print('\t');
    SerialUSB.print(PM.Concentration_1_Atmospheric);
    SerialUSB.print('\t');
    SerialUSB.print(PM.Concentration_2_5_Atmospheric);
    SerialUSB.print('\t');
    SerialUSB.print(PM.Concentration_10_Atmospheric);
    SerialUSB.print('\t');
    SerialUSB.print(PM.ParticleNumber_0_3);
    SerialUSB.print('\t');
    SerialUSB.print(PM.ParticleNumber_0_5);
    SerialUSB.print('\t');
    SerialUSB.print(PM.ParticleNumber_1);
    SerialUSB.print('\t');
    SerialUSB.print(PM.ParticleNumber_2_5);
    SerialUSB.print('\t');
    SerialUSB.print(PM.ParticleNumber_5);
    SerialUSB.print('\t');
    SerialUSB.print(PM.ParticleNumber_10);
    SerialUSB.println();
  }

  delay(INTERVAL);
}
