// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// Rcpp_init.c : Rcpp R/C++ interface class library -- Initialize and register
//
// Copyright (C) 2010	      John Chambers, Dirk Eddelbuettel and Romain Francois
//
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
#include <R.h>
#include <Rinternals.h>
#include <R_ext/Rdynload.h>

SEXP CppField__get(SEXP, SEXP, SEXP);
SEXP CppField__set(SEXP, SEXP, SEXP, SEXP);


static R_CallMethodDef callEntries[]  = {
    {"CppField__get", (DL_FUNC) &CppField__get, 3},
       {"CppField__set", (DL_FUNC) &CppField__set, 4},
       {NULL, NULL, 0}
     }; 
 
void
R_init_SoDA(DllInfo *info)
{
  /* Register routines, allocate resources. */
  R_registerRoutines(info, NULL /* .C*/, callEntries /*.Call*/,
		     NULL /* .Fortran */, NULL /*.Extern*/);
}
          
void
R_unload_SoDA(DllInfo *info)
{
  /* Release resources. */
}
