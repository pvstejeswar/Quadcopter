/*
 * Author: Dheeraj Dake
 * Description: Definitions for the Acceleration sensor of MPU 9255
 */

#include <stdint.h>
#include <io.hpp>
#include <stdio.h>
#include <utilities.h>

bool MPU_9255::init()
{

#if dheeraj
    //Configuring the accelerometer, GYRO sensitivity
    const unsigned char accel_sens = (1 << 4) | (1 << 3);//Reg 28: 00010000 = 8g - 16 in decimal
    const unsigned char gyro_sens = (1 << 4);//Reg 28: 00010000 = 1000 dps - 16 in decimal

    //write that value to reg 28
    writeReg(ACCEL_CONFIG, accel_sens);
    writeReg(GYRO_CONFIG, gyro_sens);

    //checking whether we are accessing our device
    const char whoAmIReg = readReg(WHO_AM_I);

    //if this gives 0x73 we are good
    return (0x71 == whoAmIReg);//Don't know how it is 0x71. I checked by passing the reg address.
#endif

    const char who_am_i = readReg(WHO_AM_I);

    if (who_am_i == 0x71) // WHO_AM_I should always be 0x68
    {

        /*Reset the device*/
        IMU.reset_9255();


        /*Calibrate for bias calculation*/
        float gyroBias = 0, accelBias = 0;
        IMU.calibrate_9255(&gyroBias, &accelBias);


        //uint8_t Ascale = AFS_2G;     // AFS_2G, AFS_4G, AFS_8G, AFS_16G
        //uint8_t Gscale = GFS_250DPS; // GFS_250DPS, GFS_500DPS, GFS_1000DPS, GFS_2000DPS
        //uint8_t Mscale = MFS_16BITS; // MFS_14BITS or MFS_16BITS, 14-bit or 16-bit magnetometer resolution
        //uint8_t Mmode = 0x06;        // Either 8 Hz 0x02) or 100 Hz (0x06) magnetometer data ODR

        // Initialize MPU9250 device
        // wake up device
        writeReg(PWR_MGMT_1, 0x00); // Clear sleep mode bit (6), enable all sensors
        delay_ms(100); // Delay 100 ms for PLL to get established on x-axis gyro; should check for PLL ready interrupt

        // get stable time source
        writeReg(PWR_MGMT_1, 0x01);  // Set clock source to be PLL with x-axis gyroscope reference, bits 2:0 = 001

        // Configure Gyro and Accelerometer
        // Disable FSYNC and set accelerometer and gyro bandwidth to 44 and 42 Hz, respectively;
        // DLPF_CFG = bits 2:0 = 010; this sets the sample rate at 1 kHz for both
        // Maximum delay is 4.9 ms which is just over a 200 Hz maximum rate
        writeReg(CONFIG, 0x03);

        //TODO:VISHNU: DO YOU GUYS Understand anyone below register config ?????

        // Set sample rate = gyroscope output rate/(1 + SMPLRT_DIV)
        writeReg(SMPLRT_DIV, 0x04);  // Use a 200 Hz rate; the same rate set in CONFIG above

        // Set gyroscope full scale range
        // Range selects FS_SEL and AFS_SEL are 0 - 3, so 2-bit values are left-shifted into positions 4:3
        uint8_t c = readReg(GYRO_CONFIG); // get current GYRO_CONFIG register value
        // c = c & ~0xE0; // Clear self-test bits [7:5]
        c = c & ~0x02; // Clear Fchoice bits [1:0]
        c = c & ~0x18; // Clear AFS bits [4:3]
        c = c | AFS_2G << 3; // Set full scale range for the gyro
        // c =| 0x00; // Set Fchoice for the gyro to 11 by writing its inverse to bits 1:0 of GYRO_CONFIG
        writeReg(GYRO_CONFIG, c ); // Write new GYRO_CONFIG value to register

        // Set accelerometer full-scale range configuration
        c = readReg(ACCEL_CONFIG); // get current ACCEL_CONFIG register value
        // c = c & ~0xE0; // Clear self-test bits [7:5]
        c = c & ~0x18;  // Clear AFS bits [4:3]
        c = c | AFS_2G << 3; // Set full scale range for the accelerometer
        writeReg(ACCEL_CONFIG, c); // Write new ACCEL_CONFIG register value

        // Set accelerometer sample rate configuration
        // It is possible to get a 4 kHz sample rate from the accelerometer by choosing 1 for
        // accel_fchoice_b bit [3]; in this case the bandwidth is 1.13 kHz
        c = readReg(ACCEL_CONFIG2); // get current ACCEL_CONFIG2 register value
        c = c & ~0x0F; // Clear accel_fchoice_b (bit 3) and A_DLPFG (bits [2:0])
        c = c | 0x03;  // Set accelerometer rate to 1 kHz and bandwidth to 41 Hz
        writeReg(ACCEL_CONFIG2, c); // Write new ACCEL_CONFIG2 register value

        // The accelerometer, gyro, and thermometer are set to 1 kHz sample rates,
        // but all these rates are further reduced by a factor of 5 to 200 Hz because of the SMPLRT_DIV setting

        // Configure Interrupts and Bypass Enable
        // Set interrupt pin active high, push-pull, and clear on read of INT_STATUS, enable I2C_BYPASS_EN so additional chips
        // can join the I2C bus and all can be controlled by the Arduino as master

        //writeReg(INT_PIN_CFG, 0x22);
        //writeReg(INT_ENABLE, 0x01);  // Enable data ready (bit 0) interrupt
        return true;
    }

    return false;
 }

