#include "catch.hpp"
#include "Factorial.hpp"

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( Factorial(1) == 1 );
    REQUIRE( Factorial(2) == 2 );
    REQUIRE( Factorial(3) == 6 );
    REQUIRE( Factorial(0) == 1 );
    REQUIRE( Factorial(10) == 3628800 );
}

TEST_CASE( "64-bit Test - Sign Extension", "[64-bit]" ) {
    int i = -2;
    unsigned k = 1U;
    long n = foo(i+k);
    REQUIRE( n == -1 );
}
