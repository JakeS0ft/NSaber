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
 * AMotionReactive.h
 *
 *  Created on: Nov 9, 2019
 *      Author: JakeSoft
 */

#ifndef AMOTIONREACTIVE_H_
#define AMOTIONREACTIVE_H_


//A base class for classes that react to motion
class AMotionReactive
{
public:

	//Destructor
	virtual ~AMotionReactive()
	{

	}

	/**
	 * Notify a swing happened and provide rotation speed.
	 * Args:
	 *  aSpeed - Rotation speed in degrees per second.
	 */
	inline virtual void NotifySwing(float aSpeed)
	{

	}

	/**
	 * Notify a clash happened.
	 * Args:
	 *  aMagnitude - Magnitude in G
	 */
	inline virtual void NotifyClash(float aMagnitude)
	{

	}

	/**
	 * Notify a twist happened.
	 * Args:
	 *  aSpeed - Rotation speed in degrees per second.
	 */
	inline virtual void NotifyTwist(float aSpeed)
	{

	}


protected:
	//Constructor. Made private to avoid instantiation.
	AMotionReactive()
	{

	}
};



#endif /* AMOTIONREACTIVE_H_ */
