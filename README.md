convert_and_bwtest
==================

Fast and saft C++ type conversion library.
[Project hoome](http://www.caibowen.com/work.html#id_convert_h)

Unit Test Code is [here](https://github.com/xkommando/convert_and_bwtest/blob/master/convert_bwtest/convert_test.h)

Quick Start:

```C++
std::string str = to<std::string>(
"In Math", "it is", true, "that",  
'P', 'I', "is about", to<float>("3.1415926"));
```

```C++
// string -> integer
uint64_t uint64 = to<uint64_t>("18446744073709551615");
uint32 = 0;
// safe conversion
expect_throw(uint32 = to<uint32_t>(18446744073709551615),std::overflow_error);

// append integer to string
std::string str;
int sint32 = std::numeric_limits<int>::max();
appendTo(str, sint32);
```
