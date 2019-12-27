/******************************************************************************
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 ******************************************************************************/
/*
 * Mpu6050LiteMotionManager.h
 *
 *  Created on: Dec 27, 2019
 *      Author: JakeSoft
 */


#include "Motion/Mpu6050LiteMotionManager.h"
#include <Arduino.h>

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include <Wire.h>
#endif


Mpu6050LiteMotionManager::Mpu6050LiteMotionManager(MPU6050LiteTolData* apTolData)
{
	mpTolData = apTolData;
	mIsClash = false;
	mIsSwing = false;
	mIsTwist = false;
	mSwingMagnitude = eeSmall;
	mLastSwingDetectTime = 0;
	mWireStarted = false;
}

Mpu6050LiteMotionManager::~Mpu6050LiteMotionManager()
{
	//Do nothing
}

void Mpu6050LiteMotionManager::Init()
{
	//Start the I2C session, but only if it's not already open
	if(!mWireStarted)
	{
		Wire.begin();
	}

	I2CWrite(MPU6050_RA_PWR_MGMT_1, 0); //Wake up the MPU
	I2CWrite(MPU6050_RA_GYRO_CONFIG, GYRO_FS_RANGE1000); //Set gyro full scale range to +/- 1000 deg/sec
	I2CWrite(MPU6050_RA_ACCEL_CONFIG, MPU6050_ACCEL_FS_2); //Set accl range scale

	//Set up clash detection
	I2CWrite(0x38, 0b00100000); //Enable motion detection interrupt status
	I2CWrite(0x1F, (uint8_t)mpTolData->mClash); //Set motion detection threshold for interrupt
	I2CWrite(0x20, 2);  //Set motion detection duration samples

	mWireStarted = true;
}

EMagnitudes Mpu6050LiteMotionManager::GetSwingMagnitude()
{
	return mSwingMagnitude;
}

bool Mpu6050LiteMotionManager::IsSwing()
{
	return mIsSwing;
}

bool Mpu6050LiteMotionManager::IsClash()
{
	return mIsClash;
}

bool Mpu6050LiteMotionManager::IsTwist()
{
	return mIsTwist;
}

void Mpu6050LiteMotionManager::Update()
{
	unsigned long lNow = millis();

	//Don't update more than once per 5 milliseconds
	if(lNow - mCurAcclReading.mTimeStamp < 5)
	{
		return;
	}

	Wire.beginTransmission(mMpuAddr);
	Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
	Wire.endTransmission(false);
	Wire.requestFrom(mMpuAddr,14,true);  // request a total of 6 registers

	// Store values from last cycle
	mLastAcclReading = mCurAcclReading;

	// Update with new accelerometer data
	mCurAcclReading.mnX = Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
	mCurAcclReading.mnY = Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
	mCurAcclReading.mnZ = Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
	//Snag temperature data, (but we won't use it)
	int16_t lTemp = Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
	//Update Gyro data
	mCurGyroReading.mnX =Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
	mCurGyroReading.mnY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
	mCurGyroReading.mnZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

	//Chop off low order bits so our readings don't jiggle and wiggle like Jell-O
	int lChopBits = 6;
	mCurAcclReading.mnX = (mCurAcclReading.mnX >> lChopBits);
	mCurAcclReading.mnY = (mCurAcclReading.mnY >> lChopBits);
	mCurAcclReading.mnZ = (mCurAcclReading.mnZ >> lChopBits);
	mCurGyroReading.mnX = (mCurGyroReading.mnX >> lChopBits);
	mCurGyroReading.mnY = (mCurGyroReading.mnY >> lChopBits);
	mCurGyroReading.mnZ = (mCurGyroReading.mnZ >> lChopBits);

	//Time-stamp the results
	mCurAcclReading.mTimeStamp = lNow;
	mCurGyroReading.mTimeStamp = lNow;

	mIsClash = ClashDetect();

	//Check for swings
	if(!mIsClash && lNow - mLastSwingDetectTime >= 5)
	{
		mLastSwingDetectTime = lNow;
		mIsTwist = false; //Reset the twist flag, may be set to true by SwingDetect()
		mIsSwing = SwingDetect();
	}
}

bool Mpu6050LiteMotionManager::SwingDetect()
{
	bool lSwingDetected = false;

	uint16_t lRotationMagnitude = max(abs(mCurGyroReading.mnX),abs(mCurGyroReading.mnZ));
	uint16_t lTwistMagnitude = abs(mCurGyroReading.mnY);

	//Detect a swing
	//if(lRotationMagnitude >= mpTolData->mSwingSmall && lRotationMagnitude > lTwistMagnitude)
	if(lRotationMagnitude >= mpTolData->mSwingSmall && lRotationMagnitude > lTwistMagnitude)
	{
		lSwingDetected = true;

		mSwingMagnitude = eeSmall;

		if(lRotationMagnitude >= mpTolData->mSwingMedium)
		{
			mSwingMagnitude = eeMedium;
		}

		if(lRotationMagnitude >= mpTolData->mSwingLarge)
		{
			mSwingMagnitude = eeLarge;
		}
	}
	else if(lTwistMagnitude > mpTolData->mTwist)
	{
		mIsTwist = true;
	}

	return lSwingDetected;
}

bool Mpu6050LiteMotionManager::ClashDetect()
{
	bool lClash = false;

	//Request Init status byte
	Wire.beginTransmission(mMpuAddr);
	Wire.write(MPU6050_RA_INT_STATUS);
	Wire.endTransmission(false);
	Wire.requestFrom(mMpuAddr,1,true); //Request the interrupt status byte
	uint8_t lIntStatus = Wire.read();
	//Chop off the DATA_READY bit, we don't care
	lIntStatus = lIntStatus >> 1;
	lIntStatus = lIntStatus << 1;
	//Check the Interrupt status
	if(0b01000000 == lIntStatus) //7th bit indicates motion detect interrupt was triggered
	{
		lClash = true;
	}
//	if(0b01000000 & lIntStatus) //7th bit indicates motion detect interrupt was triggered
//	{
//		lClash = true;
//	}

	return lClash;
}

void Mpu6050LiteMotionManager::I2CWrite(uint8_t aAddr, uint8_t aByte)
{
	Wire.beginTransmission(mMpuAddr);
	Wire.write(aAddr);  // Register address to write
	Wire.write(aByte);     // Value to write
	Wire.endTransmission(true);
}

void Mpu6050LiteMotionManager::Sleep()
{
	//Setting the 6th bit of the power management register enables sleep
	I2CWrite(MPU6050_RA_PWR_MGMT_1, 0b00100000);
}

