/*
 * Author: Dheeraj Dake
 * Status: Control registers @Magnetometer
 */

#ifndef MPU_9255_HPP_
#define MPU_9255_HPP_

#include <stdint.h>
#include "i2c2_device.hpp"  // I2C Device Base Class

/**
 * The IMU sensor has 2 die's comprising of accelerometer, gyroscope and magnetometer.
 * It has 9 Degrees of Freedom. It can be connected in SPI or I2C. I am using I2C.
 *
 */

class MPU_9255: private i2c2_device, public SingletonTemplate<MPU_9255>
{
    public:
        bool init(); //Initializes the sensor
        int8_t validate();
        int16_t AS_getX(); //returns the X-Axis value
        int16_t AS_getY(); //returns the Y-Axis value
        int16_t AS_getZ(); //returns the Z-Axis value

        int16_t GS_getX(); //returns the X-Axis value
        int16_t GS_getY(); //returns the Y-Axis value
        int16_t GS_getZ(); //returns the Z-Axis value

        int16_t temp_val(); //returns the onboard temperature

        int8_t device_id();
        int8_t info();
        bool status_1();
        void status_2();

        int16_t M_getX();
        int16_t M_getY();
        int16_t M_getZ();
    private:
        MPU_9255() :
                i2c2_device(I2CAddr_MPU_9255) //define this address!!
        {

        }

        friend class SingletonTemplate<MPU_9255> ;

        //define the address from the datasheet
        static const unsigned char RESET_PWR_MNT = 0x01;
        static const unsigned char RESET_WHO_AM_I = 0x73; //page 9

        typedef enum
        {
            RESET = 0x00,
            SELF_TEST_X_ACCEL = 0x0D,
            SELF_TEST_Y_ACCEL = 0x0E,
            SELF_TEST_Z_ACCEL = 0x0F,
            SELF_TEST_X_GYRO = 0x00,
            SELF_TEST_Y_GYRO = 0x01,
            SELF_TEST_Z_GYRO = 0x02, //Table - page 7
            ACCEL_CONFIG = 0x1C,
            GYRO_CONFIG = 0X1B,

            //Accelerometer Values
            ACCEL_XOUT_H = 0x3B,
            ACCEL_XOUT_L = 0x3C,
            ACCEL_YOUT_H = 0x3D,
            ACCEL_YOUT_L = 0x3E,
            ACCEL_ZOUT_H = 0x3F,
            ACCEL_ZOUT_L = 0x40,

            //Gyro Values
            GYRO_XOUT_H = 0x43,
            GYRO_XOUT_L = 0x44,
            GYRO_YOUT_H = 0x45,
            GYRO_YOUT_L = 0x46,
            GYRO_ZOUT_H = 0x47,
            GYRO_ZOUT_L = 0x48,

            //Temperature Sensor
            TEMP_OUT_H = 0X41,
            TEMP_OUT_L = 0X42,

            WHO_AM_I = 0x75,

            //Magnetometer
            DEVICE_ID = 0x00,
            INFO = 0x01,
            ST1 = 0x02,
            ST2 = 0x09,

            HXL = 0x03,
            HXH = 0x04,
            HYL = 0x05,
            HYH = 0x06,
            HZL = 0x07,
            HZH = 0x08,
        }__attribute__ ((packed)) RegisterMap;
};

#endif /* MPU_9255_HPP_ */
