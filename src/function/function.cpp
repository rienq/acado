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
 *    \file src/function/function.cpp
 *    \authors Boris Houska, Hans Joachim Ferreau, Milan Vukov
 *    \date 2008 - 2013
 */


#include <acado/utils/acado_utils.hpp>
#include <acado/symbolic_expression/symbolic_expression.hpp>
#include <acado/function/function_.hpp>

BEGIN_NAMESPACE_ACADO

//
// PUBLIC MEMBER FUNCTIONS:
//

Function::Function(){ }

Function::Function( const Expression &input, const Expression &output ){
  
   setInput(input);
   operator<<(output);
}

Function::Function( const Function& rhs ){ copy(rhs); }

Function::~Function( ){ }

Function& Function::operator=( const Function& rhs ){

  if( this != &rhs ) copy(rhs);
  return *this;
}

void Function::copy( const Function &arg ){
  
  f   = arg.f  ;
  sub = arg.sub;
  dep = arg.dep;
  in  = arg.in ;
  indices = arg.indices;
  globalExportVariableName = arg.globalExportVariableName;  
}

Function& Function::operator<<( const Expression &arg ){
  
	for( int i=0; i<arg.size(); ++i ){
		SharedOperatorVector nodes;
		arg(i).element->expandTree(dep,nodes);
		while( nodes.size() > 0 ) { // not done
			uint nSize = 0;
			while( nodes.size() > nSize ) { // nodes were added --> we are not at a leaf of the tree yet
				nSize = nodes.size();
				nodes.back()->expandTree(dep,nodes);
			}
			// we are at a leaf of the tree
			if( !nodes.back()->isIn(dep) ) {
				nodes.back()->addTo(dep,sub,nodes.back());
				nodes.back()->getArgumentList(dep,sub,indices);
			}
			nodes.erase(nodes.end());
		}
		f.push_back(arg(i).element);
	}
	setupAuxVector( );

    return *this;
}


Function Function::operator()( uint idx ) const{

    ASSERT( idx >= 0      );
    ASSERT( idx < size() );
    
    Function component;
    component << Expression(f[idx]);
    Expression input((int) in.size(), 1);
    for( int i=0; i<input.size(); ++i )
        input(i) = Expression(in[i]);
    component.setInput(input);
    return component;
}


uint Function::size() const{ return f.size(); }
    
returnValue Function::setInput( const Expression &input ){
  
   if( input.isVariable() == false )
       return ACADOERROR( RET_INVALID_ARGUMENTS );
  
   in.resize(input.size());
   for( int i=0; i<input.size();++i) in[i] = (input(i)).element;

   setupAuxVector( );

   return SUCCESSFUL_RETURN; 
}


BooleanType Function::isConstant() const{
    
    for( uint i=0; i<size();++i ) if(acadoIsFinite(f[i]->getValue()) == BT_FALSE ) return BT_FALSE;
    return BT_TRUE;
}


std::ostream& operator<<( std::ostream& stream, const Function &arg){ arg.print(stream,"fcn","double"); return stream; }

returnValue Function::print( std::ostream& stream, const char *fcnName, const char *realString ) const{

    stream << "/* This file was auto-generated by ACADO Toolkit. */" << std::endl << std::endl;

	exportForwardDeclarations(stream, fcnName, realString);
	stream << std::endl;
	exportCode(stream, fcnName, realString);

    return SUCCESSFUL_RETURN;
}

returnValue Function::exportForwardDeclarations( std::ostream& stream, const char *fcnName, const char *realString ) const{
  
    stream	<<
			"\n"
			"/** Export of an ACADO symbolic function.\n"
			" *\n"
			" *  \\param in Input to the exported function.\n"
			" *  \\param out Output of the exported function.\n"
			" */\n"
			<< "void " << fcnName << "(const " << realString << "* in, "
			<< realString << "* out);" << std::endl;
     return SUCCESSFUL_RETURN;
}

