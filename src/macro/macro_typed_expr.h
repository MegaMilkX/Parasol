#ifndef _MACRO_TYPED_EXPR_H_
#define _MACRO_TYPED_EXPR_H_

#define EXPAND(x) x

#define REM(...) __VA_ARGS__
#define EAT(...)

// Retrieve the type
#define TYPEOF(x) DETAIL_TYPEOF(DETAIL_TYPEOF_PROBE x,)
#define DETAIL_TYPEOF(...) EXPAND(DETAIL_TYPEOF_HEAD(__VA_ARGS__))
#define DETAIL_TYPEOF_HEAD(x, ...) REM x
#define DETAIL_TYPEOF_PROBE(...) (__VA_ARGS__),
// Strip off the type
#define STRIP(x) EAT x
// Show the type without parenthesis
#define PAIR(x) REM x

#endif