#ifndef R_SFHEADERS_SHAPES_LIST_LIST_MAT_H
#define R_SFHEADERS_SHAPES_LIST_LIST_MAT_H

#include <Rcpp.h>
#include "sfheaders/utils/utils.hpp"
#include "sfheaders/utils/subset.hpp"
#include "sfheaders/shapes/mat/mat.hpp"
#include "sfheaders/shapes/list_mat/list_mat.hpp"

/*
 * get_listListMat
 *
 * - sfg_MULTIPOLYGON
 */

namespace sfheaders {
namespace shapes {

  inline SEXP get_listListMat(
    Rcpp::IntegerMatrix& im
  ) {
    Rcpp::List mp(1);
    mp[0] = sfheaders::shapes::get_listMat(im);
    return mp;
  }

  inline SEXP get_listListMat(
      Rcpp::NumericMatrix& nm
  ) {
    Rcpp::List mp(1);
    mp[0] = sfheaders::shapes::get_listMat(nm);
    return mp;
  }

  inline SEXP get_listListMat(
      Rcpp::DataFrame& df
  ) {
    Rcpp::List mp(1);
    mp[0] = sfheaders::shapes::get_listMat(df);
    return mp;
  }


  /*
   * get_polygon
   *
   * Gets all the polygons inside a single multipolygon
   *
   */
  inline SEXP get_listListMat(
    Rcpp::DataFrame& df,
    Rcpp::StringVector& geometry_cols,
    Rcpp::String& group_id_col_1,
    Rcpp::String& group_id_col_2
  ) {

    Rcpp::NumericVector polygon_ids = df[ group_id_col_1 ];
    Rcpp::StringVector df_names = df.names();

    Rcpp::NumericVector unique_polygon_ids = Rcpp::sort_unique( polygon_ids );
    Rcpp::IntegerMatrix polygons = sfheaders::utils::line_ids( polygon_ids, unique_polygon_ids );

    size_t n_polygons = unique_polygon_ids.length();

    Rcpp::List mpl( n_polygons );

    size_t i;
    for( i = 0; i < n_polygons; i++ ) {
      int start = polygons(i, 0);
      int end = polygons(i, 1);

      Rcpp::Range rng( start, end );
      Rcpp::DataFrame df_subset = sfheaders::utils::subset_dataframe( df, df_names, start, end );

      mpl[ i ] = sfheaders::shapes::get_listMat( df_subset, geometry_cols, group_id_col_2 );
    }
    return mpl;
  }


  inline SEXP get_listListMat(
      Rcpp::DataFrame& df,
      Rcpp::IntegerVector& geometry_cols,
      int& group_id_col_1,
      int& group_id_col_2
  ) {
    // given polygon & line ids, collapse and matricise and listify
    Rcpp::StringVector df_names = df.names();

    // use the indexes to subset the df_names to get the columns
    Rcpp::String str_group_id_col_1 = df_names[ group_id_col_1 ];
    Rcpp::String str_group_id_col_2 = df_names[ group_id_col_2 ];
    Rcpp::StringVector str_geometry_cols = df_names[ geometry_cols ];

    return get_listListMat( df, str_geometry_cols, str_group_id_col_1, str_group_id_col_2 );
  }

  inline SEXP get_listListMat(
      Rcpp::IntegerMatrix& im,
      Rcpp::StringVector& geometry_cols,
      Rcpp::String& group_id_col_1,
      Rcpp::String& group_id_col_2
  ) {
    Rcpp::DataFrame df = Rcpp::as< Rcpp::DataFrame >( im );
    return get_listListMat( df, geometry_cols, group_id_col_1, group_id_col_2 );
  }


  inline SEXP get_listListMat(
      Rcpp::IntegerMatrix& im,
      Rcpp::IntegerVector& geometry_cols,
      int& group_id_col_1,
      int& group_id_col_2
  ) {
    Rcpp::DataFrame df = Rcpp::as< Rcpp::DataFrame >( im );
    return get_listListMat( df, geometry_cols, group_id_col_1, group_id_col_2 );
  }


  inline SEXP get_listListMat(
      Rcpp::NumericMatrix& nm,
      Rcpp::StringVector& geometry_cols,
      Rcpp::String& group_id_col_1,
      Rcpp::String& group_id_col_2
  ) {
    Rcpp::DataFrame df = Rcpp::as< Rcpp::DataFrame >( nm );
    return get_listListMat( df, geometry_cols, group_id_col_1, group_id_col_2 );
  }

  inline SEXP get_listListMat(
      Rcpp::NumericMatrix& nm,
      Rcpp::IntegerVector& geometry_cols,
      int& group_id_col_1,
      int& group_id_col_2
  ) {
    Rcpp::DataFrame df = Rcpp::as< Rcpp::DataFrame >( nm );
    return get_listListMat( df, geometry_cols, group_id_col_1, group_id_col_2 );
  }


