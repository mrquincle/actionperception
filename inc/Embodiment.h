/**
 * @brief 
 * @file Embodiment.h
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


#ifndef EMBODIMENT_H_
#define EMBODIMENT_H_

#include <Environment.h>
#include <System.h>

class System;
class Environment;

/**
 * The coupling between system and environment can e.g. be in the form of a point-wise
 * location if the environment is a 2D or 3D world. It can be extended spatially, it can
 * involve colors, etc. Each subsequent Action will render updating the body in the
 * environment more involved. Hence, it is smart to keep this Coupling simple.
 *
 * By default the coupling structure is empty
 */
struct Coupling { };

//struct EmbodiedSystem {
//	//! The embodiment system does have a last action
//	Action *lastAction;
//
//	//! And a last observation
//	Sensation *lastSensation;
//};

//typedef std::vector<EmbodiedSystem*> AP_EMBSYSTEMS;

/**
 * The embodiment
 */
class Embodiment {
public:
	//! Embodiment ticks the system first and then the environment (just convention)
	void Tick();

	//! Couple in general the system with the environment
	void Couple(Environment &env, AP_SYSTEMS &systems, Coupling coupling);

	void Restart();
private:
	//! The environment is referenced through embodiment, but not part of it
	Environment *environment;

	//! There are multiple systems in one environment
	AP_SYSTEMS systems;

	//! Every system does have some state information that is communicated to the environment
	AP_MAS_ACTION actions;

	//! Every system does have some state information that needs to be obtained from the environment
	AP_MAS_OBSERVATION observations;

	//! And every system does have state that it cannot control
	AP_MAS_STATE states;
};


#endif /* EMBODIMENT_H_ */
