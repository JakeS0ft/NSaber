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
 * ASaberSoundSoundManager.h
 *
 *  Created on: Nov 1, 2019
 *      Author: JakeSoft
 */

#ifndef ASABERSOUNDMANAGER_H_
#define ASABERSOUNDMANAGER_H_

#include <Arduino.h> //For uint16_t
#include "SoundTypes.h"

class ASaberSoundManager
{
public:

	virtual ~ASaberSoundManager()
	{
		//Do nothing
	}

	/**
	 * Initialize. Do anything that should happen on startup.
	 */
	virtual void Init() = 0;

	/**
	 * Play a specific sound. Subclasses should implement a method that can
	 * play a specific sound on demand. For example, a call to play the second
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
	virtual bool PlaySound(SoundTypes::ESoundTypes aSoundType, uint16_t aIndex = 0) = 0;

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
	virtual bool PlayRandomSound(SoundTypes::ESoundTypes aSoundType) = 0;

	/**
	 * Stop playback. Subclasses should implement a method that will stop
	 * all sound playback from happening, if such a feature is supported
	 * by the sound module. Otherwise, this method can be left as is and
	 * will do nothing.
	 */
	virtual void Stop()
	{
		//Do nothing
	}

	/**
	 * Set volume. Subclasses should implement a routine that can adjust the
	 * volume of the sound module if such a feature is supported. If the
	 * feature is not supported, then this method can be left as is and will
	 * do nothing.
	 * Args:
	 *  aVolume - Volume level to use
	 */
	virtual void SetMasterVolume(int aVolume)
	{
		//Do nothing
	}

	/**
	 * Change the current font. This method is for support of multi-font saber
	 * sound. Subclasses should implement code that will cause subsequent calls
	 * to PlaySound() and PlayRandomSound() to play sounds from alternate fonts
	 * specified by the aFontIndex argument of this method.
	 * Args:
	 *   aFontIndex - Index of the font to use.
	 */
	virtual void SetFont(unsigned char aFontIndex) = 0;

	/**
	 * Call this in a loop to keep sound playing.
	 */
	virtual bool ContinuePlay(bool aFillMixingBuffer = false)
	{
		return false;
	}

	/**
	 * TODO: Remove this.
	 */
	virtual void PopulateMixingBuffer(int aNumSamples)
	{
		//Do nothing
	}

	/**
	 * Set the base name for the font directories.
	 * Example: "font" if fonts are divided like "font1, font2, font3, etc."
	 * Args:
	 *  aBaseStr - Base string for font directory names
	 */
	virtual void SetFontDirNameBase(const char* aBaseStr) = 0;
};


#endif /* ASABERSOUNDMANAGER_H_ */
