/***************************************************************************************************
 * @brief
 * @file TestMutualInformation.h
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
 * @date	May 11, 2012
 * @project	Replicator FP7
 * @company	Almende B.V. & Distributed Organisms B.V.
 * @case	Self-organised criticality
 **************************************************************************************************/


#ifndef TESTMUTUALINFORMATION_H_
#define TESTMUTUALINFORMATION_H_

/**
 * Use sources at http://www.klab.caltech.edu/~kraskov/MILCA/ to test if the mutual information
 * figures are actually correct. The mutual information estimator can be compared to the MIxnyn
 * tool at the MILCA site.
 */
class TestMutualInformation {
public:
	TestMutualInformation();

	~TestMutualInformation();

	void Test();
protected:
	// Sinus
	void Sinus();

	// Random values
	void Independent();

	//! Gaussian distribution
	void Normal();
private:
	MutualInformation *mi;
};


#endif /* TESTMUTUALINFORMATION_H_ */