int8_t MPU_9255::validate()
{
    return readReg(WHO_AM_I);
}

//Accelerometer functions
int16_t MPU_9255::AS_getX()
{
    return (int16_t) get16BitRegister(ACCEL_XOUT_H) / 16;
}

int16_t MPU_9255::AS_getY()
{
    return (int16_t) get16BitRegister(ACCEL_YOUT_H) / 16;
}

int16_t MPU_9255::AS_getZ()
{
    return (int16_t) get16BitRegister(ACCEL_ZOUT_H) / 16;
}

//Gyro functions
int16_t MPU_9255::GS_getX()
{
    return (int16_t) get16BitRegister(GYRO_XOUT_H) / 16;
}

int16_t MPU_9255::GS_getY()
{
    return (int16_t) get16BitRegister(GYRO_YOUT_H) / 16;
}

int16_t MPU_9255::GS_getZ()
{
    return (int16_t) get16BitRegister(GYRO_ZOUT_H) / 16;
}

int16_t MPU_9255::temp_val()
{
    return (int16_t) get16BitRegister(TEMP_OUT_H) / 16;
}

//Magnetometer functions
int8_t MPU_9255::device_id()
{
    return readReg(DEVICE_ID); //returns the device id 0x48
}

int8_t MPU_9255::info()
{
    return readReg(INFO); //returns the device id 0x48
}

bool MPU_9255::status_1()
{
    return (0x01 == readReg(ST1)); //if data is available to read its 0x01, else 0x00(normal)
}

int16_t MPU_9255::M_getX()
{
    return (int16_t) get16BitRegister_I(HXL) / 16; //LSB is in a lower address than MSB!
}

int16_t MPU_9255::M_getY()
{
    return (int16_t) get16BitRegister_I(HYL) / 16;
}

