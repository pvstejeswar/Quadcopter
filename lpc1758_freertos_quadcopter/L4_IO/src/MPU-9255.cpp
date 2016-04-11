/*
 * Author: Dheeraj Dake
 * Description: Definitions for the Acceleration sensor of MPU 9255
 */

#include <stdint.h>
#include <io.hpp>
#include <stdio.h>

bool MPU_9255::init()
{
    //Configuring the accelerometer, GYRO sensitivity
    //const unsigned char accel_sens = (1 << 4) | (1 << 3); //Reg 28: 00011000 = +-16g  -- Sensitivity Factor -- 2048
    const unsigned char accel_sens = 0;
    unsigned char gyro_sens = (1 << 4); //Reg 27: 00010000 = 1000 dps -- Sensitivity factor -- 32.8
    gyro_sens &=  ~(3 << 0);  // Do F_CHOICE bits[1:0] as zero
    gyro_sens = 0;
    unsigned char accel_lpf = 0, gyro_lpf = 0;

    /* Set value 3 for first 3 bits [2:0]
     *Output: 41 HZ BW, 19.80 Milli Sec delay */
    accel_lpf |=  (3 << 0);

    /*Set value 4 for first 3 bits [2:0]
     *OUTPUT:  20 HZ BW, 9.9 Milli Seconds delay*/
    gyro_lpf  |= (1 << 2);


    //write that value to reg 28
    writeReg(ACCEL_CONFIG, accel_sens);
    writeReg(GYRO_CONFIG, gyro_sens);

    writeReg(ACCEL_CONFIG2, accel_lpf);
    writeReg(CONFIG, gyro_lpf);

    //checking whether we are accessing our device
    const char whoAmIReg = readReg(WHO_AM_I);

    //if this gives 0x73 we are good

    //u0_dbg_printf("\nWho ma i %d\n",whoAmIReg);
    LD.setNumber(whoAmIReg);
    return (0x71 == whoAmIReg);   //Don't know how it is 0x71. I checked by passing the reg address.
}

int8_t MPU_9255::validate()
{
    return readReg(WHO_AM_I);
}


//Accelerometer functions
int16_t MPU_9255::AS_getX()
{
    return (int16_t)get16BitRegister(ACCEL_XOUT_H) / 16;
}

int16_t MPU_9255::AS_getY()
{
    return (int16_t)get16BitRegister(ACCEL_YOUT_H) / 16;
}

int16_t MPU_9255::AS_getZ()
{
    return (int16_t)get16BitRegister(ACCEL_ZOUT_H) / 16;
}

//Gyro functions
int16_t MPU_9255::GS_getX()
{
    return (int16_t)get16BitRegister(GYRO_XOUT_H) / 16;
}

int16_t MPU_9255::GS_getY()
{
    return (int16_t)get16BitRegister(GYRO_YOUT_H) / 16;
}

int16_t MPU_9255::GS_getZ()
{
    return (int16_t)get16BitRegister(GYRO_ZOUT_H) / 16;
}

int16_t MPU_9255::temp_val()
{
    return (int16_t)get16BitRegister(TEMP_OUT_H) / 16;
}

//Magnetometer functions
int8_t MPU_9255::device_id()
{
    return readReg(DEVICE_ID);  //returns the device id 0x48
}

int8_t MPU_9255::info()
{
    return readReg(INFO);  //returns the device id 0x48
}

bool MPU_9255::status_1()
{
    return (0x01 == readReg(ST1));  //if data is available to read its 0x01, else 0x00(normal)
}

int16_t MPU_9255::M_getX()
{
    return (int16_t)get16BitRegister_I(HXL) / 16;  //LSB is in a lower address than MSB!
}

int16_t MPU_9255::M_getY()
{
    return (int16_t)get16BitRegister_I(HYL) / 16;
}

int16_t MPU_9255::M_getZ()
{
    return (int16_t)get16BitRegister_I(HZL) / 16;
}

void MPU_9255::status_2()
{
    /** STATUS 2 REGISTER - HOFL and BITM
     * [4:3] = {00, 01, 10, 11}
     * 00   -  14-bit output, Normal
     * 01   -  14-bit output, Magnetic sensor overflow
     * 10   -  16-bit output, Normal
     * 11   -  16-bit output, Magnetic sensor overflow
     */
    switch(readReg(ST2))
    {
        case 0x00:
            printf("14-bit output, Normal\n");
        case 0x08:
            printf("14-bit output, Magnetic sensor overflow\n");
        case 0x10:
            printf("16-bit output, Normal\n");
        case 0x18:
            printf("16-bit output, Magnetic sensor overflow\n");
        default:
            printf("None\n");
    }
}

