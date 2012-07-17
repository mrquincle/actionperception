/**
 * @brief 
 * @file Environment.cpp
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

#include <Environment.h>

#include <sys/syslog.h>

Environment::Environment()
{
	accumulated_reward = 0;
	discounted_reward = 0;
	instantaneous_reward = 0;
	verbosity = LOG_INFO;
}



Environment::~Environment()
{
}

void Environment::Clear() {
	accumulated_reward = 0;
	discounted_reward = 0;
	instantaneous_reward = 0;
}
