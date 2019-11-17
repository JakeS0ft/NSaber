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
 * SoundTypes.h
 *
 *  Created on: Sep 13, 2019
 *      Author: JakeSoft
 */

#ifndef SOUNDTYPES_H_
#define SOUNDTYPES_H_

#define MAX_SWING_SOUNDS      (16)
#define MAX_CLASH_SOUNDS      (16)
#define MAX_BLASTER_SOUNDS    (16)
#define MAX_POWERUP_SOUNDS    (4)
#define MAX_POWERDOWN_SOUNDS  (4)
#define MAX_FORCE_SOUNDS      (4)
#define MAX_LOW_SWING_SOUNDS  (16)
#define MAX_HIGH_SWING_SOUNDS (16)

namespace SoundTypes
{

enum ESoundTypes
{
	eeFontIdSnd,
	eeBootSnd,
	eePowerUpSnd,
	eeSwingSnd,
	eeClashSnd,
	eeBlasterSnd,
	eeLockupSnd,
	eeForceSnd,
	eePowerDownSnd,
	eeHumSnd,
	eeMenuSoundSnd,
	eeLowSwingSnd,
	eeHighSwingSnd,
	eeMaxSoundTypes
};

enum EChannelMapType
{
	eeClassicPlecter,
	eeTrueSwing,
	eeClassicNEC
};

//Container class to define sound files available from a sound font
struct tSoundFontProperties
{
	//Base directory of font
	char* mBaseDir;

	/**********************************************
	 * Basic Sounds.
	 * All fonts should have at least one of these.
	 **********************************************/
	//Font Identifier
	char* mFontIdFile;
	//Boot sound
	char* mBootFile;
	//Lockup
	char* mLockupFile;
	//Hum
	char* mHumFile;

	//Powerup sounds
	char* maPowerUpFile[MAX_POWERUP_SOUNDS];
	short mNumPowerUpFiles = 0;

	//Swing sounds
	char* maSwingFile[MAX_SWING_SOUNDS];
	short mNumSwingFiles = 0;

	//Clash sounds
	char* maClashFile[MAX_CLASH_SOUNDS];
	short mNumClashFiles = 0;

	//Blaster sounds
	char* maBlasterFile[MAX_BLASTER_SOUNDS];
	short mNumBlasterFiles = 0;

	//Power down sounds
	char* maPowerDownFile[MAX_POWERDOWN_SOUNDS];
	short mNumPowerDownFiles = 0;


	/**********************************
	 * Advanced sounds.
	 * Some fonts may not have these.
	 *********************************/
	//Special "Force" sounds
	char* maForceFile[MAX_FORCE_SOUNDS];
	short mNumForceFiles = 0;

	//Dynamic swing sounds
	char* maLowSwingFile[MAX_LOW_SWING_SOUNDS];
	short mNumLowSwingFiles = 0;

	char* maHighSwingFile[MAX_HIGH_SWING_SOUNDS];
	short mNumHighSwingFiles = 0;

};

}

#endif /* SOUNDTYPES_H_ */
