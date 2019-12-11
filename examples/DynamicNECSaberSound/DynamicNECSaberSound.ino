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
 * This sketch demonstrates hum pitch shift of NEC-style sound fonts with
 * on Protowerkstatt Infinity (nRF52) programmable FX boards.
 * Dependencies:
 *  * nRF52Audio library <https://github.com/JakeS0ft/nRF52Audio>
 *
 *  This sketch assumes all files for each font are in the same directory on
 *  the SD card. NEC fonts are sometimes distributed with each sound of each type in
 *  sub-directories so keep this in mind. If that happens, just reorganize the files
 *  by copying them all to the same directory on the SD card.
 *
 *  Like this:
 *  font1/out01.wav
 *
 *  NOT like this:
 *  font1/out/out01.wav
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

DynamicNECSoundManager* gpDynamicNecPlayer;
I2SWavPlayer* gpI2SPlayer;


void TestDynamicHum()
{

   Serial.println("Hum pitch shift test starts.");

   gpI2SPlayer = new I2SWavPlayer(PIN_I2S_MCK,
                                  PIN_I2S_BCLK,
                                  PIN_I2S_LRCK,
                                  PIN_I2S_DIN,
                                  PIN_I2S_SD);

   gpDynamicNecPlayer = new DynamicNECSoundManager(gpI2SPlayer);


   gpI2SPlayer->Init();
   gpI2SPlayer->StartPlayback();

   gpDynamicNecPlayer->Init();                            //Initialize the sound manager
   gpDynamicNecPlayer->SetFontDirNameBase("2205/ifont");  //(optional) Set base name for fonts based on SD card layout
                                                          //(defaults to "necfont" as base name)
   gpDynamicNecPlayer->SetFont(0);                        //Select font index
   gpDynamicNecPlayer->SetMasterVolume(12);               //Set volume from 0 (mute) to 100 (full)

   gpDynamicNecPlayer->PlaySound(SoundTypes::eeHumSnd, 0);

   Serial.println("Pitching down.");
   float lPitch = 0.0;
   while(lPitch > -2.0)
   {
      unsigned long lStartTime = millis();
      while(millis() - lStartTime < 100)
      {
         gpI2SPlayer->ContinuePlayback();
      }
      lPitch -= 0.03;

      //Adjust the pitch of the hum sound
      gpDynamicNecPlayer->SetSoundPitch(SoundTypes::eeHumSnd, lPitch);
   }

   Serial.println("Pitching up.");
   while(lPitch < 4.0)
   {
      unsigned long lStartTime = millis();

      while(millis() - lStartTime < 80)
      {
         gpI2SPlayer->ContinuePlayback();
      }
      lPitch += 0.05;

      //Adjust the pitch of the hum sound
      gpDynamicNecPlayer->SetSoundPitch(SoundTypes::eeHumSnd, lPitch);
   }

   Serial.println("Test ends.");

   gpI2SPlayer->StopPlayback();
   delete gpDynamicNecPlayer;
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

   TestDynamicHum();
}

// The loop function is called in an endless loop
void loop()
{
   //Do nothing
}
