#ifndef R_SFHEADERS_SFG_MULTIPOLYGON_H
#define R_SFHEADERS_SFG_MULTIPOLYGON_H

#include <Rcpp.h>
#include "sfheaders/sfg/sfg_types.hpp"
#include "sfheaders/utils/utils.hpp"

#include "geometries/utils/utils.hpp"
#include "geometries/geometries.hpp"

namespace sfheaders {
namespace sfg {


  inline SEXP sfg_multipolygon(
      SEXP& x,
      SEXP& geometry_cols,
      SEXP& polygon_id,
      SEXP& linestring_id,
      std::string xyzm,
      bool close = true
  ) {

    // needs to go through the make_geoemtries()
    // but attributes are assigned at the end

    if( Rf_isNull( geometry_cols ) ) {
      // make this all the other columns, then send back in
      SEXP geometry_cols2 = geometries::utils::other_columns( x, polygon_id, linestring_id );
      return sfg_multipolygon( x, geometry_cols2, polygon_id, linestring_id, xyzm, close );
    }

    int n_id_cols = 2;
    R_xlen_t col_counter = geometries::utils::sexp_length( geometry_cols );

    // After subset_geometries we have moved the geometry columns
    // into the 0:(n_geometry-1) positions
    Rcpp::IntegerVector int_geometry_cols = Rcpp::seq( 0, ( col_counter - 1 ) );

    xyzm = sfheaders::utils::validate_xyzm( xyzm, col_counter );

    R_xlen_t required_cols = col_counter + n_id_cols;

    Rcpp::IntegerVector geometry_cols_int = geometries::utils::sexp_col_int( x, geometry_cols );

    Rcpp::List lst = geometries::utils::as_list( x );
    Rcpp::List res( required_cols );

    sfheaders::utils::subset_geometries( lst, res, geometry_cols_int );

    Rcpp::IntegerVector int_polygon_id(1);
    sfheaders::utils::resolve_id( x, polygon_id, int_polygon_id, res, lst, col_counter );

    Rcpp::IntegerVector int_linestring_id(1);
    sfheaders::utils::resolve_id( x, linestring_id, int_linestring_id, res, lst, col_counter );

    Rcpp::IntegerVector int_id_cols = geometries::utils::concatenate_vectors( int_polygon_id, int_linestring_id );

    Rcpp::List attributes = Rcpp::List::create();
    Rcpp::List sfg = geometries::make_geometries( res, int_id_cols, int_geometry_cols, attributes, close );


    Rcpp::StringVector class_attribute = { xyzm.c_str(), "MULTIPOLYGON","sfg" };
    Rcpp::List atts = Rcpp::List::create(
      Rcpp::_["class"] = class_attribute
    );
    geometries::utils::attach_attributes( sfg, atts );

    return sfg;

  }

//
//   // only keep the outer-linestring / ring / matrix
//   inline SEXP remove_multipolygon_holes(
//       Rcpp::List& sfg_mp,
//       std::string xyzm,
//       bool close = true
//   ) {
//     // loop over and only keep the first line
//     R_xlen_t i;
//     R_xlen_t n = sfg_mp.size();
//     Rcpp::List res( n );
//     for( i = 0; i < n; ++i ) {
//       Rcpp::List poly = sfg_mp[ i ];
//       Rcpp::List new_poly(1);
//       new_poly[ 0 ] = poly[ 0 ];
//       res[ i ] = new_poly;
//     }
//     return sfg_multipolygon( res, xyzm, close);
//
//   }

} // sfg
} // sfheaders

#endif
