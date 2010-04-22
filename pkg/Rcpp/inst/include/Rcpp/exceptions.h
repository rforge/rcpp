// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// exceptions.h: Rcpp R/C++ interface class library -- exceptions
//
// Copyright (C) 2010	Dirk Eddelbuettel and Romain Francois
//
// This file is part of Rcpp.
//
// Rcpp is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Rcpp is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Rcpp.  If not, see <http://www.gnu.org/licenses/>.

#ifndef Rcpp__exceptions__h
#define Rcpp__exceptions__h

namespace Rcpp{

#define RCPP_EXCEPTION_CLASS(__CLASS__,__WHAT__)                               \
class __CLASS__ : public std::exception{                                       \
public:                                                                        \
	__CLASS__( const std::string& message ) throw() : message( __WHAT__ ){} ;  \
	virtual ~__CLASS__() throw(){} ;                                           \
	virtual const char* what() const throw(){ return message.c_str() ; } ;     \
private:                                                                       \
	std::string message ;                                                      \
} ;

#define RCPP_SIMPLE_EXCEPTION_CLASS(__CLASS__,__MESSAGE__)                     \
class __CLASS__ : public std::exception{                                       \
public:                                                                        \
	__CLASS__() throw() {} ;                                                   \
	virtual ~__CLASS__() throw(){} ;                                           \
	virtual const char* what() const throw(){ return __MESSAGE__ ; } ;         \
} ;

RCPP_SIMPLE_EXCEPTION_CLASS(not_a_matrix, "not a matrix" )
RCPP_SIMPLE_EXCEPTION_CLASS(index_out_of_bounds, "index out of bounds" )
RCPP_SIMPLE_EXCEPTION_CLASS(parse_error, "parse error") 
RCPP_SIMPLE_EXCEPTION_CLASS(not_s4, "not an S4 object" )
RCPP_SIMPLE_EXCEPTION_CLASS(no_such_slot, "no such slot" )

RCPP_EXCEPTION_CLASS(not_compatible, message )
RCPP_EXCEPTION_CLASS(S4_creation_error, std::string("error creating object of S4 class : ") + message )
RCPP_EXCEPTION_CLASS(no_such_binding, std::string("no such binding : '") + message + "'" )

#undef RCPP_EXCEPTION_CLASS
#undef RCPP_SIMPLE_EXCEPTION_CLASS

} // namesapce Rcpp

#endif
