#include "GroveAccelerometerLSM6DS3.h"
#include "LSM6DS3.h"
#include <math.h>

bool GroveAccelerometerLSM6DS3::Init()
{
    uint8_t val;
    _Device->ReadReg8(LSM6DS3_ACC_GYRO_WHO_AM_I_REG, &val);
    if (val != LSM6DS3_ACC_GYRO_WHO_AM_I &&
        val != LSM6DS3_C_ACC_GYRO_WHO_AM_I)
    {
        return false;
    }

	_IsExist = true;
	return true;
}

void GroveAccelerometerLSM6DS3::Setup(lsm6ds3_mode_t mode)
{
    if (!_IsExist)
    {
        return;
    }

    switch(mode) {
        case MODE_FREEFALL_DETECTION:
            _Device->WriteReg8(LSM6DS3_ACC_GYRO_CTRL1_XL,
                LSM6DS3_ACC_GYRO_BW_XL_200Hz |
                LSM6DS3_ACC_GYRO_FS_XL_2g |
                LSM6DS3_ACC_GYRO_ODR_XL_416Hz);
            _Device->WriteReg8(LSM6DS3_ACC_GYRO_FREE_FALL,
                LSM6DS3_ACC_GYRO_FF_THS_10 |
                0x30);
            _Device->WriteReg8(LSM6DS3_ACC_GYRO_MD1_CFG,
                LSM6DS3_ACC_GYRO_INT1_FF_ENABLED);
            _Device->WriteReg8(LSM6DS3_ACC_GYRO_MD2_CFG,
                LSM6DS3_ACC_GYRO_INT2_FF_ENABLED);
            _Device->WriteReg8(LSM6DS3_ACC_GYRO_TAP_CFG1,
                LSM6DS3_ACC_GYRO_LIR_ENABLED);
            break;
        case MODE_STEP_COUNTER:
            _Device->WriteReg8(LSM6DS3_ACC_GYRO_CTRL1_XL,
                LSM6DS3_ACC_GYRO_BW_XL_400Hz |
                LSM6DS3_ACC_GYRO_FS_XL_2g |
                LSM6DS3_ACC_GYRO_ODR_XL_26Hz);
            _Device->WriteReg8(LSM6DS3_ACC_GYRO_CTRL10_C,
                LSM6DS3_ACC_GYRO_PEDO_RST_STEP_ENABLED |
                LSM6DS3_ACC_GYRO_FUNC_EN_ENABLED |
                LSM6DS3_ACC_GYRO_XEN_G_ENABLED |
                LSM6DS3_ACC_GYRO_YEN_G_ENABLED |
                LSM6DS3_ACC_GYRO_ZEN_G_ENABLED);
            _Device->WriteReg8(LSM6DS3_ACC_GYRO_TAP_CFG1,
                LSM6DS3_ACC_GYRO_PEDO_EN_ENABLED);
            break;
        case MODE_BASIC_MEASURE:
        default:
            _Device->WriteReg8(LSM6DS3_ACC_GYRO_CTRL1_XL,
                LSM6DS3_ACC_GYRO_BW_XL_100Hz |
                LSM6DS3_ACC_GYRO_FS_XL_16g |
                LSM6DS3_ACC_GYRO_ODR_XL_416Hz);
            _Device->WriteReg8(LSM6DS3_ACC_GYRO_CTRL2_G,
                LSM6DS3_ACC_GYRO_FS_G_2000dps |
                LSM6DS3_ACC_GYRO_ODR_G_416Hz);
            break;
    }
    this->_mode = mode;
}

void GroveAccelerometerLSM6DS3::ReadAccel()
{
	if (!_IsExist)
	{
		accelX = NAN;
		accelY = NAN;
		accelZ = NAN;
		return;
	}

    int16_t val;
    _Device->ReadRegN(LSM6DS3_ACC_GYRO_OUTX_L_XL, (uint8_t *)&val, 2);
    accelX = (float)val * 0.061 * (16 >> 1) / 1000;

    _Device->ReadRegN(LSM6DS3_ACC_GYRO_OUTY_L_XL, (uint8_t *)&val, 2);
    accelY = (float)val * 0.061 * (16 >> 1) / 1000;

    _Device->ReadRegN(LSM6DS3_ACC_GYRO_OUTZ_L_XL, (uint8_t *)&val, 2);
    accelZ = (float)val * 0.061 * (16 >> 1) / 1000;
}

void GroveAccelerometerLSM6DS3::ReadGyro()
{
	if (!_IsExist)
	{
		gyroX = NAN;
		gyroY = NAN;
		gyroZ = NAN;
		return;
	}

    uint8_t div = 16;
    int16_t val;
    _Device->ReadRegN(LSM6DS3_ACC_GYRO_OUTX_L_G, (uint8_t *)&val, 2);
    gyroX = (float)val * 4.375 * div / 1000;

    _Device->ReadRegN(LSM6DS3_ACC_GYRO_OUTY_L_G, (uint8_t *)&val, 2);
    gyroY = (float)val * 4.375 * div / 1000;

    _Device->ReadRegN(LSM6DS3_ACC_GYRO_OUTZ_L_G, (uint8_t *)&val, 2);
    gyroZ = (float)val * 4.375 * div / 1000;
}

uint16_t GroveAccelerometerLSM6DS3::GetStepCounter()
{
    uint16_t count;
    uint8_t val;
    _Device->ReadReg8(LSM6DS3_ACC_GYRO_STEP_COUNTER_H, &val);
    count = val << 8;
    _Device->ReadReg8(LSM6DS3_ACC_GYRO_STEP_COUNTER_L, &val);
    count |= val;
    return count;
}

void GroveAccelerometerLSM6DS3::ClearStepCounter()
{
    uint8_t val;
    _Device->ReadReg8(LSM6DS3_ACC_GYRO_CTRL10_C, &val);
    val |= LSM6DS3_ACC_GYRO_PEDO_RST_STEP_ENABLED;
    _Device->WriteReg8(LSM6DS3_ACC_GYRO_CTRL10_C, val);
}

bool GroveAccelerometerLSM6DS3::WaitForFreeFallEvent(uint32_t timeout_ms, uint32_t polling_ms)
{
    uint8_t val;
    while (timeout_ms)
    {
        _Device->ReadReg8(LSM6DS3_ACC_GYRO_WAKE_UP_SRC, &val);
        if (val & LSM6DS3_ACC_GYRO_FF_EV_STATUS_DETECTED)
        {
            return true;
        }

        uint32_t delay_ms = polling_ms;
        if (timeout_ms < polling_ms)
        {
            delay_ms = timeout_ms;
        }
        delay(delay_ms);
        timeout_ms -= delay_ms;
    }
    return false;
}