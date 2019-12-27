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
 * NSaber.h
 *
 *  Created on: Nov 14, 2019
 *      Author: JakeSoft
 */

#ifndef NSABER_H_
#define NSABER_H_

#include <nRF52Audio.h> //NRF52 Library needed for sound playback
#include <SD.h>         //SD needed for SD card reader

#include "Sound/NECSoundManager.h"
#include "Sound/DynamicNECSoundManager.h"

#include "FileUtils.h"
#include "AMotionReactive.h"

#include "Motion/AMotionManager.h"
#include "Motion/Mpu6050LiteMotionManager.h"
#include "Motion/Mpu6050AdvancedMotionManager.h"

#endif /* NSABER_H_ */