  inline SEXP get_listListMat(
      SEXP& x,
      Rcpp::StringVector& geometry_cols,
      Rcpp::String& group_id_col_1,
      Rcpp::String& group_id_col_2
  ) {
    switch( TYPEOF( x ) ) {
    case INTSXP: {
      if( !Rf_isMatrix( x ) ) {
      Rcpp::stop("sfheaders - polygons needs to be matrices or dataframes");
    } else {
      Rcpp::IntegerMatrix im = Rcpp::as< Rcpp::IntegerMatrix >( x );
      return get_listListMat( im, geometry_cols, group_id_col_1, group_id_col_2 );
    }
    }
    case REALSXP: {
      if( !Rf_isMatrix( x ) ) {
      Rcpp::stop("sfheaders - polygons needs to be matrices or dataframes");
    } else {
      Rcpp::NumericMatrix nm = Rcpp::as< Rcpp::NumericMatrix >( x );
      return get_listListMat( nm, geometry_cols, group_id_col_1, group_id_col_2 );
    }
    }
    case VECSXP: {
      if( Rf_inherits( x, "data.frame") ) {
      Rcpp::DataFrame df = Rcpp::as< Rcpp::DataFrame >( x );
      return get_listListMat( df, geometry_cols, group_id_col_1, group_id_col_2 );
    }
    }
    default: {
      Rcpp::stop("sfheaders - unknown polygons type");
    }
    }
  }


  inline SEXP get_listListMat(
    SEXP& x,
    Rcpp::IntegerVector& geometry_cols,
    int& group_id_col_1,
    int& group_id_col_2
  ) {
    switch( TYPEOF( x ) ) {
    case INTSXP: {
    if( !Rf_isMatrix( x ) ) {
        Rcpp::stop("sfheaders - polygons needs to be matrices or dataframes");
    } else {
      Rcpp::IntegerMatrix im = Rcpp::as< Rcpp::IntegerMatrix >( x );
      return get_listListMat( im, geometry_cols, group_id_col_1, group_id_col_2 );
    }
    }
    case REALSXP: {
    if( !Rf_isMatrix( x ) ) {
      Rcpp::stop("sfheaders - polygons needs to be matrices or dataframes");
    } else {
      Rcpp::NumericMatrix nm = Rcpp::as< Rcpp::NumericMatrix >( x );
      return get_listListMat( nm, geometry_cols, group_id_col_1, group_id_col_2 );
    }
    }
    case VECSXP: {
    if( Rf_inherits( x, "data.frame") ) {
      Rcpp::DataFrame df = Rcpp::as< Rcpp::DataFrame >( x );
      return get_listListMat( df, geometry_cols, group_id_col_1, group_id_col_2 );
    }
    }
    default: {
      Rcpp::stop("sfheaders - unknown polygons type");
    }
    }
  }

  inline SEXP get_listListMat(
    SEXP& x,
    SEXP& geometry_cols,  // IntegerVector or StringVector
    SEXP& group_id_col_1, // the outer-most ID column; e.g. polygon_id
    SEXP& group_id_col_2  // the inner-most ID column; e.g. line_id
  ) {

    if( ( TYPEOF( group_id_col_1 ) != TYPEOF( group_id_col_2 ) ) ||
        TYPEOF( geometry_cols ) != TYPEOF( group_id_col_1 ) ) {
      Rcpp::stop("sfheaders - different column types detected");
    }

    switch( TYPEOF( geometry_cols ) ) {
    case REALSXP: {}
    case INTSXP: {
      Rcpp::IntegerVector iv_geometry_cols = Rcpp::as< Rcpp::IntegerVector >( geometry_cols );
      Rcpp::IntegerVector iv_group_id_col_1 = Rcpp::as< Rcpp::IntegerVector >( group_id_col_1 );
      Rcpp::IntegerVector iv_group_id_col_2 = Rcpp::as< Rcpp::IntegerVector >( group_id_col_2 );
      int i_group_id_col_1 = iv_group_id_col_1[0];
      int i_group_id_col_2 = iv_group_id_col_2[0];
      return get_listListMat( x, iv_geometry_cols, i_group_id_col_1, i_group_id_col_2 );
    }
    case STRSXP: {
      Rcpp::StringVector sv_geometry_cols = Rcpp::as< Rcpp::StringVector >( geometry_cols );
      Rcpp::StringVector sv_group_id_col_1 = Rcpp::as< Rcpp::StringVector >( group_id_col_1 );
      Rcpp::StringVector sv_group_id_col_2 = Rcpp::as< Rcpp::StringVector >( group_id_col_2 );
      Rcpp::String s_group_id_col_1 = sv_group_id_col_1[0];
      Rcpp::String s_group_id_col_2 = sv_group_id_col_2[0];
      return get_listListMat( x, sv_geometry_cols, s_group_id_col_1, s_group_id_col_2 );
    }
    default: {
      Rcpp::stop("sfheaders - unknown id column types");
    }
    }

  }

} // shapes
} // sfheaders


#endif