/**
 * @brief 
 * @file RandomVariableConverter.h
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
 * @date	May 5, 2012
 * @project	Replicator FP7
 * @company	Almende B.V.
 * @case	Self-organised criticality
 */


#ifndef RANDOMVARIABLECONVERTER_H_
#define RANDOMVARIABLECONVERTER_H_

#include <Structs.h>


/**
 * Converts sensorimotor trajectory towards a random variable presentation. The most
 * simple way to do this conversion is by (fixed) binning.
 */
class RandomVariableConverter {
public:
	RandomVariableConverter();

	~RandomVariableConverter();

    inline void setSensorimotorPath(SensorimotorPath *path) {
    	this->path = path; }

	void CalculateJointRandomVariables();

    int getNofBins() const;

    Point *getMotor() const;
    Point *getSensor() const;
    Point *getSensorimotor() const;

    void setNofBins(int nofBins);
//    void setSensorResolution();
protected:
    int GetBin(AP_TYPE value);
private:
    //! Set number of bins and its reciproce, the bin width
	int nof_bins;

	AP_TYPE bin_width;

	//! Discrete and normalized outputs
//	FreqVariable *sensor_bins;

	//! Probability of sensor values over time
	RandomVariable *sensors_over_time;

	//! Discrete and normalized outputs
//	FreqVariable *motor_bins;

	//! Discrete and normalized outputs
//	FreqVariable *sensorimotor_bins;

	//! Discrete and normalized outputs
	Point *sensor;

	//! Discrete and normalized outputs
	Point *motor;

	//! Discrete and normalized outputs
	Point *sensorimotor;

	//! Input for discretization
	SensorimotorPath *path;
};

#endif /* RANDOMVARIABLECONVERTER_H_ */
