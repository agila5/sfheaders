#ifndef R_SFHEADERS_SFC_POINT_H
#define R_SFHEADERS_SFC_POINT_H

#include <Rcpp.h>
#include "sfheaders/sfc/sfc_attributes.hpp"
#include "sfheaders/sfc/sfc_types.hpp"
#include "sfheaders/sfc/zm_range.hpp"

#include "sfheaders/utils/utils.hpp"

#include "geometries/bbox/bbox.hpp"
#include "geometries/utils/columns/columns.hpp"
#include "geometries/geometries.hpp"

namespace sfheaders {
namespace sfc {

inline SEXP sfc_point(
    SEXP& x,
    SEXP& geometry_cols,
    std::string xyzm
) {

  // TODO:
  // if the input is a single vector
  // if( Rf_isVector( x ) ) {
  //   Rcpp::NumericMatrix nm( 1, Rf_length( x ) );
  //   nm( 0, Rcpp::_ ) = Rcpp::as< Rcpp::NumericVector >( x );
  //   return sfc_point( nm, geometry_cols, xyzm );
  // }

  if( Rf_isNull( geometry_cols ) ) {
    // Rcpp::Rcout << "sfc_point - null geometries" << std::endl;
    // Rcpp::Rcout << "type x: " << TYPEOF( x ) << std::endl;
    // Rcpp::Rcout << "is vec: " << Rf_isVector( x ) << std::endl;
    // Rcpp::Rcout << "is mat: " << Rf_isMatrix( x ) << std::endl;
    // Rcpp::Rcout << "type geom cols: " << TYPEOF( geometry_cols ) << std::endl;
    // make this all the other columns, then send back in
    SEXP geometry_cols2 = geometries::utils::other_columns( x );
    // Rcpp::Rcout << "geometry_col2" << std::endl;
    return sfc_point( x, geometry_cols2, xyzm );
  }

  int n_id_cols = 0;
  R_xlen_t col_counter = geometries::utils::sexp_length( geometry_cols );

  // After subset_geometries we have moved the geometry columns
  // into the 0:(n_geometry-1) positions
  //Rcpp::IntegerVector int_geometry_cols = Rcpp::seq( 0, ( col_counter - 1 ) );

  xyzm = sfheaders::utils::validate_xyzm( xyzm, col_counter );

  Rcpp::StringVector class_attribute = { xyzm.c_str(), "POINT","sfg" };
  Rcpp::List attributes = Rcpp::List::create(
    Rcpp::_["class"] = class_attribute
  );


  Rcpp::NumericVector bbox = sfheaders::bbox::start_bbox();
  Rcpp::NumericVector z_range = sfheaders::zm::start_z_range();
  Rcpp::NumericVector m_range = sfheaders::zm::start_m_range();
  geometries::bbox::calculate_bbox( bbox, x, geometry_cols );
  sfheaders::zm::calculate_zm_ranges( z_range, m_range, x, geometry_cols, xyzm );

  R_xlen_t required_cols = col_counter + n_id_cols;

  Rcpp::IntegerVector geometry_cols_int = geometries::utils::sexp_col_int( x, geometry_cols );

  Rcpp::List lst = geometries::utils::as_list( x );
  Rcpp::List res( required_cols );

  sfheaders::utils::subset_geometries( lst, res, geometry_cols_int );

  Rcpp::List sfc = geometries::make_geometries( res, attributes );

  // Rcpp::Rcout << "making_sfc" << std::endl;
  return sfheaders::sfc::make_sfc( sfc, sfheaders::sfc::SFC_POINT, bbox, z_range, m_range );

}


} // sfc
} // sfheaders

#endif
