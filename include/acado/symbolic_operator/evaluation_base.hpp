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
 *    \file include/acado/symbolic_operator/evaluation_base.hpp
 *    \author Boris Houska
 */


#ifndef ACADO_TOOLKIT_EVALUATION_BASE_HPP
#define ACADO_TOOLKIT_EVALUATION_BASE_HPP


#include <acado/symbolic_operator/symbolic_operator_fwd.hpp>


BEGIN_NAMESPACE_ACADO


/**
 *	\brief Abstract base class for templated evaluation of operators.
 *
 *	\ingroup BasicDataStructures
 *
 *	\author Boris Houska
 */

class EvaluationBase{

public:

	/** Default constructor. */
	EvaluationBase(){};

	virtual ~EvaluationBase(){};

	virtual void addition   ( uint arg1, uint arg2, uint res ) = 0;
	virtual void subtraction( uint arg1, uint arg2, uint res ) = 0;
	virtual void product    ( uint arg1, uint arg2, uint res ) = 0;
	virtual void quotient   ( uint arg1, uint arg2, uint res ) = 0;
	virtual void power      ( uint arg1, uint arg2, uint res ) = 0;
	virtual void powerInt   ( uint arg1, int arg2, uint res ) = 0;

	virtual void Acos       ( uint arg, uint res ) = 0;
	virtual void Asin       ( uint arg, uint res ) = 0;
	virtual void Atan       ( uint arg, uint res ) = 0;
	virtual void Cos        ( uint arg, uint res ) = 0;
	virtual void Exp        ( uint arg, uint res ) = 0;
	virtual void Log        ( uint arg, uint res ) = 0;
	virtual void Sin        ( uint arg, uint res ) = 0;
	virtual void Tan        ( uint arg, uint res ) = 0;

	virtual void set        ( double val, uint res ) = 0;

};

CLOSE_NAMESPACE_ACADO

#endif
