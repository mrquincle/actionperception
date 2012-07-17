/**
 * @brief 
 * @file Sky.cpp
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

#include <Sky.h>

#include <assert.h>

/**
 * The robot will never discover periodicity in a world in which it is not able to move
 * that fast that it encounters the same rain drop, but on the other side of the
 * periodic world. For that to be the case the "width" of the world need to be the same
 * as the length of the whiskers.
 */
Sky::Sky(int ceiling)
{
	this->ceiling = ceiling;
	world_width = ceiling;
	pixels = new RAIN_TYPE[ceiling*world_width];
	ground = 0;
}

Sky::~Sky()
{
	delete [] pixels;
	ground = 0;
	ceiling = 0;
	world_width = 0;
}

/**
 * The sky falls (with one line)
 */
void Sky::fall()
{
	ground++;
	if (ground == ceiling) ground = 0;
}

int Sky::firstDrop(int x_position, const int max_height) {
	assert (max_height < ceiling);
	x_position += world_width;
	x_position %= world_width;
	for (int h = 0; h < max_height; h++) {
		if (getDrop(x_position, h)) return h;
	}
	return 0;
}

void Sky::setDrop(int x_position, RAIN_TYPE value) {
	x_position += world_width;
	x_position %= world_width;
	pixels[((ground + ceiling -1) % ceiling)*world_width + x_position] = value;
}

int Sky::getDrop(int x_position, const int y_position) {
	x_position += world_width;
	x_position %= world_width;
	return pixels[(y_position + ground % ceiling)*world_width + x_position];
}
