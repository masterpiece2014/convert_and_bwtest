




#include "convert.h"
#include "bwtest.h"

using std::cout;
using std::cin;
using std::endl;

TEST(test_str_to_int, 1) {
   unsigned short uint16 = to<unsigned short>("65530");
                assert(uint16 == 65530);

    signed short sint16 = to<signed short>("-32766");
                assert(sint16 == -32766);

    int sint32 = to<int>("-2147483647");
                    assert(sint32 == -2147483647);

    uint32_t uint32 = to<uint32_t>("4294967295");
                    assert(uint32 == 4294967295);

    signed long long int sint64 = to<signed long long int>("-9223372036854775807");
                    assert(sint64 == -9223372036854775807);

    uint64_t uint64 = to<uint64_t>("18446744073709551615");
                    assert(uint64 == 18446744073709551615);

    uint32 = 0;
            assert_throw(uint32 = to<uint32_t>(18446744073709551615),
                     std::overflow_error);
            assert_throw(uint32 = to<uint32_t>(-1),
                     std::underflow_error);

    uint32 = std::numeric_limits<int>::max() + 1;
            assert_throw(sint32 = to<int>(uint32), std::overflow_error);

    sint64 = std::numeric_limits<decltype(sint64)>::min() + 1;
            assert_throw(uint64 = to<uint64_t>(sint64),std::underflow_error);

    double dub = std::numeric_limits<double>::min();
    float foo = 0.0;
            assert_throw(foo = to<float>(dub), std::underflow_error);

    bool boli = to<bool>("    true");
                assert(boli);
            boli = to<bool>("           FALSE");
                assert(!boli);
            boli = to<bool>("           ON");
                assert(boli);
            boli = to<bool>("           f");
                assert(!boli);
            boli = to<bool>("           1");
                assert(boli);
            boli = to<bool>("      0");
                assert(!boli);

            boli = to<bool>("     yes");
                assert(boli);
            boli = to<bool>("     n");
                assert(!boli);

        std::string strtmp;
        appendTo(strtmp, boli);
            assert(strtmp == "false");

        double dob = to<double>("     e");
                    assert_eq(dob, 2.7182818);

            dob = to<double>(" PI  ");
                    assert_eq_at(dob, 3.1415926, 0.01);

            dob = to<double>("10e10");
                    assert_eq(dob, 1e11);
}

TEST(test_int_to_str, 2) {
    std::string str;

    cout << "-------------------------------------\n";
    cout << " signed 16\n";
    signed short sint16 = 1024;
    str.clear();
        appendTo(str, sint16);
            assert(str == "1024");


    sint16 = - sint16;
    str.clear();
        appendTo(str, sint16);
            assert(str == "-1024");

    cout << "-------------------------------------\n";
    cout << " signed 32\n";
    int sint32 = std::numeric_limits<int>::max() / 3;
    str.clear();
        appendTo(str, sint32);
        cout << sint32 << endl << str << endl;

    sint32 = - sint32;
    str.clear();
        appendTo(str, sint32);
            cout << sint32 << endl << str << endl;


    cout << "-------------------------------------\n";
    cout << " signed 64\n";
    signed long long int sint64 = std::numeric_limits<signed long long int>::max() / 2;
    str.clear();
        appendTo(str, sint64);
                   cout << sint64 << endl << str << endl;

    sint64 = - sint64;
    str.clear();
        appendTo(str, sint64);
                    cout << sint64 << endl << str << endl;

//------------------------------------------------------------------------------
//                      unsigned
//------------------------------------------------------------------------------
    cout << "-------------------------------------\n";
    cout << " unsigned 16\n";
    signed short usint16 = 1024;
    str.clear();
        appendTo(str, usint16);
            assert(str == "1024");
        usint16 = - usint16;
    str.clear();
        appendTo(str, usint16);
           cout << usint16 << endl << str << endl;
  //  cout << "-------------------------------------\n";

    cout << "-------------------------------------\n";
    cout << " unsigned 32\n";
    uint32_t usint32 = std::numeric_limits<uint32_t>::max() / 3;
    str.clear();
        appendTo(str, usint32);
        cout << usint32 << endl << str << endl;

    cout << "-------------------------------------\n";
    cout << " unsigned 64\n";
    uint64_t usint64 = std::numeric_limits<uint64_t>::max() / 3;
    str.clear();
        appendTo(str, usint64);
                   cout << usint64 << endl << str << endl;

  //  cout << "--------------float---------\n";
    double doo = to<double>("12.3456");
            assert_eq(doo, 12.3456);
    float foo = to<float>(123.456);
            assert_eq(foo, 123.456);

    str.clear();
    str.append("starter_starter_");
        appendTo(str, "appending");
            assert(str == "starter_starter_appending");

   // cout << "---------c string append char---------------\n";
    str.clear();
    appendTo(str, "i am having FUN");
    appendTo(str, ", yeah!");
                assert(str == "i am having FUN, yeah!");

    //    cout << "-------- string append char---------------\n";
        std::string str2 = "____~!@#$%^&*()_+|";
        appendTo(str, str2);
                assert(str == "i am having FUN, yeah!____~!@#$%^&*()_+|");



    uint16_t u16int = 1;
    uint64_t u64int64 = std::numeric_limits<uint16_t>::max();
    u64int64++;

    assert_throw(u16int = to<uint16_t>(u64int64),std::overflow_error);

        assert_throw(u16int = to<uint16_t>(-1), std::underflow_error);

}

TEST(test_integrated, 1) {
    double dob;
    std::string str = to<std::string>("integrated test ", 'r', 'o', 'u', 'n', 'd', ' ', 1, '\n');
            assert(str == "integrated test round 1\n");
////////////////////////////////////////////////
   // str.clear();
     //   appendTo(str, "   NaN");
     //           assert(dob = to<double>(str) == std::numeric_limits<double>::quiet_NaN());
    str.clear();
    dob = to<double>("3.141592653");
                assert_eq(dob, 3.141592653);
    str.clear();
    appendTo(str, "PI is about ",  dob);
        cout << str << endl;
//////////////////////////////////////////////////
    str.clear();
    appendTo(str, "true is ", to<bool>(" T   "), '\n');
    appendTo(str, "false is ", to<bool>(" 0"), '\n');
    appendTo(str, "E ", 'i', 's', " ablout ", to<double>("e"), '\n');
    appendTo(str, 'a', 'n', 'd', " PI is about ", to<double>("       PI"), '\n');
    appendTo(str, "2exp10 is ", to<uint16_t>("1024"), '\n');
   // appendTo(str, "max of unsigned 64-bit integer is ", std::numeric_limits<uint64_t>::max(), '\n');
    //appendTo(str, "max of signed 64-bit integer is ", std::numeric_limits<signed long long>::max(), '\n');
    appendTo(str, '\n',
             to<std::string>("max of unsigned 64-bit integer is ", std::numeric_limits<uint64_t>::max()),
             '\n');
        cout << str << endl;
//////////////////////////////////////////////////
    uint16_t uint16;
        assert_throw(uint16 = to<uint16_t>("-1"), std::overflow_error);

    str.clear();
    appendTo(str, std::numeric_limits<double>::max());
    float foo;
        assert_throw(foo = to<float>(str), std::overflow_error);

    str.clear();
    appendTo(str, std::numeric_limits<double>::min());
        assert_throw(foo = to<float>(str), std::underflow_error);
}


int main()
{
    RUN_ALL();
    REPORT_ALL();
    return 0;
}