returnValue Function::exportCode( std::ostream& stream,
                                  const char *fcnName,
                                  const char *realString,
                                  bool       allocateMemory,
                                  bool       staticMemory ) const{

    using namespace std;

    stream << "void " << fcnName << "(const " << realString << "* in, " << realString << "* out)\n{\n";

    if (sub.size() > in.size())
    {
    	stream << "/* Vector of auxiliary variables; number of elements: " << sub.size()-in.size() << ". */" << endl;

    	if ( allocateMemory )
    	{
    		if ( staticMemory )
    		{
    			stream << "static ";
    		}
    		stream << realString << " a[" << sub.size()-in.size() << "];";
    	}
    	else
    		stream << realString << "* a = " << globalExportVariableName << ";";
    	stream << endl << endl;

    	stream << "/* Compute intermediate quantities: */" << endl;
    }

    StringMap auxVarIndividualNames;

    // DEFINE THE NAMES OF THE INPUT VARIABLES:
    // ----------------------------------------
    
    for( uint i=0; i<in.size(); ++i ){
         stringstream ss;
         ss << "in" << "[" << i << "]";
         auxVarIndividualNames[in[i].get()] = ss.str();
    }
    
    // DEFINE THE NAMES OF THE AUXILIARY VARIABLES:
    // --------------------------------------------
    
    for( uint i=0; i<auxVector.size(); ++i ){
    	stringstream ss;
    	ss << "a" << "[" << i << "]";
    	auxVarIndividualNames[sub[auxVector[i]].get()] = ss.str();
    }

    IoFormatter iof( stream );
    iof.set(16, iof.width, ios::scientific);

	// Export intermediate quantities
	for (uint i=0; i<auxVector.size(); ++i)
	{
		stream << "a[" << i << "] = ";
		sub[auxVector[i]]->print( stream, auxVarIndividualNames );
		stream << ";" << endl;
	}

	// Export output quantities
	stream << endl << "/* Compute outputs: */" << endl;
	for (uint i=0; i<size(); ++i)
	{
		stream << "out[" << i << "] = ";
		f[i]->print( stream, auxVarIndividualNames );
		stream << ";" << endl;
	}

	iof.reset();

	stream << "}" << endl << endl;
	
	
  return SUCCESSFUL_RETURN;
}

BooleanType Function::isSymbolic() const{
  
    for( uint i=0; i<sub.size();++i ) if(acadoIsFinite(sub[i]->isSymbolic()) == BT_FALSE ) return BT_FALSE;
    for( uint i=0; i<size()    ;++i ) if(acadoIsFinite(f  [i]->isSymbolic()) == BT_FALSE ) return BT_FALSE;
    return BT_TRUE;
}

returnValue Function::setGlobalExportVariableName(const std::string& var){

     if (var.size() == 0) return ACADOERROR( RET_INVALID_ARGUMENTS );

     globalExportVariableName = var;
     return SUCCESSFUL_RETURN;
}

std::string Function::getGlobalExportVariableName( ) const{ return globalExportVariableName; }

unsigned Function::getGlobalExportVariableSize( ) const{ return sub.size()-in.size(); }


void Function::setupAuxVector( ) {
	if(sub.size() == 0) {
		auxVector = std::vector<uint>();
	}
	else if(in.size() == 0) {
		auxVector = std::vector<uint>(sub.size());
		for( uint i=0; i<sub.size(); i++ ) {
			auxVector[i] = i;
		}
	}
	else {
		auxVector = std::vector<uint>(sub.size()-in.size());
		uint index = 0;
		for( uint i=0; i<sub.size(); i++ ) {
			if( std::find(in.begin(), in.end(), sub[i]) == in.end() ) { // sub[i] is not among the inputs
				auxVector[index] = i;
				index++;
			}
		}
	}
}

CLOSE_NAMESPACE_ACADO

// end of file.
