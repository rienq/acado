/*
 *    This file is part of ACADO Toolkit.
 *
 *    ACADO Toolkit -- A Toolkit for Automatic Control and Dynamic Optimization.
 *    Copyright (C) 2008-2013 by Boris Houska, Hans Joachim Ferreau,
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
 *    \file examples/function/symmetricAD.cpp
 *    \author Boris Houska, Rien Quirynen
 *    \date 2014
 */

#include <time.h>

#include <acado/utils/acado_utils.hpp>
#include <acado/user_interaction/user_interaction.hpp>
#include <acado/symbolic_expression/symbolic_expression.hpp>
#include <acado/function/function.hpp>
#include <acado/code_generation/export_algorithm_factory.hpp>

USING_NAMESPACE_ACADO

// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------
// SIMPLE AUXILIARY FUNCTION:
Expression symmetricDoubleProduct( const Expression& expr, const Expression& arg ) {

	uint dim = arg.getNumCols();
	uint dim2 = arg.getNumRows();

	IntermediateState inter_res = zeros<double>(dim2,dim);
	for( uint i = 0; i < dim; i++ ) {
		for( uint k1 = 0; k1 < dim2; k1++ ) {
			for( uint k2 = 0; k2 <= k1; k2++ ) {
				inter_res(k1,i) += expr(k1,k2)*arg(k2,i);
			}
			for( uint k2 = k1+1; k2 < dim2; k2++ ) {
				inter_res(k1,i) += expr(k2,k1)*arg(k2,i);
			}
		}
	}

	Expression new_expr( "", dim, dim );
	for( uint i = 0; i < dim; i++ ) {
		for( uint j = 0; j <= i; j++ ) {
			Expression new_tmp = 0;
			for( uint k1 = 0; k1 < dim2; k1++ ) {
				new_tmp = new_tmp+arg(k1,i)*inter_res(k1,j);
			}
			new_expr(i,j) = new_tmp;
			new_expr(j,i) = new_tmp;
		}
	}
	return new_expr;
}

Expression returnLowerTriangular( const Expression& expr ) {
	ASSERT( expr.getNumRows() == expr.getNumCols() );

	Expression new_expr;
	for( uint i = 0; i < expr.getNumRows(); i++ ) {
		for( uint j = 0; j <= i; j++ ) {
			new_expr << expr(i,j);
		}
	}
	return new_expr;
}
// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------


/* >>> start tutorial code >>> */
int main( ){
    
    DifferentialState   x1;     
    DifferentialState   x2;    
   		

    //
    // DEFINE THE MODEL EQUATIONS:
    //
    DifferentialEquation   f, f2;
    ExportAcadoFunction    fun, fun2;

    Expression states;
    states << x1;
    states << x2;
    
    Expression expr;
    expr << sin(x1*x2);
    
    f << symmetricDerivative(expr,states,eye<double>(2.2),1.0);
    
    Expression back = backwardDerivative(expr, states);
    f2 << forwardDerivative(back.transpose(), states);
    
    fun.init(f, "symmetric", 2, 0, 0, 0, 0, 0);
    
    fun2.init(f2, "FOB", 2, 0, 0, 0, 0, 0);
    
    std::ofstream stream( "ADtest/ADsymbolic_output.c" );
    fun.exportCode( stream, "double" );
    fun2.exportCode( stream, "double" );
    
    stream.close();

    return 0;
}
/* <<< end tutorial code <<< */

