




#include "convert_test.h"
#include "hallocator_test.h"

using std::cout;
using std::cin;
using std::endl;


int main()
{
	//RUN_GROUP("default");
	cout <<(1<<8) <<endl;
    RUN_GROUP("convert");
	//put_out << "\n>>>total size" << BWTest_Names::TestRegister::instance()->tests_.size();// << "  " <<TestRegister::instance()->tests_.begin()->first << "\n";

    return 0;
}
