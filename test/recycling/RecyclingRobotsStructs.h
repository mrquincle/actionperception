/***************************************************************************************************
 * @brief
 * @file RecyclingRobotsStructs.h
 *
 * This file is created at Almende B.V. It is open-source software and part of the Common Hybrid 
 * Agent Platform (CHAP). A toolbox with a lot of open-source tools, ranging from thread pools and 
 * TCP/IP components to control architectures and learning algorithms. This software is published 
 * under the GNU Lesser General Public license (LGPL).
 *
 * It is not possible to add usage restrictions to an open-source license. Nevertheless, we 
 * personally strongly object against this software used by the military, in the bio-industry, for 
 * animal experimentation, or anything that violates the Universal Declaration of Human Rights.
 *
 * Copyright © 2012 Anne van Rossum <anne@almende.com>
 ***************************************************************************************************
 * @author 	Anne C. van Rossum
 * @date	May 22, 2012
 * @project	Replicator FP7
 * @company	Almende B.V. & Distributed Organisms B.V.
 * @case	Self-organised criticality
 **************************************************************************************************/


#ifndef RECYCLINGROBOTSSTRUCTS_H_
#define RECYCLINGROBOTSSTRUCTS_H_

#define NOF_SYSTEMS 2

#include <string>

/**
 * We have several navigation actions available. We can search for the big item, for the small item,
 * or we can go back to the recharging station. The probability of detecting a big item is
 * implicitly incorporated by having a larger chance of depleting the battery.
 */
enum RR_ACTION { RRA_SEARCH_BIG, RRA_SEARCH_SMALL, RRA_RECHARGE, RRA_COUNT };
const std::string RR_ACTION_STR[] = { "RRA_SEARCH_BIG", "RRA_SEARCH_SMALL", "RRA_RECHARGE" };

/**
 * There are two more "actions" we can do. We can search using the camera, which is more expensive,
 * but of better quality, or we can search using infrared, which fails to find sometimes the item
 * we search for.
 */
//enum RR_ACTION_SENSOR { RRA_VISION, RRA_INFRARED, RRA_COUNT };

enum RR_OBSERVATION { RRO_BATTERY_LOW, RRO_BATTERY_HIGH, RRO_COUNT };
const std::string RR_OBSERVATION_STR[] = { "RRO_BATTERY_LOW", "RRO_BATTERY_HIGH" };

enum RR_STATE { RRS_BATTERY_LOW, RRS_BATTERY_HIGH, RRS_COUNT };

//! There is only one action type
enum RR_ACTION_TYPE { RRAT_SEARCHING, RRAT_COUNT };

//! There is only one observation type
enum RR_OBSERVATION_TYPE { RROT_BATTERY_LEVEL, RROT_INSTANTANEOUS_REWARD, RROT_COUNT };

//! There is only one state type per robot
enum RR_STATE_TYPE { RRST_BATTERY_LEVEL, RRST_COUNT };


#endif /* RECYCLINGROBOTSSTRUCTS_H_ */
