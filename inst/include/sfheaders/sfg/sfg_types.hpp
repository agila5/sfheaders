#ifndef R_SFHEADERS_SFG_TYPES_H
#define R_SFHEADERS_SFG_TYPES_H

#include <Rcpp.h>

#include "sfheaders/sfg/sfg_attributes.hpp"
#include "sfheaders/sfg/sfg_dimension.hpp"

namespace sfheaders {
namespace sfg {

  const int SFG_POINT              = 1;
  const int SFG_MULTIPOINT         = 2;
  const int SFG_LINESTRING         = 3;
  const int SFG_MULTILINESTRING    = 4;
  const int SFG_POLYGON            = 5;
  const int SFG_MULTIPOLYGON       = 6;
  const int SFG_GEOMETRYCOLLECTION = 7;

  const int VECTOR              = 1;
  const int MATRIX              = 2;
  const int LIST_MATRIX         = 3;
  const int LIST_LIST_MATRIX    = 4;

  inline std::string get_sfg_type( int sfg_type ) {
    switch( sfg_type ) {
    case SFG_POINT: { return "POINT"; }
    case SFG_MULTIPOINT: { return "MULTIPOINT"; }
    case SFG_LINESTRING: { return "LINESTRING"; }
    case SFG_MULTILINESTRING: { return "MULTILINESTRING"; }
    case SFG_POLYGON: { return "POLYGON"; }
    case SFG_MULTIPOLYGON: { return "MULTIPOLYGON"; }
    default: {
      Rcpp::stop("sfheaders - unknown sfg type");
    }
    }
    return ""; // never reaches
  }

  inline int get_sfg_type( std::string& sfg ) {
    if( sfg == "POINT" ) {
      return sfheaders::sfg::SFG_POINT;
    } else if ( sfg == "MULTIPOINT" ) {
      return sfheaders::sfg::SFG_MULTIPOINT;
    } else if ( sfg == "LINESTRING" ) {
      return sfheaders::sfg::SFG_LINESTRING;
    } else if ( sfg == "MULTILINESTRING" ) {
      return sfheaders::sfg::SFG_MULTILINESTRING;
    } else if ( sfg == "POLYGON" ) {
      return sfheaders::sfg::SFG_POLYGON;
    } else if ( sfg == "MULTIPOLYGON" ) {
      return sfheaders::sfg::SFG_MULTIPOLYGON;
    } else if ( sfg == "GEOMETRYCOLLECTION" ) {
      return sfheaders::sfg::SFG_GEOMETRYCOLLECTION;
    } else {
      Rcpp::stop("sfheaders - unknown sfg type");  // #nocov
    }
  }

  inline void make_sfg(
    Rcpp::IntegerVector& iv,
    int sfg_type,
    std::string& xyzm
  ) {
    R_xlen_t n_col = iv.length();
    std::string dim = sfheaders::sfg::sfg_dimension( n_col, xyzm );

    std::string geom_type = get_sfg_type( sfg_type );
    iv.attr("class") = sfheaders::sfg::sfg_attributes( dim, geom_type );
  }

  inline void make_sfg(
      Rcpp::NumericVector& nv,
      int sfg_type,
      std::string& xyzm
  ) {
    R_xlen_t n_col = nv.length();
    std::string dim = sfheaders::sfg::sfg_dimension( n_col, xyzm );

    std::string geom_type = get_sfg_type( sfg_type );
    nv.attr("class") = sfheaders::sfg::sfg_attributes( dim, geom_type );
  }

  inline void make_sfg(
    Rcpp::IntegerMatrix& im,
    int sfg_type,
    std::string& xyzm
  ) {

    R_xlen_t n_col = im.ncol();
    std::string dim = sfheaders::sfg::sfg_dimension( n_col, xyzm );

    std::string geom_type = get_sfg_type( sfg_type );
    im.attr("class") = sfheaders::sfg::sfg_attributes( dim, geom_type );
  }

  inline void make_sfg(
      Rcpp::NumericMatrix& nm,
      int sfg_type,
      std::string& xyzm
  ) {

    R_xlen_t n_col = nm.ncol();
    std::string dim = sfheaders::sfg::sfg_dimension( n_col, xyzm );

    std::string geom_type = get_sfg_type( sfg_type );
    nm.attr("class") = sfheaders::sfg::sfg_attributes( dim, geom_type );
  }

  inline void make_sfg(
      Rcpp::List& lst,
      int sfg_type,
      std::string& xyzm
  ) {
    std::string dim = sfheaders::sfg::sfg_dimension( lst, xyzm );

    std::string geom_type = get_sfg_type( sfg_type );
    lst.attr("class") = sfheaders::sfg::sfg_attributes( dim, geom_type );
  }

  inline void make_sfg(
    Rcpp::List& lst,
    R_xlen_t n_col,
    int sfg_type,
    std::string& xyzm
  ) {
    std::string dim = sfheaders::sfg::sfg_dimension( n_col, xyzm );

    std::string geom_type = get_sfg_type( sfg_type );
    lst.attr("class") = sfheaders::sfg::sfg_attributes( dim, geom_type );
  }



} // sfg
} // sfheaders


#endif
