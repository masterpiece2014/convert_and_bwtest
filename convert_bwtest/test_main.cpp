

#include "bwtest\bwtest.h"
#include "bwtest\bwtest.cpp"

#include "sample1_unittest.h"
#include "sample2_unittest.h"


int main() {
    //RUN_TEST(default, "unknown");
    //RUN_GROUP(custom);

    RUN_GROUP(sample1);
    RUN_TEST(custom, MyCustomTestClass);
    RUN_TEST(sample1, exceptions);
    //RUN_GROUP(sample2);
    
    return 0;
}
