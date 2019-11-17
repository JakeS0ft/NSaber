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
 * DynamicNECSoundManager.h
 *
 *  Created on: Nov 11, 2019
 *      Author: JakeSoft
 */

#ifndef DYNAMICNECSOUNDMANAGER_H_
#define DYNAMICNECSOUNDMANAGER_H_

#include "IDynamicSoundManager.h"
#include "NECSoundManager.h"

class DynamicNECSoundManager : public NECSoundManager, public IDynamicSoundManager
{
public:
	DynamicNECSoundManager(I2SWavPlayer* apWavPlayer);

	virtual ~DynamicNECSoundManager();

	//Plays sound and applies tuning properties
	virtual bool PlaySound(SoundTypes::ESoundTypes aSoundType, uint16_t aIndex = 0);

	//Switch font
	virtual void SetFont(unsigned char aFontIndex);

	//See IDynamicSwing interface
	virtual void SetSoundVolume(SoundTypes::ESoundTypes aType, float aVol);

	//See IDynamicSwing interface
	virtual void SetSoundPitch(SoundTypes::ESoundTypes aType, float aPitch);
protected:

	void LoadDefaults();

	float maSoundVols[SoundTypes::eeMaxSoundTypes];
	float maSoundPitchs[SoundTypes::eeMaxSoundTypes];
};

#endif /* DYNAMICNECSOUNDMANAGER_H_ */
