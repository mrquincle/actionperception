/**
 * @brief 
 * @file RainBenchmark.cpp
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

#include <RainBenchmark.h>

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

//! Use xorshift (wikipedia), or something else simple, but under control
//! It would be nice to have just one seed necessary though.
uint32_t xor128(void) {
  static uint32_t x = 123456789;
  static uint32_t y = 362436069;
  static uint32_t z = 521288629;
  static uint32_t w = 88675123;
  uint32_t t;

  t = x ^ (x << 11);
  x = y; y = z; z = w;
  return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}


RainBenchmark::RainBenchmark()
{
}

RainBenchmark::~RainBenchmark()
{

}

/**
 * After all parameters have been set, this is the function that needs to
 * called in an (infinite or very long) loop.
 */
void RainBenchmark::Tick(const Action &action, Observation &observation)
{
	assert (!action.empty());
	AP_TYPE act = action.front();
	robot_position += (act > 0) ? 1 : world_size -1;
	robot_position %= world_size;
	sky->fall();
	for (int w = 0; w < whisker_count; ++w)
	{
		RAIN_TYPE drop = sky->firstDrop(w+robot_position-whisker_count/2, whisker_length);
//		observation.push(drop);
	}
}


int RainBenchmark::getWhiskerCount() const
{
	return whisker_count;
}

int RainBenchmark::getWhiskerLength() const
{
	return whisker_length;
}

void RainBenchmark::setWhiskerCount(int whiskerCount)
{
	whisker_count = whiskerCount;
}

void RainBenchmark::setWhiskerLength(int whiskerLength)
{
	whisker_length = whiskerLength;
	world_size = whiskerLength+1;
	assert (sky == NULL);
	sky = new Sky(world_size);
}

void RainBenchmark::setRainDistribution(int seed)
{
	rain_seed = seed;
}





