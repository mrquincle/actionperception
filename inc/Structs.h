/**
 * @brief 
 * @file Structs.h
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
 * @date	Apr 17, 2012
 * @project	Replicator FP7
 * @company	Almende B.V.
 * @case	Self-organised criticality
 */


#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <vector>
#include <list>

typedef double AP_TYPE;

typedef double PROB_TYPE;

//typedef int FREQ_TYPE;

//! Simple definition for Observation for now
typedef std::vector<AP_TYPE> Observation;

//! Simple definition for Action too
typedef std::vector<AP_TYPE> Action;

//! And a generic description for State
typedef std::vector<AP_TYPE> State;

/***************************************************************************************************
 * A random variable is in most papers just a scalar. In our case, however, it is a vector. A sensor
 * input or motor output is most naturally represented by multiple values. Hence, a RandomVariable
 * in our framework is a set of such points (a vector of Point*).
 **************************************************************************************************/
typedef std::vector<PROB_TYPE> Point;
typedef std::vector<Point*> RandomVariable;

//! A observation-action pair at time=t
struct SensationActionPair {
	Observation observation;
	Action action;
	long int t;
};

//! The sequence of observations and actions, because of most often a Markovian
//! approach it seems most naturally to use a list (but see SensorimotorContainer)
typedef std::list<SensationActionPair*> SensorimotorPath;

//! The natural representation is SensorimotorPath, but to be able to use sorting we need a random
//! access iterator for our std container, hence we copy the entire list to a container
typedef std::vector<SensationActionPair*> SensorimotorContainer;


#endif /* STRUCTS_H_ */
