/* 
 * File:   example_adddition.cpp
 * Author: stefan
 *
 * Created on November 20, 2013, 12:08 PM
 */

#include <cstdlib>
#include "../src/lib/numbers/FLOAT_T.h"
#include <chrono>
#include <mpfr.h>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    typedef std::chrono::high_resolution_clock clock;
    typedef std::chrono::microseconds timeunit;
    
    double f1 = 3.141592654;
    double f2 = 1.414;
    
    hypro::FLOAT_T<mpfr_t> hf1 = hypro::FLOAT_T<mpfr_t>(f1);
    hypro::FLOAT_T<mpfr_t> hf2 = hypro::FLOAT_T<mpfr_t>(f2);
    hypro::FLOAT_T<mpfr_t> result;
    
    mpfr_t mf1;
    mpfr_t mf2;
    mpfr_t mResult;
    
    mpfr_init(mf1);
    mpfr_init(mf2);
    mpfr_init(mResult);
    
    mpfr_set_d(mf1,f1,MPFR_RNDN);
    mpfr_set_d(mf2,f2,MPFR_RNDN);
    
    clock::time_point start = clock::now();
    int count = 0;
    while (count < 100000000)
    {
        result.add(hf1,hf2,hypro::HYPRO_RNDN);
        ++count;
    }
    std::cout << "Total time(HYPRO): " << std::chrono::duration_cast<timeunit>( clock::now() - start ).count()/1000 << std::endl;
    
    start = clock::now();
    count = 0;
    while (count < 100000000)
    {
        mpfr_add(mResult, mf1, mf2, MPFR_RNDN);
        ++count;
    }
    std::cout << "Total time(MPFR): " << std::chrono::duration_cast<timeunit>( clock::now() - start ).count()/1000 << std::endl;
    
    return 0;
}

