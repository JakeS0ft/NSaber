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

#ifndef MPU6050LITEMOTIONMANAGER_H_
#define MPU6050LITEMOTIONMANAGER_H_

#include "AMotionManager.h"
#include <Arduino.h>

#define MPU6050_CLOCK_INTERNAL          0x00
#define MPU6050_CLOCK_PLL_XGYRO         0x01
#define MPU6050_CLOCK_PLL_YGYRO         0x02
#define MPU6050_CLOCK_PLL_ZGYRO         0x03
#define MPU6050_CLOCK_PLL_EXT32K        0x04
#define MPU6050_CLOCK_PLL_EXT19M        0x05
#define MPU6050_CLOCK_KEEP_RESET        0x07

#define GYRO_FS_RANGE250  (0b00000000) //250 deg/sec
#define GYRO_FS_RANGE500  (0b00001000) //500 deg/sec
#define GYRO_FS_RANGE1000 (0b00010000) //1000 deg/sec
#define GYRO_FS_RANGE2000 (0b00011000) //2000 deg/sec

#define MPU6050_ACCEL_FS_2          0x00
#define MPU6050_ACCEL_FS_4          0x01
#define MPU6050_ACCEL_FS_8          0x02
#define MPU6050_ACCEL_FS_16         0x03

#define MPU6050_RA_INT_ENABLE       0x38
#define MPU6050_RA_INT_STATUS       0x3A

#define MPU6050_RA_GYRO_CONFIG      0x1B
#define MPU6050_RA_ACCEL_CONFIG     0x1C

#define MPU6050_RA_PWR_MGMT_1       0x6B
#define MPU6050_RA_PWR_MGMT_2       0x6C


//Container to define motion tolerance data
struct MPU6050LiteTolData
{
	//Tolerance for large swings
	unsigned int mSwingLarge;
	//Tolerance for medium swings
	unsigned int mSwingMedium;
	//Tolerance for small swings
	unsigned int mSwingSmall;
	//Tolerance for clashes
	unsigned int mClash;
	//Tolerance for twist
	unsigned int mTwist;
};

/**
 * Light weight version of the MPU6050 Motion manager. Primary design
 * consideration for this class is to keep compiled size down while
 * still offering serviceable performance.
 *
 * Note: This class assumes that the Y-axis of the MPU6050 is parallel with
 * the saber's blade.
 */
class Mpu6050LiteMotionManager : public AMotionManager
{
public:

	/**
	 * Constructor.
	 * Args: apTolData - Swing tolerance data
	 */
	Mpu6050LiteMotionManager(MPU6050LiteTolData* apTolData);

	virtual ~Mpu6050LiteMotionManager();

	virtual void Init();

	virtual bool IsSwing();

	virtual bool IsClash();

	virtual bool IsTwist();

	virtual void Update();

	virtual EMagnitudes GetSwingMagnitude();

	virtual void Sleep();
protected :
	//Container for time-stamped axis data
	typedef struct
	{
		//X-axis reading
		int16_t mnX;
		//Y-axis reading
		int16_t mnY;
		//Z-axis reading
		int16_t mnZ;
		//When data was captured
		unsigned long mTimeStamp;
	}AxisData;

	/**
	 * Check for swing event.
	 */
	virtual bool SwingDetect();

	/**
	 * Check for clash event.
	 */
	bool ClashDetect();

	/**
	 * Sends I2C command to the MPU6050.
	 * Args: aAddr - Register address to write to
	 *       aByte - Value to write
	 */
	void I2CWrite(uint8_t aAddr, uint8_t aByte);

	// Magnitude of last detected swing
	EMagnitudes mSwingMagnitude;

	// Tolerance data
	MPU6050LiteTolData* mpTolData;

	// Previous accelerometer sensor readings
	AxisData mLastAcclReading;
	// Most recent accelerometer sensor reading
	AxisData mCurAcclReading;
	// Most recent gyro sensor reading
	AxisData mCurGyroReading;

	// Last time we checked for swing events
	unsigned long mLastSwingDetectTime;

	//Flags if swing was detected in last update cycle
	bool mIsSwing;

	//Flags if clash was detected in last update cycle
	bool mIsClash;

	//Flags if a twist gesture was detected last update cycle
	bool mIsTwist;

	// I2C address of the MPU-6050
	const int mMpuAddr=0x68;

	//Flag to keep track of if we started wire for I2C comms already or not
	bool mWireStarted;
};

#endif /* MPU6050LITEMOTIONMANAGER_H_ */
