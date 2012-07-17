/**
 * @brief 
 * @file RainBenchmark.h
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

#ifndef RAINBENCHMARK_H_
#define RAINBENCHMARK_H_

#include <queue>
#include <vector>

#include <Environment.h>
#include <Sky.h>

/*
 * A simple variant of FallingBoxBenchmark. N whiskers are pointing upwards and particles drop
 * from the sky. How to position an N-width robot such that it won't get wet/hurt? There are
 * two types of rain particles. One can be used to indicate "energy", the other one to "loose"
 * energy.
 */
class RainBenchmark: public Environment {
public:
	RainBenchmark();

	~RainBenchmark();

	/**
	 * An action as input, a observation as output.
	 */
	void Tick(const Action &action, Observation &observation);

    int getWhiskerCount() const;
    void setWhiskerCount(int whiskerCount);

    int getWhiskerLength() const;
    void setWhiskerLength(int whiskerLength);

    void setRainDistribution(int seed);

private:
	int whisker_count;

	int whisker_length;

	int world_size;

	int robot_position;

	int rain_seed;

	Sky *sky;
};

#endif // RAINBENCHMARK_H_
