// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// SimpleVector.h: Rcpp R/C++ interface class library -- simple vectors (INTSXP,REALSXP,RAWSXP,LGLSXP,CPLXSXP)
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

#ifndef Rcpp_SimpleVector_h
#define Rcpp_SimpleVector_h

#include <RcppCommon.h>
#include <Rcpp/MatrixRow.h>
#include <Rcpp/MatrixColumn.h>
#include <Rcpp/VectorBase.h>
#include <Rcpp/r_cast.h>
#include <Rcpp/Dimension.h>

namespace Rcpp{

template <int RTYPE>
class SimpleVector : public VectorBase< SimpleVector<RTYPE> > {
public:
	
	typedef VectorBase< SimpleVector<RTYPE> > Base ;
	const static int r_type = RTYPE ;
	typedef typename traits::storage_type<RTYPE>::type value_type ;
	typedef value_type* iterator ;
	typedef value_type& reference ;
	
	SimpleVector() : Base(), start(0){}
	
	SimpleVector(SEXP x) throw(RObject::not_compatible) : Base(x), start(0) {
		update_vector() ;
	}
	
	SimpleVector( const size_t& size) : Base(size), start(0) {
		update_vector() ;
	}
	
	SimpleVector( const Dimension& dims) : Base(dims), start(0) {
		update_vector() ;
	}
	
	SimpleVector( const SimpleVector& other) : Base( other.asSexp() ), start(0) {
		update_vector() ;
	}
	
	SimpleVector& operator=(const SimpleVector& other){
		Base::setSEXP( other.asSexp() ) ;
		return *this ;
	}
	
	internal::ListInitialization<iterator,value_type> operator=( value_type x){
		*start = x ;
		return internal::ListInitialization<iterator,value_type>( start + 1 ) ; ;
	}
	
	template <typename InputIterator>
	SimpleVector( InputIterator first, InputIterator last) : Base(), start(0){
		assign( first, last ) ;
	}
	
#ifdef HAS_INIT_LISTS
	SimpleVector( std::initializer_list<value_type> list ) : Base(), start(0){
		assign( list.begin() , list.end() ) ;
	}
#endif

	inline reference operator[]( const int& i ){ return start[i] ; }
	inline reference operator[]( const std::string& name) {
		return start[ Base::offset(name) ];
	}
	inline iterator begin() const{ return start ; }
	inline iterator end() const{ return start+Rf_length(Base::m_sexp); }
	
	inline reference operator()( const size_t& i) throw(index_out_of_bounds){
		return start[ Base::offset(i) ] ;
	}
	inline reference operator()( const std::string& name) {
		return start[ Base::offset(name) ];
	}
	inline reference operator()( const size_t& i, const size_t& j) throw(not_a_matrix,index_out_of_bounds){
		return start[ Base::offset(i,j) ] ;
	}
	
	template <typename InputIterator>
	void assign( InputIterator first, InputIterator last){
		/* FIXME: we can do better than this r_cast to avoid 
		          allocating an unnecessary temporary object
		 */
		SEXP x = PROTECT( r_cast<RTYPE>( wrap( first, last ) ) );
		Base::setSEXP( x) ;
		UNPROTECT(1) ;
		update_vector() ;
	}
	
	
protected:
	void init(){
		internal::r_init_vector<RTYPE>(Base::m_sexp) ;
	}
	
	void update_vector(){
		start = internal::r_vector_start<RTYPE,value_type>(Base::m_sexp) ;
	}
	
private:
	value_type* start ;
	
	virtual void update(){ 
		update_vector() ;
	}
	
} ;

template <int RTYPE> 
class SimpleMatrix : public SimpleVector<RTYPE> {
public:
	SimpleMatrix() : SimpleVector<RTYPE>() {}
	
	SimpleMatrix(SEXP x) throw(RObject::not_compatible) : SimpleVector<RTYPE>(){
		if( ! ::Rf_isMatrix(x) ) throw RObject::not_compatible("not a matrix") ;
		SEXP y = r_cast<RTYPE>( x ) ;
		SimpleVector<RTYPE>::setSEXP( y );
	}
	
	SimpleMatrix( const Dimension& dims) : SimpleVector<RTYPE>() {
		if( dims.size() != 2 ) throw RObject::not_compatible("not a matrix") ;
		SimpleVector<RTYPE>::setSEXP( Rf_allocVector( RTYPE, dims.prod() ) ) ;
		SimpleVector<RTYPE>::init() ;
		SimpleVector<RTYPE>::attr( "dim" ) = dims ;
	}
	
	SimpleMatrix( const int& nrows, const int& ncols) : SimpleVector<RTYPE>() {
		SimpleVector<RTYPE>::setSEXP( Rf_allocVector( RTYPE, nrows*ncols ) ) ;
		SimpleVector<RTYPE>::init() ;
		SimpleVector<RTYPE>::attr( "dim" ) = Dimension( nrows, ncols ) ;
	}
	
	
	SimpleMatrix( const SimpleMatrix& other) : SimpleVector<RTYPE>() {
		SEXP x = other.asSexp() ;
		if( ! ::Rf_isMatrix(x) ) throw RObject::not_compatible("not a matrix") ;
		SimpleVector<RTYPE>::setSEXP( x ) ;
	}
	
	SimpleMatrix& operator=(const SimpleMatrix& other) {
		SEXP x = other.asSexp() ;
		if( ! ::Rf_isMatrix(x) ) throw RObject::not_compatible("not a matrix") ;
		SimpleVector<RTYPE>::setSEXP( x ) ;
		return *this ;
	}
	
private:
	virtual void update(){
		SimpleVector<RTYPE>::update_vector() ;
	}
	
} ;


typedef SimpleVector<CPLXSXP> ComplexVector ;
typedef SimpleVector<INTSXP> IntegerVector ;
typedef SimpleVector<LGLSXP> LogicalVector ;
typedef SimpleVector<REALSXP> NumericVector ;
typedef SimpleVector<RAWSXP> RawVector ;

typedef SimpleMatrix<CPLXSXP> ComplexMatrix ;
typedef SimpleMatrix<INTSXP> IntegerMatrix ;
typedef SimpleMatrix<LGLSXP> LogicalMatrix ;
typedef SimpleMatrix<REALSXP> NumericMatrix ;
typedef SimpleMatrix<RAWSXP> RawMatrix ;

}// namespace Rcpp

#endif
