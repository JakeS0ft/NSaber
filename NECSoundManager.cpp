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
 * NECSoundManager.cpp
 *
 *  Created on: Nov 8, 2019
 *      Author: JakeSoft
 */


#include "Sound/NECSoundManager.h"

#include "FileUtils.h"

static const char* sFontFileStr = "font";
static const char* sBootFileStr = "boot";
static const char* sBlasterFileStr = "blst";
static const char* sClashFileStr = "clsh";
static const char* sForceFileStr = "force";
static const char* sHumFileStr = "hum";
static const char* sLockupFileStr = "lock";
static const char* sPowerUpFileStr = "out";
static const char* sPowerDownFileStr = "in";
static const char* sSwingFileStr = "swng";

NECSoundManager::NECSoundManager(I2SWavPlayer* apWavPlayer)
{
	mpWavPlayer = apWavPlayer;

	mpEffectSound = nullptr;
	mpHumSound = nullptr;

	for(int lIdx = 0; lIdx < SoundTypes::eeMaxSoundTypes; lIdx++)
	{
		maSoundCounts[lIdx] = 0;
	}

	mEffectSoundType = SoundTypes::eeMaxSoundTypes;
	mHumChannel = 0;
	mEffectChannel = 1;

	mFontBaseNameStr = "necfont";
}

NECSoundManager::~NECSoundManager()
{
	mpWavPlayer->SetWavFile(nullptr, mEffectChannel);
	mpWavPlayer->SetWavFile(nullptr, mHumChannel);

	if(nullptr != mpEffectSound)
	{
		delete mpEffectSound;
	}

	if(nullptr != mpHumSound)
	{
		delete mpHumSound;
	}
}

/**
 * Initialize. Do anything that should happen on startup.
 */
void NECSoundManager::Init()
{

}

/**
 * Play a specific sound. For example, a call to play the second
 * clash sound within a sound font would be:
 *  PlaySound(eeClash, 1);
 * Remember that indexes are assumed to be base-zero, meaning that zero is
 * the first element.
 *
 * Args:
 *  aSoundType - Type of sound to play
 *  aIndex - Index of the sound to play.
 * Returns:
 *  TRUE if successful, FALSE otherwise
 */
bool NECSoundManager::PlaySound(SoundTypes::ESoundTypes aSoundType, uint16_t aIndex)
{
	char laNewFileName[MAX_FILE_NAME_SIZE];
	GenerateFileName(aSoundType, laNewFileName, aIndex);

	Serial.print("Trying to play ");Serial.println((const char*)laNewFileName);
	//SDWavFile* lpNewSound = new SDWavFile((const char*)laNewFileName);

	PitchShiftSDWavFile* lpNewSound = new PitchShiftSDWavFile((const char*)laNewFileName);

	ISDWavFile* lpDeletePtr = nullptr;;

	//To improve performance, rapid requests for the same effect sound
	//simply reset same sound to play again to avoid time-expensive seeking
	//on the SD card
	if(mEffectSoundType == aSoundType
	   && IsHighPerformanceSoundType(aSoundType)
	   && nullptr != mpEffectSound
	   && false == mpEffectSound->IsEnded())
	{
		mpEffectSound->SeekStartOfData();
	}
	else if(SoundTypes::eeHumSnd == aSoundType)
	{
		lpDeletePtr = mpHumSound;
		mpHumSound = lpNewSound;
		mpHumSound->SetLooping(true);

		mpWavPlayer->SetWavFile(mpHumSound, mHumChannel);
	}
	else
	{
		mpEffectSound->Pause();

		lpDeletePtr = mpEffectSound;
		mpEffectSound = lpNewSound;

		if(SoundTypes::eeLockupSnd == aSoundType)
		{
			mpEffectSound->SetLooping(true);
		}
		else
		{
			mpEffectSound->SetLooping(false);
		}

		mpWavPlayer->SetWavFile(mpEffectSound, mEffectChannel);
		mEffectSoundType = aSoundType;
	}

	if(nullptr != lpDeletePtr)
	{
		lpDeletePtr->Close();
		delete lpDeletePtr;
	}

	return true;
}

/**
 * Play a random sound within the selected category. Subclasses should
 * implement a method that can play a random sound within the selected
 * category on demand.
 *
 * Args:
 *  aSoundType - Type of sound to play
 * Returns:
 *  TRUE if successful, FALSE otherwise
 */
