/**
 * @brief System
 * @file System.h
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
 * @date	Apr 16, 2012
 * @project	Replicator FP7
 * @company	Almende B.V.
 * @case	Self-organised criticality
 */


#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <vector>
#include <queue>

#include <Structs.h>

class System;

typedef std::vector<System*> AP_SYSTEMS;

/**
 * We start with a very simple system. It has sensors and actuators and they are
 * both represented by a vector of doubles. The system cannot add a observation itself,
 * which is done by the Embodiment class instead. Similarly, the environment as the
 * "anti-particle" of the system cannot set actions itself
 */
class System {
public:
	System();

	virtual ~System();

	//! This will update the observations
	virtual void Tick(const Observation &observation, Action &action) = 0;

	//! Restart this system
	virtual void Restart() = 0;
private:
	//! History of observations, best represented by a FIFO queue (or deque for element-wise access)
//	std::queue<Observation> observation;
};


#endif /* SYSTEM_H_ */
