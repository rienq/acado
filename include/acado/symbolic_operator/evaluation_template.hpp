/*
 *    This file is part of ACADO Toolkit.
 *
 *    ACADO Toolkit -- A Toolkit for Automatic Control and Dynamic Optimization.
 *    Copyright (C) 2008-2014 by Boris Houska, Hans Joachim Ferreau,
 *    Milan Vukov, Rien Quirynen, KU Leuven.
 *    Developed within the Optimization in Engineering Center (OPTEC)
 *    under supervision of Moritz Diehl. All rights reserved.
 *
 *    ACADO Toolkit is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 3 of the License, or (at your option) any later version.
 *
 *    ACADO Toolkit is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with ACADO Toolkit; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */



/**
 *    \file include/acado/symbolic_operator/evaluation_template.hpp
 *    \author Boris Houska
 */


#ifndef ACADO_TOOLKIT_EVALUATION_TEMPLATE_HPP
#define ACADO_TOOLKIT_EVALUATION_TEMPLATE_HPP

#include <acado/symbolic_operator/symbolic_operator_fwd.hpp>

#include <functional>


namespace std {
  template <>
  struct hash<ACADO::Operator*>
  {
    size_t operator()(const ACADO::Operator* k) const
    {

      return hash<size_t>()((size_t)k);
    }
  };
}

BEGIN_NAMESPACE_ACADO

/**
 *	\brief Templated class for operator evaluation.
 *
 *	\ingroup BasicDataStructures
 *
 *	\author Boris Houska
 */

template <typename T>
class EvaluationTemplate : public EvaluationBase{

public:

	/** Default constructor. */
	EvaluationTemplate();
	EvaluationTemplate( std::vector<T> *res );
	
	virtual ~EvaluationTemplate();

	virtual void addition   ( uint arg1, uint arg2, uint res );
	virtual void subtraction( uint arg1, uint arg2, uint res );
	virtual void product    ( uint arg1, uint arg2, uint res );
	virtual void quotient   ( uint arg1, uint arg2, uint res );
	virtual void power      ( uint arg1, uint arg2, uint res );
	virtual void powerInt   ( uint arg1, int arg2, uint res );

	virtual void Acos       ( uint arg, uint res );
	virtual void Asin       ( uint arg, uint res );
	virtual void Atan       ( uint arg, uint res );
	virtual void Cos        ( uint arg, uint res );
	virtual void Exp        ( uint arg, uint res );
	virtual void Log        ( uint arg, uint res );
	virtual void Sin        ( uint arg, uint res );
	virtual void Tan        ( uint arg, uint res );

	virtual void set        ( double val, uint res );

	virtual T get( uint index );

	std::vector<T> *result;
};



CLOSE_NAMESPACE_ACADO

#include <acado/symbolic_operator/operator.hpp>

BEGIN_NAMESPACE_ACADO



template <typename T> EvaluationTemplate<T>::EvaluationTemplate():EvaluationBase(){ result = 0; }
template <typename T> EvaluationTemplate<T>::EvaluationTemplate( std::vector<T> *res ):EvaluationBase()
{ result = res; }
template <typename T> EvaluationTemplate<T>::~EvaluationTemplate(){}

template <typename T> void EvaluationTemplate<T>::addition( uint arg1, uint arg2, uint res ){

	result->operator[](res) = result->operator[](arg1)+result->operator[](arg2);
}

template <typename T> void EvaluationTemplate<T>::subtraction( uint arg1, uint arg2, uint res ){

	result->operator[](res) = result->operator[](arg1)-result->operator[](arg2);
}

template <typename T> void EvaluationTemplate<T>::product( uint arg1, uint arg2, uint res ){

	result->operator[](res) = result->operator[](arg1)*result->operator[](arg2);
}

template <typename T> void EvaluationTemplate<T>::quotient( uint arg1, uint arg2, uint res ){

	result->operator[](res) = result->operator[](arg1)/result->operator[](arg2);
}

template <typename T> void EvaluationTemplate<T>::power( uint arg1, uint arg2, uint res ){

	result->operator[](res) = pow(result->operator[](arg1),result->operator[](arg2));
}

template <typename T> void EvaluationTemplate<T>::powerInt( uint arg1, int arg2, uint res ){

	result->operator[](res) = pow(result->operator[](arg1),arg2);
}


template <typename T> void EvaluationTemplate<T>::Acos( uint arg, uint res ){

	result->operator[](res) = acos(result->operator[](arg));
}

template <typename T> void EvaluationTemplate<T>::Asin( uint arg, uint res ){

	result->operator[](res) = asin(result->operator[](arg));
}

template <typename T> void EvaluationTemplate<T>::Atan( uint arg, uint res ){

	result->operator[](res) = atan(result->operator[](arg));
}

template <typename T> void EvaluationTemplate<T>::Cos( uint arg, uint res ){

	result->operator[](res) = cos(result->operator[](arg));
}

template <typename T> void EvaluationTemplate<T>::Exp( uint arg, uint res ){

	result->operator[](res) = exp(result->operator[](arg));
}

template <typename T> void EvaluationTemplate<T>::Log( uint arg, uint res ){

	result->operator[](res) = log(result->operator[](arg));
}

template <typename T> void EvaluationTemplate<T>::Sin( uint arg, uint res ){

	result->operator[](res) = sin(result->operator[](arg));
}

template <typename T> void EvaluationTemplate<T>::Tan( uint arg, uint res ){

	result->operator[](res) = tan(result->operator[](arg));
}

template <typename T> void EvaluationTemplate<T>::set( double val, uint res ){

	result->operator[](res) = val;
}


template <typename T> T  EvaluationTemplate<T>::get( uint index ){

	return result->operator[](index);
}

CLOSE_NAMESPACE_ACADO

#endif
