#ifndef R_SFHEADERS_SFG_MULTILINESTRING_H
#define R_SFHEADERS_SFG_MULTILINESTRING_H

#include <Rcpp.h>
#include "sfheaders/sfg/sfg_types.hpp"
#include "sfheaders/utils/utils.hpp"

#include "geometries/utils/utils.hpp"
#include "geometries/geometries.hpp"

namespace sfheaders {
namespace sfg {

inline SEXP sfg_multilinestring(
    SEXP& x,
    SEXP& geometry_cols,
    SEXP& linestring_id,
    std::string xyzm
) {

  // needs to go through the make_geoemtries()
  // but attributes are assigned at the end

  if( Rf_isNull( geometry_cols ) ) {
    // make this all the other columns, then send back in
    SEXP geometry_cols2 = geometries::utils::other_columns( x, linestring_id );
    return sfg_multilinestring( x, geometry_cols2, linestring_id, xyzm );
  }

  int n_id_cols = 1;
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

  Rcpp::IntegerVector int_linestring_id(1);

  sfheaders::utils::resolve_id( x, linestring_id, int_linestring_id, res, lst, col_counter );

  Rcpp::List attributes = Rcpp::List::create();
  Rcpp::List sfg = geometries::make_geometries( res, int_linestring_id, int_geometry_cols, attributes );


  Rcpp::StringVector class_attribute = { xyzm.c_str(), "MULTILINESTRING","sfg" };
  Rcpp::List atts = Rcpp::List::create(
    Rcpp::_["class"] = class_attribute
  );
  geometries::utils::attach_attributes( sfg, atts );

  return sfg;

  //sfheaders::sfg::make_sfg( sfg, sfheaders::sfg::SFG_LINESTRING, xyzm );

  //return sfheaders::sfc::make_sfc( sfc, sfheaders::sfc::SFC_LINESTRING, bbox, z_range, m_range );

  // SEXP geometry_mat = geometries::matrix::to_matrix( x, geometry_cols );
  // R_xlen_t n_col = geometries::utils::sexp_n_col( geometry_mat );
  // xyzm = sfheaders::utils::validate_xyzm( xyzm, n_col );
  // sfheaders::sfg::make_sfg( geometry_mat, n_col, sfheaders::sfg::SFG_LINESTRING, xyzm );
  // return geometry_mat;

  return Rcpp::List::create();

}

