


#include "bwtest\bwtest.h"
#include "sample1.h"


TEST(sample1, odd_even, 1) {
    expect_true(true == isOdd(5)) << "if 5 is not odd, \
                                      hence this expectation failed, \
                                      this line will be put out.";
    expect_false(true == isOdd(7)) << "e.g. this line will be put out\
                                        'cause 7 is odd";

    expect_eq(true, isOdd(11)) << "this line won't be put out";
    expect_nq(true, isOdd(10)) << "either";
}


// step 1 : inherite
class MyCustomTestClass : public bwtest::TestBase
{
    // step 2 : define constructor
    CONSTRUCT(MyCustomTestClass) {
        // you own initialzation actions here
    }

    // step 3 : overwrite run()
    void run() {
        put_out << "\n\t====test invoked====\n";
        long num = rand() / 10;
        expect_eq(myCorrectFactorialFunction(num), factorial(num)) << " wrong factorial"
                                                    << "correct: " << myCorrectFactorialFunction(num)
                                                    << "incrrect:" << factorial(num);

        expect_true(isPrime(97)) <<  97 << "is not prime";
        
        expect_true(isPrime(2));
        expect_true(isPrime(3));
        expect_true(isPrime(5));

        expect_false(isPrime(1));
        expect_false(isPrime(91));
        expect_false(isPrime(-1));
        expect_false(isPrime(-17));
    }

    int myCorrectFactorialFunction(long n) {
        int result = 1;
        for (long i = 1; i <= n; i++) {
            result *= i;
        }
        return result;
    }
};
REGISTER(MyCustomTestClass);


TEST(sample1, exceptions, 2) {
    put_out << "\n\n===this test will be excuted twice";

    expect_throw(throwRangeError(), std::range_error);
    expect_throw(throwRangeError(), std::bad_alloc) << "caught no bad_alloc";

    expect_throw_any(throwRandomStdExcept());
}