bool NECSoundManager::PlayRandomSound(SoundTypes::ESoundTypes aSoundType)
{
	int lRandomSoundIdx = random(0, maSoundCounts[aSoundType]-1);

	return PlaySound(aSoundType, lRandomSoundIdx);
}

/**
 * Change the current font. This method is for support of multi-font saber
 * sound. Subclasses should implement code that will cause subsequent calls
 * to PlaySound() and PlayRandomSound() to play sounds from alternate fonts
 * specified by the aFontIndex argument of this method.
 * Args:
 *   aFontIndex - Index of the font to use.
 */
void NECSoundManager::SetFont(unsigned char aFontIndex)
{
	//const char* lDirBaseStr = "2205/ifont";
	const char* lDirBaseStr = mFontBaseNameStr.c_str();

	int lFontIdxAppend = aFontIndex + 1;
	char laIdxStrBuf[3];
	itoa(lFontIdxAppend, laIdxStrBuf, 10);

	//Figure out new font base directory
	memset(maFontBaseDir, 0, 10);
	strcat(maFontBaseDir, lDirBaseStr);
	strcat(maFontBaseDir, (const char*)laIdxStrBuf);

	//Count sounds of each type in the new font
	CountSoundsInFont();

	//Figure out hum file path
	char laHumFileName[MAX_FILE_NAME_SIZE];
	GenerateFileName(SoundTypes::eeHumSnd, laHumFileName, 0);
	mpHumSound = new PitchShiftSDWavFile((const char*)laHumFileName);
	mpHumSound->SetLooping(true);

	char laFontIdFileName[MAX_FILE_NAME_SIZE];
	GenerateFileName(SoundTypes::eeFontIdSnd, laFontIdFileName);
	mpEffectSound = new PitchShiftSDWavFile((const char*)laFontIdFileName);
}

/**
 * Call this in a loop to keep sound playing.
 */
