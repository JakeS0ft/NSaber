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

/**
 * This sketch demonstrates polyphonic playback of NEC-style sound fonts with
 * on Protowerkstatt Infinity (nRF52) programmable FX boards.
 * Dependencies:
 *  * nRF52Audio library <https://github.com/JakeS0ft/nRF52Audio>
 */

#include "Arduino.h"
#include <NSaber.h>

//I2S Pins
#define PIN_I2S_MCK 13
#define PIN_I2S_BCLK (A2)
#define PIN_I2S_LRCK (A3)
#define PIN_I2S_DIN 18
#define PIN_I2S_SD  10

//SPI's Cable Select pin for SD card reader
#define PIN_SPI_CS  11

NECSoundManager* gpNecPlayer;
I2SWavPlayer* gpI2SPlayer;

/**
 * Plays all sounds of a given type.
 * Args:
 *  aSndType - Type of sound to play (swing, clash, etc.)
 *  apSndMgr - Pointer to sound manager
 *  aSoundCount - How many different sounds to play
 *  aTimeForEach - How long to allow each sound to play (in milliseconds)
 */
void PlayAllSoundsOfType(SoundTypes::ESoundTypes aSndType, NECSoundManager* apSndMgr, int aSoundCount, unsigned long aTimeForEach)
{

	for(int lIdx = 0; lIdx < aSoundCount; lIdx++)
	{
		apSndMgr->PlaySound(aSndType, lIdx);

		unsigned long lStartTime = millis();
		while(millis() - lStartTime < aTimeForEach)
		{
			__WFE();
			gpI2SPlayer->ContinuePlayback();  //Keep the global I2S player pumping data
			apSndMgr->ContinuePlay();
		}
	}
}

void TestNECSoundPlayer()
{

	//Create objects for test
	gpI2SPlayer = new I2SWavPlayer(PIN_I2S_MCK,
								   PIN_I2S_BCLK,
								   PIN_I2S_LRCK,
								   PIN_I2S_DIN,
								   PIN_I2S_SD);
	gpNecPlayer = new NECSoundManager(gpI2SPlayer);

	gpI2SPlayer->Init();                            //Initialize the I2S hardware and wav player
	gpI2SPlayer->StartPlayback();                   //Begin I2S in hardware

	gpNecPlayer->Init();                            //Initialize the sound manager
	gpNecPlayer->SetFontDirNameBase("2205/ifont");  //(optional) Set base name for fonts based on SD card layout
	                                                //(defaults to "necfont" as base name)
	gpNecPlayer->SetFont(0);                        //Select font index
	gpNecPlayer->SetMasterVolume(12);               //Set volume from 0 (mute) to 100 (full)

	//Play several sounds of each type (must be present on the SD card or they won't play)
	PlayAllSoundsOfType(SoundTypes::eePowerUpSnd, gpNecPlayer, 1, 3000);
	PlayAllSoundsOfType(SoundTypes::eeHumSnd, gpNecPlayer, 1, 3000);
	PlayAllSoundsOfType(SoundTypes::eeSwingSnd, gpNecPlayer, 8, 1000);
	PlayAllSoundsOfType(SoundTypes::eeClashSnd, gpNecPlayer, 4, 900);
	PlayAllSoundsOfType(SoundTypes::eeBlasterSnd, gpNecPlayer, 4, 900);
	PlayAllSoundsOfType(SoundTypes::eeLockupSnd, gpNecPlayer, 1, 5000);
	PlayAllSoundsOfType(SoundTypes::eePowerDownSnd, gpNecPlayer, 1, 3000);

	Serial.println("Test ends.");

	//Cleanup
	gpI2SPlayer->StopPlayback();
	delete gpNecPlayer;
	delete gpI2SPlayer;

}

void setup()
{
	delay(1000);
	Serial.begin(115200);

	if(!SD.begin(8000000, PIN_SPI_CS))
	{
		Serial.println("SD init failed.");
		return; //Punt. We can't work without SD card
	}
	Serial.println("SD init completed.");

	TestNECSoundPlayer();
}

// The loop function is called in an endless loop
void loop()
{
	//Do nothing
}
