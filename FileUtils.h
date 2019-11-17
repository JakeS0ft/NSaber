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
 * FileUtils.h
 *
 *  Created on: Nov 1, 2019
 *      Author: JakeSoft
 */

#ifndef FILEUTILS_H_
#define FILEUTILS_H_

#include <SD.h>
#include "Arduino.h"

#define MAX_FILE_NAME_SIZE 35

namespace FileUtils
{

/**
 * Structure used to configure the file counter algorithm.
 */
struct tCountConfig
{
	//TRUE if files with no index number should be counted, FALSE otherwise
	//Example: "poweron.wav" vs. "poweron1.wav"
	bool mCountIfNoIndex = true;

	//Index number should have leading zeros
	//Example: "swing1.wav" vs. "swing01.wav"
	bool mLeadingZero = false;

	//Search should skip index number 1
	//Example: {poweron.wav, poweron2.wav} vs. {poweron1.wav. poweron2.wav}
	bool mbSkip1 = false;
};

//Configure count algorithm
static tCountConfig CountConfig;

inline int Count(const char* aFileName, const char* aBaseDir)
{
	char lFileName[MAX_FILE_NAME_SIZE];
	memset(&lFileName, 0, MAX_FILE_NAME_SIZE);
	strcat(lFileName, aBaseDir);
	strcat(lFileName, "/");
	strcat(lFileName, aFileName); //"clash, swing, lockup, etc."

	String lFileNameStr((const char*)lFileName);

	if(SD.exists(lFileNameStr))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * Counts files matching a naming pattern.
 * Args:
 *  aBeginsWith - Filename begins with this string (Example: "swing")
 *  aEndsWith - File name ends with this string ("Example ".wav")
 *  aBaseDir - Directory to look in
 *  aStartIdx - Starting index
 *  aEndIndex - Ending index
 *
 * NOTE: Algorithm can be tweaked by setting CountConfig
 */
inline int Count(const char* aBeginsWith, const char* aEndsWith, const char* aBaseDir,
		         int aStartIdx = 0, int aEndIdx = 99)
{
	int lCount = 0;
	bool lContinue = true;

	if(CountConfig.mCountIfNoIndex)
	{
		char lFileName[MAX_FILE_NAME_SIZE];
		memset(&lFileName, 0, MAX_FILE_NAME_SIZE);
		strcat(lFileName, aBaseDir);
		strcat(lFileName, "/");
		strcat(lFileName, aBeginsWith); //"clash, swing, lockup, etc."
		strcat(lFileName, aEndsWith);   //".wav, .txt, etc."

		String lFileNameStr((const char*)lFileName);

		if(	SD.exists(lFileNameStr) )
		{
			lCount++;
			Serial.print(lFileNameStr);Serial.println(" exists.");
		}
		else
		{
			Serial.print(lFileNameStr);Serial.println(" DNE.");
			lContinue = false;
		}
	}


	for(int lIdx = aStartIdx; lIdx <= aEndIdx && lContinue; lIdx++)
	{

		if(1 == lIdx && CountConfig.mbSkip1)
		{
			lIdx++;
			if(lIdx > aEndIdx)
			{
				break;
			}
		}

		//Convert current index to string
		char lIdxStrBuf[3];
		char* lIdxStr = itoa(lIdx, lIdxStrBuf, 10);

		char lFileName[MAX_FILE_NAME_SIZE];
		memset(&lFileName, 0, MAX_FILE_NAME_SIZE);
		strcat(lFileName, aBaseDir);
		strcat(lFileName, "/");
		strcat(lFileName, aBeginsWith); //"clash, swing, lockup, etc."

		if(CountConfig.mLeadingZero && lIdx < 10)
		{
			strcat(lFileName, "0");
		}

		strcat(lFileName, lIdxStr);     //"1, 2, 3, 4..."
		strcat(lFileName, aEndsWith);   //".wav, .txt, etc."

		String lFileNameStr((const char*)lFileName);

		if(	SD.exists(lFileNameStr) )
		{
			Serial.print((const char*)lFileName);
			Serial.println(" exists.");

			lCount++;
		}
		else
		{
			//Give up as soon as we don't find one
			lContinue = false;
		}
	}


	return lCount;
}

};


#endif /* FILEUTILS_H_ */
