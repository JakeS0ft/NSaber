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
 * DynamicNECSoundManager.cpp
 *
 *  Created on: Nov 11, 2019
 *      Author: JakeSoft
 */

#include "Sound/DynamicNECSoundManager.h"

DynamicNECSoundManager::DynamicNECSoundManager(I2SWavPlayer* apWavPlayer) :
	NECSoundManager(apWavPlayer) //base constructor
{
	LoadDefaults();
}

DynamicNECSoundManager::~DynamicNECSoundManager()
{
	//Do nothing
}

bool DynamicNECSoundManager::PlaySound(SoundTypes::ESoundTypes aSoundType, uint16_t aIndex)
{
	//Call base class to switch to set up new sound
	bool lSuccess = NECSoundManager::PlaySound(aSoundType, aIndex);

	//Set pitch and volume for the newly commanded sound
	if(lSuccess)
	{
		if(SoundTypes::eeHumSnd == aSoundType && nullptr != mpHumSound)
		{
			mpHumSound->SetVolume(maSoundVols[aSoundType]);
			mpHumSound->SetRate(maSoundPitchs[aSoundType]);
		}
		else if(nullptr != mpEffectSound)
		{
			mpEffectSound->SetVolume(maSoundVols[aSoundType]);
			mpEffectSound->SetRate(maSoundPitchs[aSoundType]);
		}
	}

	return lSuccess;
}

void DynamicNECSoundManager::SetFont(unsigned char aFontIndex)
{
	//Reset pitches and volumes to defaults
	LoadDefaults();

	//Call base class to switch fonts
	NECSoundManager::SetFont(aFontIndex);
}

void DynamicNECSoundManager::SetSoundVolume(SoundTypes::ESoundTypes aType, float aVol)
{
	maSoundVols[aType] = aVol;

	//If setting a currently playing sound, adjust the volume
	//for that sound immediately
	if(mEffectSoundType == aType && nullptr != mpEffectSound)
	{
		mpEffectSound->SetVolume(aVol);
	}
	else if(SoundTypes::eeHumSnd == aType)
	{
		mpHumSound->SetVolume(aVol);
	}
}

void DynamicNECSoundManager::SetSoundPitch(SoundTypes::ESoundTypes aType, float aPitch)
{
	maSoundPitchs[aType] = aPitch;

	//If setting pitch for a currently playing sound, make
	//the adjustment right away
	if(mEffectSoundType == aType && nullptr != mpEffectSound)
	{
		mpEffectSound->SetRate(aPitch);
	}
	else if(SoundTypes::eeHumSnd == aType)
	{
		mpHumSound->SetRate(aPitch);
	}
}

void DynamicNECSoundManager::LoadDefaults()
{
	for(int lIdx = 0; lIdx < SoundTypes::eeMaxSoundTypes; lIdx++)
	{
		maSoundVols[lIdx] = 1.0;
		maSoundPitchs[lIdx] = 0.0;
	}
}