  // // multilinestring is a list of linestrings (matrices)
  // // but can also be a single matrix
  // inline SEXP sfg_multilinestring(
  //     Rcpp::IntegerMatrix& im,
  //     std::string xyzm
  // ) {
  //   Rcpp::List mls( 1 );
  //   mls[0] = im;
  //   R_xlen_t n_col = im.ncol();
  //   sfheaders::sfg::make_sfg( mls, n_col, sfheaders::sfg::SFG_MULTILINESTRING, xyzm );
  //
  //   return mls;
  // }
  //
  // inline SEXP sfg_multilinestring(
  //     Rcpp::IntegerVector& iv,
  //     std::string xyzm
  // ) {
  //   R_xlen_t n = iv.length();
  //   Rcpp::IntegerMatrix im( 1, n );
  //   im( 0, Rcpp::_ ) = iv;
  //   return sfg_multilinestring( im, xyzm );
  // }
  //
  // inline SEXP sfg_multilinestring(
  //     Rcpp::NumericMatrix& nm,
  //     std::string xyzm
  // ) {
  //   Rcpp::List mls( 1 );
  //   mls[0] = nm;
  //   R_xlen_t n_col = nm.ncol();
  //   sfheaders::sfg::make_sfg( mls, n_col, sfheaders::sfg::SFG_MULTILINESTRING, xyzm );
  //   return mls;
  // }
  //
  // inline SEXP sfg_multilinestring(
  //     Rcpp::NumericVector& nv,
  //     std::string xyzm
  // ) {
  //   R_xlen_t n = nv.length();
  //   Rcpp::NumericMatrix nm( 1, n );
  //   nm( 0, Rcpp::_ ) = nv;
  //   return sfg_multilinestring( nm, xyzm );
  // }
  //
  // inline SEXP sfg_multilinestring(
  //     Rcpp::DataFrame& df,
  //     std::string xyzm
  // ) {
  //   Rcpp::List mls( 1 );
  //   Rcpp::NumericMatrix nm = geometries::utils::df_to_matrix( df );
  //   mls[0] = nm;
  //   R_xlen_t n_col = nm.ncol();
  //   sfheaders::sfg::make_sfg( mls, n_col, sfheaders::sfg::SFG_MULTILINESTRING, xyzm );
  //   return mls;
  // }
  //
  // inline SEXP sfg_multilinestring(
  //   Rcpp::List& lst,
  //   std::string xyzm
  // ) {
  //   sfheaders::sfg::make_sfg( lst, sfheaders::sfg::SFG_MULTILINESTRING, xyzm );
  //   return lst;
  // }
  //
  //
  // inline SEXP sfg_multilinestring(
  //     Rcpp::DataFrame& df,
  //     Rcpp::IntegerVector& cols,
  //     std::string xyzm
  // ) {
  //   Rcpp::NumericMatrix nm = geometries::shapes::to_mat( df, cols );
  //   return sfg_multilinestring( nm, xyzm );
  // }
  //
  // inline SEXP sfg_multilinestring(
  //     Rcpp::DataFrame& df,
  //     Rcpp::StringVector& cols,
  //     std::string xyzm
  // ) {
  //   Rcpp::NumericMatrix nm = geometries::shapes::to_mat( df, cols );
  //   return sfg_multilinestring( nm, xyzm );
  // }
  //
  // inline SEXP sfg_multilinestring(
  //     Rcpp::NumericMatrix& nm,
  //     Rcpp::IntegerVector& cols,
  //     std::string xyzm
  // ) {
  //   Rcpp::NumericMatrix nm2 = geometries::shapes::to_mat( nm, cols );
  //   return sfg_multilinestring( nm2, xyzm );
  // }
  //
  // inline SEXP sfg_multilinestring(
  //     Rcpp::NumericMatrix& nm,
  //     Rcpp::StringVector& cols,
  //     std::string xyzm
  // ) {
  //   Rcpp::NumericMatrix nm2 = geometries::shapes::to_mat( nm, cols );
  //   return sfg_multilinestring( nm2, xyzm );
  // }
  //
  // inline SEXP sfg_multilinestring(
  //     Rcpp::IntegerMatrix& im,
  //     Rcpp::IntegerVector& cols,
  //     std::string xyzm
  // ) {
  //   Rcpp::IntegerMatrix im2 = geometries::shapes::to_mat( im, cols );
  //   return sfg_multilinestring( im2, xyzm );
  // }
  //
  // inline SEXP sfg_multilinestring(
  //     Rcpp::IntegerMatrix& im,
  //     Rcpp::StringVector& cols,
  //     std::string xyzm
  // ) {
  //   Rcpp::IntegerMatrix im2 = geometries::shapes::to_mat( im, cols );
  //   return sfg_multilinestring( im2, xyzm );
  // }
  //
  // // We're still on single sfg objects.
  // // when we go up to sfc objects then we will need multiline_id & line_id
  // inline SEXP sfg_multilinestring(
  //   Rcpp::DataFrame& df,
  //   Rcpp::IntegerVector& cols,
  //   int& id_col,
  //   std::string xyzm
  // ) {
  //   Rcpp::List lst = geometries::shapes::to_listMat( df, cols, id_col );
  //   return sfg_multilinestring( lst, xyzm );
  // }
  //
  // inline SEXP sfg_multilinestring(
  //   Rcpp::DataFrame& df,
  //   Rcpp::StringVector& cols,
  //   Rcpp::String& id_col,
  //   std::string xyzm
  // ) {
  //   Rcpp::List lst = geometries::shapes::to_listMat( df, cols, id_col );
  //   return sfg_multilinestring( lst, xyzm );
  // }
  //
  // inline SEXP sfg_multilinestring(
  //     Rcpp::NumericMatrix& nm,
  //     Rcpp::IntegerVector& cols,
  //     int& id_col,
  //     std::string xyzm
  // ) {
  //   Rcpp::List lst = geometries::shapes::to_listMat( nm, cols, id_col );
  //   return sfg_multilinestring( lst, xyzm );
  // }
  //
  // inline SEXP sfg_multilinestring(
  //     Rcpp::NumericMatrix& nm,
  //     Rcpp::StringVector& cols,
  //     Rcpp::String& id_col,
  //     std::string xyzm
  // ) {
  //   Rcpp::List lst = geometries::shapes::to_listMat( nm, cols, id_col );
  //   return sfg_multilinestring( lst, xyzm );
  // }
  //
  // inline SEXP sfg_multilinestring(
  //     Rcpp::IntegerMatrix& im,
  //     Rcpp::IntegerVector& cols,
  //     int& id_col,
  //     std::string xyzm
  // ) {
  //   Rcpp::List lst = geometries::shapes::to_listMat( im, cols, id_col );
  //   return sfg_multilinestring( lst, xyzm );
  // }
  //
  // // #nocov start
  // inline SEXP sfg_multilinestring(
  //     Rcpp::IntegerMatrix& im,
  //     Rcpp::StringVector& cols,
  //     Rcpp::String& id_col,
  //     std::string xyzm
  // ) {
  //   Rcpp::List lst = geometries::shapes::to_listMat( im, cols, id_col );
  //   return sfg_multilinestring( lst, xyzm );
  // }
  // // #nocov end
  //
  // inline SEXP sfg_multilinestring(
  //     SEXP& x,
  //     Rcpp::IntegerVector& cols,
  //     std::string xyzm
  // ) {
  //   switch( TYPEOF( x ) ) {
  //   case INTSXP: {
  //     if( !Rf_isMatrix( x ) ) {
  //     Rcpp::stop("sfheaders - expecting a matrix");  // #nocov
  //   }
  //     Rcpp::IntegerMatrix im = Rcpp::as< Rcpp::IntegerMatrix >( x );
  //     return sfg_multilinestring( im, cols, xyzm );
  //   }
  //   case REALSXP: {
  //     if( !Rf_isMatrix( x ) ) {
  //     Rcpp::stop("sfheaders - expecting a matrix");  // #nocov
  //   }
  //     Rcpp::NumericMatrix nm = Rcpp::as< Rcpp::NumericMatrix >( x );
  //     return sfg_multilinestring( nm, cols, xyzm );
  //   }
  //   case VECSXP: {
  //     if( Rf_inherits( x, "data.frame") ) {
  //     Rcpp::DataFrame df = Rcpp::as< Rcpp::DataFrame >( x );
  //     return sfg_multilinestring( df, cols, xyzm );
  //   } // else default
  //   }
  //   default: {
  //     Rcpp::stop("sfheaders - unsupported sfg_LINESTRING type");  // #nocov
  //   }
  //   }
  //
  //   return Rcpp::List::create(); // never reaches
  // }
  //
  //
  //
  // inline SEXP sfg_multilinestring(
  //     SEXP& x,
  //     Rcpp::IntegerVector& cols,
  //     int& line_id,
  //     std::string xyzm
  // ) {
  //   switch( TYPEOF( x ) ) {
  //   case INTSXP: {
  //     if( !Rf_isMatrix( x ) ) {
  //     Rcpp::stop("sfheaders - expecting a matrix");  // #nocov
  //   }
  //     Rcpp::IntegerMatrix im = Rcpp::as< Rcpp::IntegerMatrix >( x );
  //     return sfg_multilinestring( im, cols, line_id, xyzm );
  //   }
  //   case REALSXP: {
  //     if( !Rf_isMatrix( x ) ) {
  //     Rcpp::stop("sfheaders - expecting a matrix");  // #nocov
  //   }
  //     Rcpp::NumericMatrix nm = Rcpp::as< Rcpp::NumericMatrix >( x );
  //     return sfg_multilinestring( nm, cols, line_id, xyzm );
  //   }
  //   case VECSXP: {
  //     if( Rf_inherits( x, "data.frame") ) {
  //     Rcpp::DataFrame df = Rcpp::as< Rcpp::DataFrame >( x );
  //     return sfg_multilinestring( df, cols, line_id, xyzm );
  //   } // else default
  //   }
  //   default: {
  //     Rcpp::stop("sfheaders - unsupported sfg_LINESTRING type"); // #nocov
  //   }
  //   }
  //
  //   return Rcpp::List::create(); // never reaches
  // }
  //
  //
  // // inline SEXP sfg_multilinestring(
  // //     SEXP& x,
  // //     Rcpp::IntegerVector& cols,
  // //     Rcpp::String& line_id
  // // ) {
  // //   switch( TYPEOF( x ) ) {
  // //   case INTSXP: {
  // //     if( !Rf_isMatrix( x ) ) {
  // //     Rcpp::stop("sfheaders - expecting a matrix");
  // //   }
  // //     Rcpp::IntegerMatrix im = Rcpp::as< Rcpp::IntegerMatrix >( x );
  // //     return sfg_multilinestring( im, cols, line_id );
  // //   }
  // //   case REALSXP: {
  // //     if( !Rf_isMatrix( x ) ) {
  // //     Rcpp::stop("sfheaders - expecting a matrix");
  // //   }
  // //     Rcpp::NumericMatrix nm = Rcpp::as< Rcpp::NumericMatrix >( x );
  // //     return sfg_multilinestring( nm, cols, line_id );
  // //   }
  // //   case VECSXP: {
  // //     if( Rf_inherits( x, "data.frame") ) {
  // //     Rcpp::DataFrame df = Rcpp::as< Rcpp::DataFrame >( x );
  // //     return sfg_multilinestring( df, cols, line_id );
  // //   } // else default
  // //   }
  // //   default: {
  // //     Rcpp::stop("sfheaders - unsupported sfg_LINESTRING type");
  // //   }
  // //   }
  // //
  // //   return Rcpp::List::create(); // never reaches
  // // }
  //
  //
  // inline SEXP sfg_multilinestring(
  //     SEXP& x,
  //     Rcpp::StringVector& cols,
  //     std::string xyzm
  // ) {
  //   switch( TYPEOF( x ) ) {
  //   case INTSXP: {
  //     if( !Rf_isMatrix( x ) ) {
  //     Rcpp::stop("sfheaders - expecting a matrix");  // #nocov
  //   }
  //     Rcpp::IntegerMatrix im = Rcpp::as< Rcpp::IntegerMatrix >( x );
  //     return sfg_multilinestring( im, cols, xyzm );
  //   }
  //   case REALSXP: {
  //     if( !Rf_isMatrix( x ) ) {
  //     Rcpp::stop("sfheaders - expecting a matrix");  // #nocov
  //   }
  //     Rcpp::NumericMatrix nm = Rcpp::as< Rcpp::NumericMatrix >( x );
  //     return sfg_multilinestring( nm, cols, xyzm );
  //   }
  //   case VECSXP: {
  //     if( Rf_inherits( x, "data.frame") ) {
  //     Rcpp::DataFrame df = Rcpp::as< Rcpp::DataFrame >( x );
  //     return sfg_multilinestring( df, cols, xyzm );
  //   } // else default
  //   }
  //   default: {
  //     Rcpp::stop("sfheaders - unsupported sfg_MULTILINESTRING type");   // #nocov
  //   }
  //   }
  //
  //   return Rcpp::List::create(); // never reaches
  // }
  //
  // // inline SEXP sfg_multilinestring(
  // //     SEXP& x,
  // //     Rcpp::StringVector& cols,
  // //     int& line_id
  // // ) {
  // //   switch( TYPEOF( x ) ) {
  // //   case INTSXP: {
  // //     if( !Rf_isMatrix( x ) ) {
  // //     Rcpp::stop("sfheaders - expecting a matrix");
  // //   }
  // //     Rcpp::IntegerMatrix im = Rcpp::as< Rcpp::IntegerMatrix >( x );
  // //     return sfg_multilinestring( im, cols, line_id );
  // //   }
  // //   case REALSXP: {
  // //     if( !Rf_isMatrix( x ) ) {
  // //     Rcpp::stop("sfheaders - expecting a matrix");
  // //   }
  // //     Rcpp::NumericMatrix nm = Rcpp::as< Rcpp::NumericMatrix >( x );
  // //     return sfg_multilinestring( nm, cols, line_id );
  // //   }
  // //   case VECSXP: {
  // //     if( Rf_inherits( x, "data.frame") ) {
  // //     Rcpp::DataFrame df = Rcpp::as< Rcpp::DataFrame >( x );
  // //     return sfg_multilinestring( df, cols, line_id );
  // //   } // else default
  // //   }
  // //   default: {
  // //     Rcpp::stop("sfheaders - unsupported sfg_MULTILINESTRING type");
  // //   }
  // //   }
  // //
  // //   return Rcpp::List::create(); // never reaches
  // // }
  //
  // inline SEXP sfg_multilinestring(
  //     SEXP& x,
  //     Rcpp::StringVector& cols,
  //     Rcpp::String& line_id,
  //     std::string xyzm
  // ) {
  //   switch( TYPEOF( x ) ) {
  //   case INTSXP: {
  //     if( !Rf_isMatrix( x ) ) {
  //     Rcpp::stop("sfheaders - expecting a matrix");  // #nocov
  //   }
  //     Rcpp::IntegerMatrix im = Rcpp::as< Rcpp::IntegerMatrix >( x );
  //     return sfg_multilinestring( im, cols, line_id, xyzm );
  //   }
  //   case REALSXP: {
  //     if( !Rf_isMatrix( x ) ) {
  //     Rcpp::stop("sfheaders - expecting a matrix");  // #nocov
  //   }
  //     Rcpp::NumericMatrix nm = Rcpp::as< Rcpp::NumericMatrix >( x );
  //     return sfg_multilinestring( nm, cols, line_id, xyzm );
  //   }
  //   case VECSXP: {
  //     if( Rf_inherits( x, "data.frame") ) {
  //     Rcpp::DataFrame df = Rcpp::as< Rcpp::DataFrame >( x );
  //     return sfg_multilinestring( df, cols, line_id, xyzm );
  //   } // else default
  //   }
  //   default: {
  //     Rcpp::stop("sfheaders - unsupported sfg_MULTILINESTRING type");   // #nocov
  //   }
  //   }
  //
  //   return Rcpp::List::create(); // never reaches
  // }
  //
  //
  // inline SEXP sfg_multilinestring(
  //     SEXP& x,
  //     std::string xyzm
  // ) {
  //   switch ( TYPEOF( x ) ) {
  //   case INTSXP: {
  //   if( !Rf_isMatrix( x ) ) { // #nocov
  //     Rcpp::IntegerVector iv = Rcpp::as< Rcpp::IntegerVector >( x ); // #nocov
  //     return sfg_multilinestring( iv, xyzm );
  //   }
  //     Rcpp::IntegerMatrix im = Rcpp::as< Rcpp::IntegerMatrix >( x );
  //     return sfg_multilinestring( im, xyzm );
  //   }
  //   case REALSXP: {
  //   if( !Rf_isMatrix( x ) ) {
  //     Rcpp::NumericVector nv = Rcpp::as< Rcpp::NumericVector >( x );
  //     return sfg_multilinestring( nv, xyzm );
  //   }
  //     Rcpp::NumericMatrix nm = Rcpp::as< Rcpp::NumericMatrix >( x );
  //     return sfg_multilinestring( nm, xyzm );
  //   }
  //   case VECSXP: {
  //     if( Rf_inherits( x, "data.frame") ) {
  //     Rcpp::DataFrame df = Rcpp::as< Rcpp::DataFrame >( x );
  //     return sfg_multilinestring( df, xyzm );
  //   } else {
  //     Rcpp::List lst = Rcpp::as< Rcpp::List >( x ); // #nocov
  //     return sfg_multilinestring( lst, xyzm );            // #nocov
  //   }
  //   }
  //   default: {
  //     Rcpp::stop("sfheaders - unsupported sfg_MULTILINESTRING type");  // #nocov
  //   }
  //   }
  //   return x; // never reaches
  // }
  //
  //
  // inline SEXP sfg_multilinestring(
  //     SEXP& x,
  //     SEXP& geometry_cols,
  //     std::string xyzm
  // ) {
  //   if( Rf_isNull( geometry_cols ) ) {
  //     return sfg_multilinestring( x, xyzm );   // #nocov
  //   }
  //   switch( TYPEOF( geometry_cols ) ) {
  //   case REALSXP: {}
  //   case INTSXP: {
  //     Rcpp::IntegerVector iv = Rcpp::as< Rcpp::IntegerVector >( geometry_cols );
  //     return sfg_multilinestring( x, iv, xyzm );
  //   }
  //   case STRSXP: {
  //     Rcpp::StringVector sv = Rcpp::as< Rcpp::StringVector >( geometry_cols );
  //     return sfg_multilinestring( x, sv, xyzm );
  //   }
  //   default: {
  //     Rcpp::stop("sfheaders - unknown column types");   // #nocov
  //   }
  //   }
  //   return Rcpp::List::create(); // never reaches
  // }
  //
  //
  // inline SEXP sfg_multilinestring(
  //     SEXP& x,
  //     SEXP& cols,
  //     Rcpp::String& line_id,
  //     std::string xyzm
  // ) {
  //   if( Rf_isNull( cols ) ) {
  //     // #nocov start
  //     Rcpp::StringVector id_cols( 1 );
  //     id_cols[0] = line_id;
  //     SEXP other_cols = geometries::utils::other_columns( x, id_cols );
  //     return sfg_multilinestring( x, other_cols, line_id, xyzm );
  //     // #nocov end
  //   }
  //   switch( TYPEOF( cols ) ) {
  //   // case REALSXP: {}
  //   // case INTSXP: {
  //   //   Rcpp::IntegerVector iv = Rcpp::as< Rcpp::IntegerVector >( cols );
  //   //   return sfg_multilinestring( x, iv, line_id );
  //   // }
  //   case STRSXP: {
  //     Rcpp::StringVector sv = Rcpp::as< Rcpp::StringVector >( cols );
  //     return sfg_multilinestring( x, sv, line_id, xyzm );
  //   }
  //   default: {
  //     Rcpp::stop("sfheaders - unknown column types");   // #nocov
  //   }
  //   }
  //   return Rcpp::List::create(); // never reaches
  // }
  //
  //
  // inline SEXP sfg_multilinestring(
  //     SEXP& x,
  //     SEXP& cols,
  //     int& line_id,
  //     std::string xyzm
  // ) {
  //
  //   if( Rf_isNull( cols ) ) {
  //     // #nocov start
  //     Rcpp::IntegerVector id_cols( 1 );
  //     id_cols[0] = line_id;
  //     SEXP other_cols = geometries::utils::other_columns( x, id_cols );
  //     return sfg_multilinestring( x, other_cols, line_id, xyzm );
  //     // #nocov end
  //   }
  //   switch( TYPEOF( cols ) ) {
  //   case REALSXP: {}
  //   case INTSXP: {
  //     Rcpp::IntegerVector iv = Rcpp::as< Rcpp::IntegerVector >( cols );
  //     return sfg_multilinestring( x, iv, line_id, xyzm );
  //   }
  //   // case STRSXP: {
  //   //   Rcpp::StringVector sv = Rcpp::as< Rcpp::StringVector >( cols );
  //   //   return sfg_multilinestring( x, sv, line_id );
  //   // }
  //   default: {
  //     Rcpp::stop("sfheaders - unknown column types");  // #nocov
  //   }
  //   }
  //   return Rcpp::List::create(); // never reaches
  // }
  //
  // inline SEXP sfg_multilinestring(
  //     SEXP& x,
  //     SEXP& cols,
  //     SEXP& line_id,
  //     std::string xyzm
  // ) {
  //
  //   if( Rf_isNull( cols ) && Rf_isNull( line_id ) ) {
  //     return sfg_multilinestring( x, xyzm );
  //   }
  //
  //   if( !Rf_isNull( line_id ) && Rf_isNull( cols ) ) {
  //     SEXP other_cols = geometries::utils::other_columns( x, line_id );
  //     return sfg_multilinestring( x, other_cols, line_id, xyzm );
  //   }
  //
  //   if( Rf_isNull( line_id ) && !Rf_isNull( cols ) ) {
  //     return sfg_multilinestring( x, cols, xyzm );
  //   }
  //   switch( TYPEOF( line_id ) ) {
  //   case REALSXP: {}
  //   case INTSXP: {
  //     Rcpp::IntegerVector iv = Rcpp::as< Rcpp::IntegerVector >( line_id );
  //     int i = iv[0];
  //     return sfg_multilinestring( x, cols, i, xyzm );
  //   }
  //   case STRSXP: {
  //     Rcpp::StringVector sv = Rcpp::as< Rcpp::StringVector >( line_id );
  //     Rcpp::String s = sv[0];
  //     return sfg_multilinestring( x, cols, s, xyzm );
  //   }
  //   default: {
  //     Rcpp::stop("sfheaders - unknown column types");  // #nocov
  //   }
  //   }
  //   return Rcpp::List::create(); // never reaches
  // }


} // sfg
} // sfheaders

#endif
