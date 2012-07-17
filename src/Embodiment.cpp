/**
 * @brief 
 * @file Embodiment.cpp
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

#include <Embodiment.h>

#include <iostream>

using namespace std;

/**
 * The embodiment sensorimotor loop. The system gets a sensor input, gets a tick, then
 * the environment gets the result of the system, the action, and gets a tick. The
 * environment and the system are not allowed to empty the incoming action/perception
 * themselves. This means it can in the future be used for multiple environments or
 * systems.
 */
void Embodiment::Tick() {
	// Iterate over all systems
	for (unsigned int i = 0; i < systems.size(); ++i) {
		Action *last_action = actions[i];
		// Emptying is maybe not really necessary but probably prevents bugs
		last_action->clear();
		Observation *last_observation = observations[i];
		systems[i]->Tick(*last_observation, *last_action);
		// Emptying is maybe not really necessary but probably prevents bugs
		last_observation->clear();
	}

	// The environment is itself responsible of filling observation array for all systems
	environment->Tick(actions, observations, states);
}

void Embodiment::Restart() {
	environment->Restart();
	for (unsigned int i = 0; i < systems.size(); ++i) {
		systems[i]->Restart();
	}
	environment->Init(observations, states);
}

/**
 * The default coupling is just by calling Tick on all systems in the same order. And
 * then call Tick on the environment.
 */
void Embodiment::Couple(Environment & env, AP_SYSTEMS & systems, Coupling coupling) {
	environment = &env;
	this->systems = systems;
	this->actions.clear();
	this->observations.clear();
	this->states.clear();
	AP_SYSTEMS::iterator it;
	for (it = systems.begin(); it != systems.end(); ++it) {
		Action *action = new Action(0);
		Observation *observation = new Observation(0);
		State *state = new State(0);
		actions.push_back(action);
		observations.push_back(observation);
		states.push_back(state);
	}
	environment->Init(observations, states);
}
