\name{RcppVector}
\alias{RcppVector}
\alias{RcppMatrix}
\alias{RcppVectorView}
\alias{RcppMatrixView}
\alias{RcppStringVector}
\alias{RcppVectorExample}
\alias{RcppMatrixExample}
\alias{RcppStringVectorExample}
\title{C++ classes for receiving R object in C++ -- deprecated API}
\description{
  \code{RcppVector}, \code{RcppMatrix} and \code{RcppStringVector} are
  C++ classes that can pass vectors (matrices) of \R objects of
  appropriate types to C++ via the \code{.Call()} function interface. 
  They are part of the 'classic' Rcpp API. 
  
  The vector and matrix types are templated and can operate on \R types
  \code{intger} and \code{numeric}. 

  The \code{RcppVectorView} and \code{RcppMatrixView} are slighly more
  lightweight read-only variants.
  
  Member functions are provided to query the dimension of the vector or
  matrix object, convert it in a corresponding \code{C} representation,
  and also to convert it into a corresponding STL object.

  The new API has classes \code{NumericVector}, \code{NumericMatrix},
  \code{CharacterVector} (and also an alias \code{StringVector}).

  The files \code{RcppVectorExample.cpp} and
  \code{RcppMatrixExample.cpp} provide examples for both the classic and
  new APIs.

  Note that the \pkg{RcppClassic} package has been deprecated since
  2010, all new development should use the \pkg{Rcpp} package instead.
}
\details{
  Usage of \code{RcppVector}, \code{RcppMatrix} and
  \code{RcppStringVector} in \code{C++} is fully defined in
  the respective header files. 

  As example, consider a call from \R to \code{C++} such as

  \preformatted{
  # an R example passing one type of each class to a function
  # someFunction in package somePackage
  val <- .Call("someFunction",
               rnorm(100),        # numeric vector
	       sample(1:10, 5, TRUE)  # int vector
	       search(),          # character vector
	       as.matrix(rnorm(100),10,10), # matrix
   	       PACKAGE="somePackage")
  }

  At the \code{C++} level, the corresponding code to assign these parameter to
  \code{C++} objects is can be as follows (taken from the C++ source of
  \code{RcppExample}):
  \preformatted{%
  SEXP someFunction(SEXP nvec, SEXP ivec,
                    SEXP svec, SEXP nmat) {

    RcppVector<double> nv(nvec);  		      
    RcppVector<int>    iv(ivec);
    RcppStringVector   sv(svec);
    RcppMatrix<double> nm(nmat);
  }
  }

  These \code{C++} objects could then be queried via
  \preformatted{%
    int n = nv.size();
    int d1 = nm.dim1(), d2 = nm.dim2();
  }
  to retrieve, respectively, vector length and matrix dimensions.

  Moreover, the \code{stlVector()} and \code{stlMatrix()} member
  functions can be used to convert the objects into STL objects:
  \preformatted{%
    vector<int> ivstl = iv.stlVector();
    vector< vector< double > > = nm.stlMatrix();
  }

}
\seealso{
  \code{RcppExample}.

  See the \link[RcppExamples]{RcppExamples-package} for examples of the
  recommended \pkg{Rcpp} API and \link[Rcpp]{Rcpp-package} for
  documentation on the recommended API to extend R with C++ code, while
  the deprecated \link[RcppClassic]{RcppClassic-package} documents the
  older, deprecated API.
}
\author{Dominick Samperi wrote the initial versions of Rcpp (and
  RcppTemplate) during 2005 and 2006.  Dirk Eddelbuettel made some
  additions, and became maintainer in 2008. Dirk Eddelbuettel and Romain
  Francois have been extending Rcpp since 2009.
}
\examples{
# set up some value
vector <- (seq(1,9))^2

# call the underlying  C++ function
result <- RcppVectorExample(vector)

# inspect returned object
result
}
\keyword{programming}
\keyword{interface}
