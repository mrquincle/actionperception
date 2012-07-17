/**
 * @brief 
 * @file Information.h
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


#ifndef INFORMATION_H_
#define INFORMATION_H_

#include <Structs.h>

enum InfoType {
	IT_EMPOWERMENT, 					// Klyubin
	IT_INFORMATION_TO_GO,
	IT_LOOKAHEAD_RELEVANT_INFORMATION,	// Polani
	IT_FREE_ENERGY, 					// Friston, minimum bound on surprise
	SENSORIMOTOR_ENTROPY,
	EXCESS_ENTROPY,
	IT_COUNT							// # of types
};

/**
 * The information metrics that can be calculated. It will need "logs" or "traces"
 * from the action values and sensor values. It is not possible to access world states
 * from the robot/system. Hence, we shouldn't use them either to calculate forms of
 * information we cannot actually access in a real system.
 */
class Information {
public:
	Information();

	~Information();

	//! The results should be a number of bits(!)
	int Calculate();

    InfoType getInfoType() const;
    void setInfoType(InfoType infoType);

    inline void setSensorimotorPath(SensorimotorPath *path) {
    	this->sensorimotor_path = path; }

    //! Calculate the (joint) random variables
    void CalculateJointRandomVariables();
protected:
    int CalculateEmpowerment();

    //! Random variable contains probabilities (normalized frequencies)
    int Uncertainty(Point & var);
private:
	InfoType info_type;

	//! The sensorimotor path contains sensor and actuator values which need to
	//! be translated towards probabilities (by e.g. binning)
	SensorimotorPath *sensorimotor_path;

	Point sensor;

	Point motor;

	Point sensorimotor;

};



#endif /* INFORMATION_H_ */
