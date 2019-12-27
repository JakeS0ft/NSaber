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
 * Mpu6050AdvancedMotionManager.cpp
 *
 *  Created on: Dec 27, 2019
 *      Author: JakeSoft
 */

#include "Motion/Mpu6050AdvancedMotionManager.h"

Mpu6050AdvancedMotionManager::Mpu6050AdvancedMotionManager(MPU6050AdvancedTolData* apTolData)
:Mpu6050LiteMotionManager(apTolData)
{

}

void Mpu6050AdvancedMotionManager::GetRawAcclData
	(uint16_t& arAclX, uint16_t& arAclY, uint16_t& arAclZ)
{
	arAclX = mCurAcclReading.mnX;
	arAclY = mCurAcclReading.mnY;
	arAclZ = mCurAcclReading.mnZ;
}


void Mpu6050AdvancedMotionManager::GetRawGyroData
	(uint16_t& arGyX, uint16_t& arGyY, uint16_t& arGyZ)
{
	arGyX = mCurGyroReading.mnX;
	arGyY = mCurGyroReading.mnY;
	arGyZ = mCurGyroReading.mnZ;
}

