# sfheaders


## Another spatial library?

Yep. In a few of my other libraries I've made use of `sf` objects, but without importing the `sf` library itself. This is by design because `sf` is quite a 'heavy' library. 

Therefore I've written / copied these methods for constructing the `sf` objects across a few different libraries. 

So I thought it would be useful to have these in one place. And here they are (or will be, when it's finished).



## Can I use these methods too?

Yep

TODO: how to use.


### sfg


```r

library(Rcpp)
library(sf) ## for print

cppFunction(
  includes = '
    #include "sfheaders/sfg/sfg.hpp"
  ',
  code = '
    SEXP a_polygon( Rcpp::DataFrame df ) {
      return sfheaders::sfg::to_polygon( df );
    }
  ',
  depends = "sfheaders"
)

df <- data.frame(
  x = 1:5
  , y = 5:1
)

a_polygon( df )
POLYGON ((1 5, 2 4, 3 3, 4 2, 5 1))

```