bool NECSoundManager::ContinuePlay(bool aFillMixingBuffer)
{
	if(this->mpWavPlayer->IsEnded())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void NECSoundManager::SetMasterVolume(int aVolume)
{
	if(nullptr != mpWavPlayer)
	{
		float lVol = (float)aVolume / 100.0;
		mpWavPlayer->SetVolume(lVol);
	}
}

void NECSoundManager::SetFontDirNameBase(const char* aBaseStr)
{
	mFontBaseNameStr = aBaseStr;
}

bool NECSoundManager::GenerateFileName(SoundTypes::ESoundTypes aSoundType, char* apStrOut, uint16_t aIndex)
{
	bool lbSuccess = true;

	memset(apStrOut, 0, MAX_FILE_NAME_SIZE);

	strcat(apStrOut, (const char*)maFontBaseDir);
	strcat(apStrOut, "/");


	int lIdxValue = aIndex + 1; //NEC starts counting at 1, there is no zero
	char lStrBuf[3]; //Temporary buffer to create index number strings
	memset(lStrBuf, 0, 3);

	switch(aSoundType)
	{
	case SoundTypes::eeFontIdSnd:
		strcat(apStrOut, sFontFileStr);
		break;
	case SoundTypes::eeBootSnd:
		strcat(apStrOut, sBootFileStr);
		break;
	case SoundTypes::eePowerUpSnd:
		strcat(apStrOut, sPowerUpFileStr);
		itoa(lIdxValue, lStrBuf, 10);
		strcat(apStrOut, "0");
		strcat(apStrOut, (const char*)lStrBuf);

		break;
	case SoundTypes::eeSwingSnd:
		strcat(apStrOut, sSwingFileStr);

		itoa(lIdxValue, lStrBuf, 10);
		if(lIdxValue < 10)
		{
			strcat(apStrOut, "0"); //Leading zero
		}
		strcat(apStrOut, (const char*)lStrBuf);

		break;
	case SoundTypes::eeClashSnd:
		strcat(apStrOut, sClashFileStr);

		itoa(lIdxValue, lStrBuf, 10);
		if(lIdxValue < 10)
		{
			strcat(apStrOut, "0"); //Leading zero
		}
		strcat(apStrOut, (const char*)lStrBuf);

		break;
	case SoundTypes::eeBlasterSnd:
		strcat(apStrOut, sBlasterFileStr);

		itoa(lIdxValue, lStrBuf, 10);
		strcat(apStrOut, "0");
		strcat(apStrOut, (const char*)lStrBuf);


		break;
	case SoundTypes::eeLockupSnd:
		strcat(apStrOut, sLockupFileStr);

		itoa(lIdxValue, lStrBuf, 10);
		strcat(apStrOut, "0");
		strcat(apStrOut, (const char*)lStrBuf);


		break;
	case SoundTypes::eeForceSnd:
		strcat(apStrOut, sForceFileStr);

		itoa(lIdxValue, lStrBuf, 10);
		strcat(apStrOut, "0");
		strcat(apStrOut, (const char*)lStrBuf);


		break;
	case SoundTypes::eePowerDownSnd:
		strcat(apStrOut, sPowerDownFileStr);

		itoa(lIdxValue, lStrBuf, 10);
		strcat(apStrOut, "0");
		strcat(apStrOut, (const char*)lStrBuf);

		break;
	case SoundTypes::eeHumSnd:
		itoa(lIdxValue, lStrBuf, 10);

		strcat(apStrOut, sHumFileStr);
		strcat(apStrOut, "0");
		strcat(apStrOut, (const char*)lStrBuf);


		break;
	case SoundTypes::eeMenuSoundSnd:
		//TODO: Figure out how to handle menu sounds
		break;
	case SoundTypes::eeLowSwingSnd:
	case SoundTypes::eeHighSwingSnd:
	default:
		Serial.println("Unhandled sound type.");
		lbSuccess = false;
		break;
	}

	strcat(apStrOut, ".wav");

	return lbSuccess;
}

void NECSoundManager::CountSoundsInFont()
{
	for(int lIdx = 0; lIdx < SoundTypes::eeMaxSoundTypes; lIdx++)
	{
		maSoundCounts[lIdx] = 0;
	}

	FileUtils::CountConfig.mCountIfNoIndex = false;
	FileUtils::CountConfig.mLeadingZero = true;
	FileUtils::CountConfig.mbSkip1 = false;
	maSoundCounts[SoundTypes::eeBlasterSnd] = FileUtils::Count(sBlasterFileStr, ".wav", (const char*)maFontBaseDir, 1, 4);
	maSoundCounts[SoundTypes::eeBootSnd] = FileUtils::Count(sBootFileStr, ".wav", (const char*)maFontBaseDir, 1, 4);
	maSoundCounts[SoundTypes::eeForceSnd] = FileUtils::Count(sForceFileStr, ".wav", (const char*)maFontBaseDir, 1, 4);
	maSoundCounts[SoundTypes::eeHumSnd] = FileUtils::Count(sHumFileStr, ".wav", (const char*)maFontBaseDir, 1, 4);
	maSoundCounts[SoundTypes::eeLockupSnd] = FileUtils::Count(sLockupFileStr, ".wav", (const char*)maFontBaseDir, 1, 1);
	maSoundCounts[SoundTypes::eePowerDownSnd] = FileUtils::Count(sPowerDownFileStr, ".wav", (const char*)maFontBaseDir, 1, 4);
	maSoundCounts[SoundTypes::eePowerUpSnd] = FileUtils::Count(sPowerUpFileStr, ".wav", (const char*)maFontBaseDir, 1, 4);
	maSoundCounts[SoundTypes::eeClashSnd] = FileUtils::Count(sClashFileStr, ".wav", (const char*)maFontBaseDir, 1, 16);
	maSoundCounts[SoundTypes::eeSwingSnd] = FileUtils::Count(sSwingFileStr, ".wav", (const char*)maFontBaseDir, 1, 16);

	FileUtils::CountConfig.mCountIfNoIndex = true;
	FileUtils::CountConfig.mLeadingZero = false;
	FileUtils::CountConfig.mbSkip1 = false;
	maSoundCounts[SoundTypes::eeFontIdSnd] = FileUtils::Count(sFontFileStr, ".wav", (const char*)maFontBaseDir, 0, 1);
}

bool NECSoundManager::IsHighPerformanceSoundType(SoundTypes::ESoundTypes aSoundType)
{
	bool lIsHighPerformanceSoundType = false;
	switch(aSoundType)
	{
	case SoundTypes::eeClashSnd:
	case SoundTypes::eeSwingSnd:
	case SoundTypes::eeBlasterSnd:
		lIsHighPerformanceSoundType = true;
		break;
	default:
		lIsHighPerformanceSoundType = false;
		break;
	}

	return lIsHighPerformanceSoundType;
}
