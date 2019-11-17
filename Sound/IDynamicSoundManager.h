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
 * IDynamicSoundManager.h
 *
 *  Created on: Nov 11, 2019
 *      Author: JakeSoft
 */

#ifndef IDYNAMICSOUNDMANAGER_H_
#define IDYNAMICSOUNDMANAGER_H_

#include "SoundTypes.h"
/**
 * Interface for dynamic swing effects. Provides handles for
 * sound players to change pitch and volume of individual sounds.
 */
class IDynamicSoundManager
{
public:

	virtual ~IDynamicSoundManager()
	{
		//Do nothing
	}

	/**
	 * Set the relative volume of an individual sound. For example, this can be used to
	 * set the hum sound louder while keeping the swing and clash sounds unaffected.
	 * Args:
	 *  aType - Type of sound to adjust
	 *  aVol - Volume to set from 0.0 (mute) to 1.0 (full volume)
	 */
	virtual void SetSoundVolume(SoundTypes::ESoundTypes aType, float aVol) = 0;

	/**
	 * Set pitch of an individual sound. For example, this can be used to change
	 * the pitch of the hum sound while leaving clash sounds unaffected.
	 * aType - Type of sound to adjust
	 * aPitch - Desired pitch delta.
	 *     Negative numbers decrease pitch and playback rate.
	 *     Positive numbers increase pitch and playback rate.
	 *     0.0 is the default playback rate (normal speed and pitch)
	 *     Exmaple : Set hum pitch to half normal (lower): SetEffectPitch(SoundTypes::eeHumSnd, -1.0)
	 *     Exmaple2: Set hum pitch to double normal (higher): SetEffectPitch(SoundTypes::eeHumSnd, 1.0)
	 */
	virtual void SetSoundPitch(SoundTypes::ESoundTypes aType, float aPitch) = 0;
};



#endif /* IDYNAMICSOUNDMANAGER_H_ */
