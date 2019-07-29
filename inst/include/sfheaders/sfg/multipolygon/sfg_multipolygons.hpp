#ifndef R_SFHEADERS_SFG_MULTIPOLYGONS_H
#define R_SFHEADERS_SFG_MULTIPOLYGONS_H

#include <Rcpp.h>
#include "sfheaders/sfg/multipolygon/sfg_multipolygon.hpp"

namespace sfheaders {
namespace sfg {

  inline Rcpp::List sfg_multipolygons( Rcpp::List& lst ) {
    size_t n = lst.size();
    size_t i;
    Rcpp::List sfcs(n);

    for( i = 0; i < n; i++ ) {
      SEXP x = lst[i];
      sfcs[i] = sfheaders::sfg::sfg_multipolygon( x );
    }
    return sfcs;
  }


} // sfg
} // sfheaders

#endif