int16_t MPU_9255::M_getZ()
{
    return (int16_t) get16BitRegister_I(HZL) / 16;
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
    switch (readReg(ST2))
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

void MPU_9255::reset_9255()
{
    writeReg(PWR_MGMT_1, 0x80);
}

void MPU_9255::calibrate_9255(float *dest1, float *dest2)
{
    uint16_t fifo_count, packet_count;
    int32_t gyro_bias[3] =
    { 0, 0, 0 }, accel_bias[3] =
    { 0, 0, 0 };

    /*Activate the Accel and gyro*/
    writeReg(PWR_MGMT_1, 0x01); //reset internal registers and restore the default settings
    writeReg(PWR_MGMT_2, 0x00); //Activate the GYro and Accel

    /*Deactivate everything for bias calculation*/
    writeReg(INT_ENABLE, 0x00);
    writeReg(FIFO_ENABLE, 0x00);
    writeReg(PWR_MGMT_1, 0x00);
    writeReg(I2C_MST_CTRL, 0x00);
    writeReg(USER_CTRL, 0x00);
    writeReg(USER_CTRL, 0x0C);

    /*Bias calculation mode*/
    writeReg(CONFIG, 0x01);
    writeReg(SMPLRT_DIV, 0x00);
    writeReg(GYRO_CONFIG, 0x00);
    writeReg(ACCEL_CONFIG, 0x00);

    /*Setting sensitivity*/
    uint16_t gyrosensitivity = 131; // = 131 LSB/degrees/sec
    uint16_t accelsensitivity = 16384; // = 16384 LSB/g

    /*Capture bias in FIFO*/
    writeReg(USER_CTRL, 0x40);
    writeReg(FIFO_ENABLE, 0x78);

    /*Turn off FIFO sensor*/
    writeReg(FIFO_ENABLE, 0x00);
    fifo_count = get16BitRegister(FIFO_COUNTH);
    packet_count = fifo_count / 12;

    unsigned char data[12];
    /**/
    for (int a = 0; a < packet_count; a++)
    {
        int16_t accel_temp[3] =
        { 0, 0, 0 }, gyro_temp[3] =
        { 0, 0, 0 };
        readRegisters(FIFO_R_W, &data[0], sizeof(data));
        accel_temp[0] = (int16_t) (((int16_t) data[0] << 8) | data[1]); // Form signed 16-bit integer for each sample in FIFO
        accel_temp[1] = (int16_t) (((int16_t) data[2] << 8) | data[3]);
        accel_temp[2] = (int16_t) (((int16_t) data[4] << 8) | data[5]);
        gyro_temp[0] = (int16_t) (((int16_t) data[6] << 8) | data[7]);
        gyro_temp[1] = (int16_t) (((int16_t) data[8] << 8) | data[9]);
        gyro_temp[2] = (int16_t) (((int16_t) data[10] << 8) | data[11]);

        accel_bias[0] += (int32_t) accel_temp[0]; // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
        accel_bias[1] += (int32_t) accel_temp[1];
        accel_bias[2] += (int32_t) accel_temp[2];
        gyro_bias[0] += (int32_t) gyro_temp[0];
        gyro_bias[1] += (int32_t) gyro_temp[1];
        gyro_bias[2] += (int32_t) gyro_temp[2];
    }

    accel_bias[0] /= (int32_t) packet_count; // Normalize sums to get average count biases
    accel_bias[1] /= (int32_t) packet_count;
    accel_bias[2] /= (int32_t) packet_count;
    gyro_bias[0] /= (int32_t) packet_count;
    gyro_bias[1] /= (int32_t) packet_count;
    gyro_bias[2] /= (int32_t) packet_count;

    if (accel_bias[2] > 0L)
    {
        accel_bias[2] -= (int32_t) accelsensitivity;
    } // Remove gravity from the z-axis accelerometer bias calculation
    else
    {
        accel_bias[2] += (int32_t) accelsensitivity;
    }

    // Construct the gyro biases for push to the hardware gyro bias registers, which are reset to zero upon device startup
    data[0] = (-gyro_bias[0] / 4 >> 8) & 0xFF; // Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input format
    data[1] = (-gyro_bias[0] / 4) & 0xFF; // Biases are additive, so change sign on calculated average gyro biases
    data[2] = (-gyro_bias[1] / 4 >> 8) & 0xFF;
    data[3] = (-gyro_bias[1] / 4) & 0xFF;
    data[4] = (-gyro_bias[2] / 4 >> 8) & 0xFF;
    data[5] = (-gyro_bias[2] / 4) & 0xFF;

    /// Push gyro biases to hardware registers
    /*  writeReg(XG_OFFSET_H, data[0]);
     writeReg(XG_OFFSET_L, data[1]);
     writeReg(YG_OFFSET_H, data[2]);
     writeReg(YG_OFFSET_L, data[3]);
     writeReg(ZG_OFFSET_H, data[4]);
     writeReg(ZG_OFFSET_L, data[5]);
     */
    dest1[0] = (float) gyro_bias[0] / (float) gyrosensitivity; // construct gyro bias in deg/s for later manual subtraction
    dest1[1] = (float) gyro_bias[1] / (float) gyrosensitivity;
    dest1[2] = (float) gyro_bias[2] / (float) gyrosensitivity;

    // Construct the accelerometer biases for push to the hardware accelerometer bias registers. These registers contain
    // factory trim values which must be added to the calculated accelerometer biases; on boot up these registers will hold
    // non-zero values. In addition, bit 0 of the lower byte must be preserved since it is used for temperature
    // compensation calculations. Accelerometer bias registers expect bias input as 2048 LSB per g, so that
    // the accelerometer biases calculated above must be divided by 8.

    int32_t accel_bias_reg[3] =
    { 0, 0, 0 }; // A place to hold the factory accelerometer trim biases
    readRegisters(XA_OFFSET_H, &data[0],2); // Read factory accelerometer trim values
    accel_bias_reg[0] = (int16_t) ((int16_t) data[0] << 8) | data[1];
    readRegisters(YA_OFFSET_H, &data[0],2);
    accel_bias_reg[1] = (int16_t) ((int16_t) data[0] << 8) | data[1];
    readRegisters(ZA_OFFSET_H, &data[0],2);
    accel_bias_reg[2] = (int16_t) ((int16_t) data[0] << 8) | data[1];

    uint32_t mask = 1uL; // Define mask for temperature compensation bit 0 of lower byte of accelerometer bias registers
    uint8_t mask_bit[3] =
    { 0, 0, 0 }; // Define array to hold mask bit for each accelerometer bias axis

    for (int i = 0; i < 3; i++)
    {
        if (accel_bias_reg[i] & mask)
            mask_bit[i] = 0x01; // If temperature compensation bit is set, record that fact in mask_bit
    }

    // Construct total accelerometer bias, including calculated average accelerometer bias from above
    accel_bias_reg[0] -= (accel_bias[0] / 8); // Subtract calculated averaged accelerometer bias scaled to 2048 LSB/g (16 g full scale)
    accel_bias_reg[1] -= (accel_bias[1] / 8);
    accel_bias_reg[2] -= (accel_bias[2] / 8);

    data[0] = (accel_bias_reg[0] >> 8) & 0xFF;
    data[1] = (accel_bias_reg[0]) & 0xFF;
    data[1] = data[1] | mask_bit[0]; // preserve temperature compensation bit when writing back to accelerometer bias registers
    data[2] = (accel_bias_reg[1] >> 8) & 0xFF;
    data[3] = (accel_bias_reg[1]) & 0xFF;
    data[3] = data[3] | mask_bit[1]; // preserve temperature compensation bit when writing back to accelerometer bias registers
    data[4] = (accel_bias_reg[2] >> 8) & 0xFF;
    data[5] = (accel_bias_reg[2]) & 0xFF;
    data[5] = data[5] | mask_bit[2]; // preserve temperature compensation bit when writing back to accelerometer bias registers

    // Apparently this is not working for the acceleration biases in the MPU-9250
    // Are we handling the temperature correction bit properly?
    // Push accelerometer biases to hardware registers
    /*  writeReg(XA_OFFSET_H, data[0]);
     writeReg(XA_OFFSET_L, data[1]);
     writeReg(YA_OFFSET_H, data[2]);
     writeReg(YA_OFFSET_L, data[3]);
     writeReg(ZA_OFFSET_H, data[4]);
     writeReg(ZA_OFFSET_L, data[5]);
     */
    // Output scaled accelerometer biases for manual subtraction in the main program
    dest2[0] = (float) accel_bias[0] / (float) accelsensitivity;
    dest2[1] = (float) accel_bias[1] / (float) accelsensitivity;
    dest2[2] = (float) accel_bias[2] / (float) accelsensitivity;

    /*Completed writing the calibration function---- check with the main file from Kris git and write accordingly */

}
