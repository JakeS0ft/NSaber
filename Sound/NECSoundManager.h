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
 * NECSoundPlayer.h
 *
 *  Created on: Nov 8, 2019
 *      Author: JakeSoft
 */

#ifndef NECSOUNDMANAGER_H_
#define NECSOUNDMANAGER_H_

#include "AMotionReactive.h"
#include <nRF52Audio.h>
#include "ASaberSoundManager.h"

class NECSoundManager : public ASaberSoundManager
{
public:

	/**
	 * Constructor.
	 * Args:
	 *  apWavPlayer - Pointer to I2SWavPlayer to handle playback
	 */
	NECSoundManager(I2SWavPlayer* apWavPlayer);

	/**
	 * Destructor.
	 */
	~NECSoundManager();

	/**
	 * Initialize. Do anything that should happen on startup.
	 */
	virtual void Init();

	/**
	 * Plays a specific sound.
	 * Args:
	 *  aSoundType - Type of sound to play
	 *  aIndex - Index of the sound to play.
	 * Returns:
	 *  TRUE if successful, FALSE otherwise
	 */
	virtual bool PlaySound(SoundTypes::ESoundTypes aSoundType, uint16_t aIndex = 0);

	/**
	 * Plays a random sound within the selected category.
	 *
	 * Args:
	 *  aSoundType - Type of sound to play
	 * Returns:
	 *  TRUE if successful, FALSE otherwise
	 */
	virtual bool PlayRandomSound(SoundTypes::ESoundTypes aSoundType);

	/**
	 * Change the current font. This method is for support of multi-font saber
	 * sound.
	 * Args:
	 *   aFontIndex - Index of the font to use.
	 */
	virtual void SetFont(unsigned char aFontIndex);

	/**
	 * Call this in a loop to keep sound playing.
	 */
	virtual bool ContinuePlay(bool aFillMixingBuffer = false);

	/**
	 * Sets the master volume.
	 * Args:
	 *  aVolume - Integer from 0 (mute) to 100 (full volume)
	 */
	virtual void SetMasterVolume(int aVolume);

	/**
	 * Sets the font directory base name.
	 * For example, if the SD card has font directories named like "font1, font2, font3..." then
	 * this would be "font". Defaults to "necfont".
	 * Args:
	 *  aBaseStr - Base string to use
	 */
	virtual void SetFontDirNameBase(const char* aBaseStr);

protected:

	/**
	 * Generates a full file path for a specific saber sound.
	 * Takes into account the current font index and type of sound.
	 * Example: GenerateFileName(SoundTypes::eeClashSnd, MY_BUFFER, 0)
	 *          will fill MY_BUFFER with the string "fontX/clsh1.wav" where "X"
	 *          is the selected font. File naming is intended to match NEC convention.
	 * Args:
	 *  aSoundType - Type of sound
	 *  apStrOut - Output parameter, pointer to buffer to fill with the path string
	 *  aIndex - Index of the file
	 *
	 *  Returns: TRUE if successful, FALSE otherwise
	 */
	bool GenerateFileName(SoundTypes::ESoundTypes aSoundType,
			              char* apStrOut,
						  uint16_t aIndex = 0);

	/**
	 * Counts the number of each sound in the current font directory
	 * and stores the results in maSoundCounts.
	 */
	void CountSoundsInFont();

	/**
	 * Performance mitigation function. Checks if sound type is eligible
	 * for high-performance processing.
	 * Args:
	 *  aSoundType - Sound type to check
	 * Returns: TRUE if it is a high performance type, FALSE otherwise
	 */
	bool IsHighPerformanceSoundType(SoundTypes::ESoundTypes aSoundType);

	//Object to handle interface with the hardware for playback
	I2SWavPlayer* mpWavPlayer;

	//Current hum sound
	PitchShiftSDWavFile* mpHumSound;

	//Current effect sound (swing, clash, etc.)
	PitchShiftSDWavFile* mpEffectSound;
	//Current effect sound type
	SoundTypes::ESoundTypes mEffectSoundType;

	//Channel to play hum on
	int mHumChannel;
	//Channel to play effects on
	int mEffectChannel;

	//Font base name (example "necFont" or "font") numbers get appended to make the font directory name
	String mFontBaseNameStr;

	//Buffer to hold path for current font ("font1", "font2", etc.)
	char maFontBaseDir[15];

	//Keep track of how many of each sound type there is in the current font
	int maSoundCounts[SoundTypes::eeMaxSoundTypes];

};


#endif /* NECSOUNDMANAGER_H_ */
