/**
 * @brief 
 * @file Sky.h
 *
 * This file is created at Almende B.V. It is open-source software and part of the Common 
 * Hybrid Agent Platform (CHAP). A toolbox with a lot of open-source tools, ranging from 
 * thread pools and TCP/IP components to control architectures and learning algorithms. 
 * This software is published under the GNU Lesser General Public license (LGPL).
 *
 * It is not possible to add usage restrictions to an open-source license. Nevertheless,
 * we personally strongly object against this software used by the military, in the
 * bio-industry, for animal experimentation, or anything that violates the Universal
 * Declaration of Human Rights.
 *
 * Copyright © 2012 Anne van Rossum <anne@almende.com>
 *
 * @author 	Anne C. van Rossum
 * @date	Apr 26, 2012
 * @project	Replicator FP7
 * @company	Almende B.V.
 * @case	Self-organised criticality
 */


#ifndef SKY_H_
#define SKY_H_

#include <vector>
#include <queue>

typedef char RAIN_TYPE;

typedef RAIN_TYPE* SKY;

/**
 * Representing a movement of the robot to the left is of course quicker than representing a
 * shift of the entire sky to the right. It seems to be a good idea to represent the rain
 * dropping by a circular buffer such as a std::queue. However, it is better to have just a
 * 2D array of the right size. For the pointed upward whiskers namely you will elseway need
 * to loop over multiple vectors to see where they detect a rain drop.
 */
struct Sky {
public:
	//! Sky does not need to be higher than the whiskers are long, add 1 to be sure
	Sky(int ceiling);

	~Sky();

	//! Let the rain drops fall one time step
	void fall();

	//! The first drop at position is at height, the x-position is allowed to be
	//! outside of the scope of world_width, it will be automatically wrapped around
	int firstDrop(int x_position, const int max_height);

	//! Add a rain drop at the top of the sky
	void setDrop(int x_position, RAIN_TYPE value);

protected:
	//! Get drop corrected for ground position
	int getDrop(int x_position, const int y_position);
private:
	//! All pixels
	SKY pixels;

	//! Current ground level
	int ground;

	//! Beyond which there is nothing...
	int ceiling;

	//! The width of the periodic world
	int world_width;
};

#endif /* SKY_H_ */
