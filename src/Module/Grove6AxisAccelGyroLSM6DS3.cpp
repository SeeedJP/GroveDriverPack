#include "Grove6AxisAccelGyroLSM6DS3.h"

#define LSM6DS3_REG_ACC_GYRO_WHO_AM_I       (0x0F)
#define LSM6DS3_REG_ACC_GYRO_CTRL1_XL       (0x10)
#define LSM6DS3_REG_ACC_GYRO_CTRL2_G        (0x11)
#define LSM6DS3_REG_ACC_GYRO_OUTX_L_G       (0x22)
#define LSM6DS3_REG_ACC_GYRO_OUTY_L_G       (0x24)
#define LSM6DS3_REG_ACC_GYRO_OUTZ_L_G       (0x26)
#define LSM6DS3_REG_ACC_GYRO_OUTX_L_XL      (0x28)
#define LSM6DS3_REG_ACC_GYRO_OUTY_L_XL      (0x2A)
#define LSM6DS3_REG_ACC_GYRO_OUTZ_L_XL      (0x2C)

#define LSM6DS3_ACC_GYRO_WHO_AM_I           (0x69)
#define LSM6DS3_C_ACC_GYRO_WHO_AM_I         (0x6A)
#define LSM6DS3_ACC_GYRO_BW_XL_100Hz        (0x02)
#define LSM6DS3_ACC_GYRO_FS_XL_16g          (0x04)
#define LSM6DS3_ACC_GYRO_ODR_XL_416Hz       (0x60)
#define LSM6DS3_ACC_GYRO_FS_G_2000dps       (0x0C)
#define LSM6DS3_ACC_GYRO_ODR_G_416Hz        (0x60)

bool Grove6AxisAccelGyroLSM6DS3::Init()
{
    if (!_Device->IsExist()) return false;

    uint8_t val;
    _Device->ReadReg8(LSM6DS3_REG_ACC_GYRO_WHO_AM_I, &val);
    if (val != LSM6DS3_ACC_GYRO_WHO_AM_I &&
        val != LSM6DS3_C_ACC_GYRO_WHO_AM_I)
    {
        return false;
    }

    _Device->WriteReg8(LSM6DS3_REG_ACC_GYRO_CTRL1_XL,
        LSM6DS3_ACC_GYRO_BW_XL_100Hz |
        LSM6DS3_ACC_GYRO_FS_XL_16g |
        LSM6DS3_ACC_GYRO_ODR_XL_416Hz);
    _Device->WriteReg8(LSM6DS3_REG_ACC_GYRO_CTRL2_G,
        LSM6DS3_ACC_GYRO_FS_G_2000dps |
        LSM6DS3_ACC_GYRO_ODR_G_416Hz);

    _IsExist = true;
	return true;
}

void Grove6AxisAccelGyroLSM6DS3::Read()
{
    if (!_IsExist)
    {
        AccelX = NAN;
        AccelY = NAN;
        AccelZ = NAN;
        GyroX = NAN;
        GyroY = NAN;
        GyroZ = NAN;
        return;
    }

    int16_t val;
    _Device->ReadRegN(LSM6DS3_REG_ACC_GYRO_OUTX_L_XL, (uint8_t *)&val, 2);
    AccelX = (float)val * 0.061 * (16 >> 1) / 1000;

    _Device->ReadRegN(LSM6DS3_REG_ACC_GYRO_OUTY_L_XL, (uint8_t *)&val, 2);
    AccelY = (float)val * 0.061 * (16 >> 1) / 1000;

    _Device->ReadRegN(LSM6DS3_REG_ACC_GYRO_OUTZ_L_XL, (uint8_t *)&val, 2);
    AccelZ = (float)val * 0.061 * (16 >> 1) / 1000;

    _Device->ReadRegN(LSM6DS3_REG_ACC_GYRO_OUTX_L_G, (uint8_t *)&val, 2);
    GyroX = (float)val * 4.375 * 16 / 1000;

    _Device->ReadRegN(LSM6DS3_REG_ACC_GYRO_OUTY_L_G, (uint8_t *)&val, 2);
    GyroY = (float)val * 4.375 * 16 / 1000;

    _Device->ReadRegN(LSM6DS3_REG_ACC_GYRO_OUTZ_L_G, (uint8_t *)&val, 2);
    GyroZ = (float)val * 4.375 * 16 / 1000;
}
