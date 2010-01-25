// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// GenericVector.h: Rcpp R/C++ interface class library -- generic vectors (lists)
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

#ifndef Rcpp_GenericVector_h
#define Rcpp_GenericVector_h

#include <RcppCommon.h>
#include <Rcpp/wrap.h>
#include <Rcpp/SEXP_Vector.h>

namespace Rcpp{ 

typedef SEXP_Vector<VECSXP> GenericVector ;
typedef GenericVector List ;

#ifdef HAS_INIT_LISTS
inline GenericVector wrap(std::initializer_list<SEXP> list ){ return GenericVector(list) ; }
#endif

} // namespace

#endif
