//----------------------------------------------------------------------------
// Copyright (c) 2015, Codalogic Ltd (http://www.codalogic.com)
// All rights reserved.
//
// The license for this file is based on the BSD-3-Clause license
// (http://www.opensource.org/licenses/BSD-3-Clause).
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// - Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// - Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// - Neither the name Codalogic Ltd nor the names of its contributors may be
//   used to endorse or promote products derived from this software without
//   specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//----------------------------------------------------------------------------

#if defined(_MSC_VER)
// Require error when nonstandard extension used :
//      'token' : conversion from 'type' to 'type'
//      e.g. disallow foo( /*const*/ foo & ) for copy constructor
#pragma warning(error: 4239)
#endif

#include <string>
#include <iostream>

#include "annotate-lite.h"

// Modified functions from http://www.hyc.io/boost/expected-proposal.pdf
//----------------------------------------------------------------------------

template< typename Texception >
double safe_divide( double i, double j )
{
    if( j == 0.0 ) throw Texception();
    return i / j;
}

template< typename Texception >
double f1( double i, double j, double k )
{
    return i + safe_divide<Texception>(j,k);
}
// Some classes by which to record errors
//----------------------------------------------------------------------------

struct or_j_is_zero {};
struct or_k_is_zero {};
struct or_k1_is_zero {};
struct or_k2_is_zero {};

// Examples and tests
//----------------------------------------------------------------------------

void test_safe_divide_ok()
{
    try
    {
        double i=1.0, j=1.0;
        double r = safe_divide<or_j_is_zero>( i, j );
        Good( "test_safe_divide_ok exception did not throw" );
    }
    catch( or_j_is_zero & )
    {
        Bad( "test_safe_divide_ok exception should not throw" );
    }
    catch( ... )
    {
        Bad( "Unexpected shit happened. Our program is broken!" );
    }
}

void test_safe_divide_bad()
{
    try
    {
        double i=1.0, j=0.0;
        double r = safe_divide<or_j_is_zero>( i, j );
        Bad( "test_safe_divide_bad exception did not throw" );
    }
    catch( or_j_is_zero & )
    {
        Good( "test_safe_divide_bad exception thrown" );
    }
    catch( ... )
    {
        Bad( "Unexpected shit happened. Our program is broken!" );
    }
}

void test_multiple_safe_divide_ok()
{
    try
    {
        double i=1.0, j=1.0, k=1.0;
        double r = safe_divide<or_j_is_zero>(
                    safe_divide<or_k_is_zero>( i, j ) + 1.0, k );
        Good( "test_multiple_safe_divide_ok exception did not throw" );
    }
    catch( or_j_is_zero & )
    {
        Bad( "test_multiple_safe_divide_ok or_j_is_zero exception should not throw" );
    }
    catch( or_k_is_zero & )
    {
        Bad( "test_multiple_safe_divide_ok or_k_is_zero exception should not throw" );
    }
    catch( ... )
    {
        Bad( "Unexpected shit happened. Our program is broken!" );
    }
}

void test_multiple_safe_divide_bad()
{
    try
    {
        double i=1.0, j=0.0, k=1.0;
        double r = safe_divide<or_k_is_zero>(
                    safe_divide<or_j_is_zero>( i, j ) + 1.0, k );
        Bad( "test_multiple_safe_divide_bad exception did not throw" );
    }
    catch( or_j_is_zero & )
    {
        Good( "test_multiple_safe_divide_bad or_j_is_zero exception thrown" );
    }
    catch( or_k_is_zero & )
    {
        Bad( "test_multiple_safe_divide_bad or_k_is_zero exception should not throw" );
    }
    catch( ... )
    {
        Bad( "Unexpected shit happened. Our program is broken!" );
    }

    try
    {
        double i=1.0, j=1.0, k=0.0;
        double r = safe_divide<or_k_is_zero>(
                    safe_divide<or_j_is_zero>( i, j ) + 1.0, k );
        Bad( "test_multiple_safe_divide_bad exception did not throw" );
    }
    catch( or_j_is_zero & )
    {
        Bad( "test_multiple_safe_divide_bad or_j_is_zero exception should not throw" );
    }
    catch( or_k_is_zero & )
    {
        Good( "test_multiple_safe_divide_bad or_k_is_zero exception thrown" );
    }
    catch( ... )
    {
        Bad( "Unexpected shit happened. Our program is broken!" );
    }
}

void test_f1_ok()
{
    try
    {
        double i=1.0, j=1.0, k=1.0;
        double r = f1<or_k_is_zero>( i, j, k );
        Good( "test_f1_ok exception did not throw" );
    }
    catch( or_j_is_zero & )
    {
        Bad( "test_f1_ok exception should not throw" );
    }
    catch( ... )
    {
        Bad( "Unexpected shit happened. Our program is broken!" );
    }
}

void test_f1_bad()
{
    try
    {
        double i=1.0, j=1.0, k=0.0;
        double r = f1<or_k_is_zero>( i, j, k );
        Bad( "test_f1_bad exception did not throw" );
    }
    catch( or_k_is_zero & )
    {
        Good( "test_f1_bad exception thrown" );
    }
    catch( ... )
    {
        Bad( "Unexpected shit happened. Our program is broken!" );
    }
}

void test_multiple_f1_ok()
{
    try
    {
        double i=1.0, j=1.0, k1=1.0, k2=1.0;
        double r = f1<or_k1_is_zero>( i, j, k1 ) +
                    f1<or_k2_is_zero>( i, j, k2 );
        Good( "test_multiple_f1_ok exception did not throw" );
    }
    catch( or_k1_is_zero & )
    {
        Bad( "test_multiple_f1_ok exception should not throw" );
    }
    catch( or_k2_is_zero & )
    {
        Bad( "test_multiple_f1_ok exception should not throw" );
    }
    catch( ... )
    {
        Bad( "Unexpected shit happened. Our program is broken!" );
    }
}

void test_multiple_f1_bad()
{
    try
    {
        double i=1.0, j=1.0, k1=0.0, k2=1.0;
        double r = f1<or_k1_is_zero>( i, j, k1 ) +
                    f1<or_k2_is_zero>( i, j, k2 );
        Bad( "test_multiple_f1_ok exception did not throw" );
    }
    catch( or_k1_is_zero & )
    {
        Good( "test_multiple_f1_ok or_k1_is_zero exception thrown" );
    }
    catch( or_k2_is_zero & )
    {
        Bad( "test_multiple_f1_ok or_k2_is_zero exception should not throw" );
    }
    catch( ... )
    {
        Bad( "Unexpected shit happened. Our program is broken!" );
    }

    try
    {
        double i=1.0, j=1.0, k1=1.0, k2=0.0;
        double r = f1<or_k1_is_zero>( i, j, k1 ) +
                    f1<or_k2_is_zero>( i, j, k2 );
        Bad( "test_multiple_f1_ok exception did not throw" );
    }
    catch( or_k1_is_zero & )
    {
        Bad( "test_multiple_f1_ok or_k1_is_zero exception should not throw" );
    }
    catch( or_k2_is_zero & )
    {
        Good( "test_multiple_f1_ok or_k2_is_zero exception thrown" );
    }
    catch( ... )
    {
        Bad( "Unexpected shit happened. Our program is broken!" );
    }
}

int main( int argc, char * argv[] )
{
    test_safe_divide_ok();
    test_safe_divide_bad();

    test_multiple_safe_divide_ok();
    test_multiple_safe_divide_bad();

    test_f1_ok();
    test_f1_bad();

    test_multiple_f1_ok();
    test_multiple_f1_bad();

    report();

    return 0;
}
