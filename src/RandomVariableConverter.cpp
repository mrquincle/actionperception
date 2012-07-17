/**
 * @brief 
 * @file RandomVariableConverter.cpp
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

#include <assert.h>
#include <stddef.h>
#include <RandomVariableConverter.h>

RandomVariableConverter::RandomVariableConverter()
{
//	motor = new RandomVariable(0);
//	sensor = new RandomVariable(0);
//	sensorimotor = new RandomVariable(0);
//	sensors_over_time = new RandomVariableSet(0);
}



RandomVariableConverter::~RandomVariableConverter()
{
	delete motor;
	delete sensor;
	delete sensorimotor;
}

/**
 * Sum the number of times that the value falls in the given bin.
 */
//PROB_TYPE sum_equals(PROB_TYPE value, FREQ_TYPE bin) {
//	value = value +
//}

//PROB_TYPE get_sensor(PROB_TYPE value, FREQ_TYPE bin) {
//	value = value +
//}

/**
 * The value should be normalized from 0 to 1. Subsequently the bin is easy
 * to pick. With 4 bins, they will span [0 - 0.25|0.25 - 0.5|0.5 - 0.75|0.75 - 1.0].
 * The bin width with 4 bins is 0.25. Hence, to get the bin index from 0 to 3,
 * say 0.66, you'll need to divide 0.66 by 0.25, which is 2 and something. The only
 * exception is 1. That value is corrected so it falls in the last bin.
 */
int RandomVariableConverter::GetBin(AP_TYPE value) {
	if (value == (AP_TYPE)1.0) return nof_bins - 1;
	return value / bin_width;
}

/**
 * Provided: inner_product
 * Starting with an initial value of @a init, multiplies successive
 * elements from the two ranges and adds each product into the accumulated
 * value using operator+(). The values in the ranges are processed in
 * order.
 * Hence, this is only over pair-wise elements and returns one value.
 *
 * Needed: extensive_product
 * Starting with the initial values in @a result, multiplies successive elements
 * from one range with all elements in the other range and puts them into the
 * accumulated vector using operator+() (binary_op1). The values in the ranges
 * are processed in order.
 * So, the results are row-wise sums of the cross products.
 *
 * @ingroup mutating_algorithms
 * @param  first1     An input iterator.
 * @param  last1      An input iterator.
 * @param  first2     An input iterator.
 * @param  last2      An input iterator.
 * @param  result     An output iterator.
 * @param  binary_op1 A binary operator.
 * @param  binary_op2 A binary operator.
 * @return Iterator pointing just beyond the values written to result.
 */
template<typename _InputIterator1, typename _InputIterator2,
	   typename _OutputIterator, typename _BinaryOperation1,
	   typename _BinaryOperation2>
  _OutputIterator
  extensive_product(_InputIterator1 __first1, _InputIterator1 __last1,
	      _InputIterator2 __first2, _InputIterator2 __last2,
	      _OutputIterator __result,
		  _BinaryOperation1 __binary_op1,
		  _BinaryOperation2 __binary_op2)
{
    // concept requirements
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator1>)
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator2>)
    __glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator,
    		__typeof__(__binary_op(*__first1,*__first2))>)
    __glibcxx_requires_valid_range(__first1, __last1);
    __glibcxx_requires_valid_range(__first2, __last2);

    for (; __first1 != __last1; ++__first1, ++__result)
        for (; __first2 != __last2; ++__first2)
        	*__result = __binary_op1(*__result, __binary_op2(*__first1, *__first2));
    return __result;
}

/**
 * We have to calculate somehow random variables from sensor input or motor output. This
 * input can have a continuous range of values and be of a vector format. The sensor input
 * S[t=0]={0.1, 0.2, 0.1}, S[t=1]={0.1, 0.8, 0.1}, S[t=1]={0.1, 0.8, 0.8} will become
 * with two bins the following frequency matrix F={3, 1, 2 : 3, 2, 2 : 3, 2, 1}. This
 * corresponds to P[t=0]={1.0, 0.33, 0.67}, etc. Obviously the elements are here considered
 * independent. There might very well be a better way in which to calculate the probability
 * of an entire sensory input. It is of course possible to assign an aggregated value to an
 * input vector and set this as the observation value.
 */
void RandomVariableConverter::CalculateJointRandomVariables()
{
	assert (path != NULL);
	assert (!path->empty());
	SensorimotorPath::iterator it;
//	std::transform(path.begin(), path.end(), path.begin(), sensor.begin(),
//			get_sensor<PROB_TYPE>);
	// std::inner_product

//	sensor->clear();
//	sensor->reserve(path->size());
//	sensor_bins->clear();
//	sensor_bins->reserve(nof_bins);
//	for (int i = 0; i < nof_bins; ++i) {
//		sensor_bins[i] = 0;
//	}

	// the number of sensors or the "observation" size
	/*
	size_t observation_size = path->front().observation.size();

	// create double vector: sensors_over_time[observation][bin]
	sensors_over_time->clear();
	for (int s = 0; s < observation_size; ++s) {
		sensors_over_time->push_back(new RandomVariable());
		sensors_over_time[s].clear();
		for (int i = 0; i < nof_bins; ++i) {
			sensors_over_time[s].push_back(new RandomVariable());
		}
	}

	// quantify the sensory events by the GetBin function
	for (int s = 0; s < observation_size; ++s) {
		for (it = path->begin(); it != path->end(); ++it) {
			int bin = GetBin(((*it).observation)[s]);
			sensors_over_time->at(s)->at(bin)++;
		}
		// make probabilities by frequencies by dividing over path length
		for (int i = 0; i < nof_bins; ++i) {
			sensors_over_time->at(s)->at(i) /= (AP_TYPE)path->size();
		}
	}
*/
	// now calculate for each sensor value its probability
//	sensor->at()

}

void RandomVariableConverter::setNofBins(int nofBins)
{
	assert (nofBins > 0);
    nof_bins = nofBins;
    bin_width = 1.0/(AP_TYPE)nof_bins;
}

Point *RandomVariableConverter::getMotor() const
{
    return motor;
}

int RandomVariableConverter::getNofBins() const
{
    return nof_bins;
}

Point *RandomVariableConverter::getSensor() const
{
    return sensor;
}

Point *RandomVariableConverter::getSensorimotor() const
{
    return sensorimotor;
}
