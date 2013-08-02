


#include "bwtest.h"


// this function help the compiler deduce type, and do the range check.
    template<class T, class E>
    void BWTEST_FUNC_RANGE_CHEACK(T val, E, const char* funcname, const char* line) {
        std::string error("at Line: ");
        error.append(line);
        error.append("  Fuction: ");
        error.append(funcname);
        val <= static_cast<T>(std::numeric_limits<E>::max())
        ?   (void)0 : throw std::overflow_error(error.append("\n overflow in convertion\n"));
        val >= static_cast<T>(std::numeric_limits<E>::min())
            ? (void)0 : throw std::underflow_error(error.append( "\nunderflow in convertion\n"));
    }

namespace bwtest
{
//    BWTestOutputType outputStream = stdCerr;
//    char* _fileName  =__BW_NULL_PTR;

    void setOutput(const char * c_str) {
        if (!strncmp(c_str, "cout", 4))
            outputStream = stdCout;
        else if (!strncmp(c_str, "cerr", 4))
            outputStream = stdCerr;
        else if (!strncmp(c_str, "clog", 4))
            outputStream = stdClog;
        else {
            _fileName = const_cast<char*>(c_str);
            outputStream = file;
        }
    }

     std::ostream& getOutputStream() {
        switch(outputStream) {
        case stdCout : return std::cout;
        case stdCerr : return std::cerr;
        case stdClog : return std::clog;
        case file :
            static std::ofstream bwOStream(_fileName, std::ios::app);
            if (bwOStream.is_open())
                return bwOStream;
            else {
                std::cerr << "cannot open file" << _fileName << std::endl;
                exit(EXIT_FAILURE);
            }
        } // switch
    }


    void cleanOutputStream() {
        if (file == outputStream) {
            static_cast<std::ofstream&>(getOutputStream()
            ).close();
        }
    }

} // namespace bwtest


namespace BWTest_Names
{
//// multi-type, multi-arguments printing
////    e.g.
//// print(std::string("sdfds"), 32234, 546.546, "sdgdf", 'c');
////
#ifdef __BWTEST_HAS_ENOUGH_CXX11__
void print(){}
template<typename T, typename... Ts>
void print(const T& obj, const Ts&... others) {
    std::cout << obj << " ";
    print(others...);
}
#endif

//////////////////////////////////////////////////////////////////////////////////////////////

//
//class TestBase
//{
//    private:
//        const char* name_testgroup;
//        const char* name_testcase_;
//        size_t num_excutions_;
//        size_t num_std_except_;
//        size_t num_other_except_;
//        unsigned long int time_elapsed_;// duration, in second.
//
//    virtual void testBody() = 0;
//
//#ifdef __BWTEST_HAS_ENOUGH_CXX11__
//        TestBase() = delete;
//        TestBase(const TestBase&) = delete;
//        TestBase(TestBase&&) = delete;
//        TestBase& operator= (const TestBase& ) = delete;
//#else
//    private:
//        TestBase(); // no definition
//        TestBase(const TestBase&);
//        TestBase& operator= (const TestBase& );
//#endif
//
//  public:
//        explicit
//        TestBase(size_t tmies, const char* testcase, const char* testgroup = "default")
//        :   name_testgroup(testgroup),
//            name_testcase_(testcase),
//            num_excutions_(tmies),
//            num_std_except_(0),
//            num_other_except_(0),
//            time_elapsed_(0)  {}
//
//        virtual
//        ~TestBase() __BW_NOEXCEPT   {}
//
//
//#ifdef __BWTEST_HAS_ENOUGH_CXX11__
//		void excute() {
//			try {
//				auto t_start = std::chrono::high_resolution_clock::now();
//				for(size_t i = 0; i < num_excutions_; ++i) {
//					try {
//						testBody();
//					}
//					catch(std::exception& e) {
//						num_std_except_++;
//                        OUT_ONCE("\n>>>");
//						OUT_ONCE(name_testcase_);
//						OUT_ONCE("       caught std exception:\n>>> ");
//						OUT_ONCE(e.what());
//					}
//					catch(...) {
//						num_other_except_++;
//                        OUT_ONCE("\n>>>");
//						OUT_ONCE(name_testcase_);
//						OUT_ONCE("       caught other exception");
//					}
//				}
//				auto t_end = std::chrono::high_resolution_clock::now();
//				this->time_elapsed_ = std::chrono::duration_cast<std::chrono::seconds>(t_end - t_start).count();
//			}
//			catch(...) {
//			    OUT_ONCE("\n>>> ");
//			    OUT_ONCE(name_testcase_);
//                OUT_ONCE("  testBody: caught unknown exception. \n");
//			}
//		}
//#else
//		void excute() {
//			try {
//				std::clock_t c_start = std::clock();
//				for(size_t i = 0; i != _excutionOfTimes; ++i) {
//					try {
//						testBody();
//					} catch(std::exception& e) {
//						num_std_except_++;
//                        OUT_ONCE("\n>>>");
//						OUT_ONCE(name_testcase_);
//						OUT_ONCE("       caught std exception:\n>>> ");
//						OUT_ONCE(e.what());
//					} catch(...) {
//						num_other_except_++;
//                        OUT_ONCE("\n>>>");
//						OUT_ONCE(name_testcase_);
//						OUT_ONCE("       caught other exception");
//					}
//				}
//				std::clock_t c_end = std::clock();
//				this->time_elapsed_ = (c_end - c_start) / CLOCKS_PER_SEC;
//			}
//			catch(...) {
//			    OUT_ONCE("\n>>> ");
//			    OUT_ONCE(name_testcase_);
//                OUT_ONCE("  testBody: caught unknown exception. \n");
//			}
//		}
//#endif
//
//        void printReport() const __BW_NOEXCEPT {
//            ::bwtest::getOutputStream() << "\n>>>  test\"" << name_testcase_
//            << "\" excuted " << num_excutions_ << "times."
//            << "\n>>>  time passed: " << this->time_elapsed_
//            << "s\n>>>  caught std exception " << num_std_except_ << " times."
//            << "\n>>>  caught other exception " << num_other_except_ << " times." << std::endl;
//        }
//};
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
        TestRegister::~TestRegister() __BW_NOEXCEPT {
//            for(size_t i = 0; i != list_testcases_.size(); ++i) {
//                delete list_testcases_[i];
//            }

            for (GroupMap::iterator i = tests_.begin(); i != tests_.end(); ++i) {
                for (int j = 0; j != i->second.size(); ++j) {
                    delete i->second.at(j);
                }
            }
            ::bwtest::cleanOutputStream();
        }


        bool TestRegister::registerTestDefaultGroup(TestBase* newTest) {
//            TestRegister::instance()->class_handler->list_testcases_.push_back(newTest);

            TestRegister::instance()->tests_["default"].push_back(newTest);
//put_out << "\n>>>registed. size" << TestRegister::instance()->tests_.size() << "  " <<TestRegister::instance()->tests_.begin()->first << "\n";
            return true;
        }

        bool TestRegister::registerTest(TestBase* newTest, const char* group) {
//            TestRegister::instance()->class_handler->list_testcases_.push_back(newTest);
            TestRegister::instance()->tests_[group].push_back(newTest);
            return true;
        }

        TestRegister* TestRegister::instance(){
            if(TestRegister::class_handler == __BW_NULL_PTR) {
                class_handler = new TestRegister();
            }
            return class_handler;
        }

        unsigned long TestRegister::runGroup(const char* group) {
//put_out << "call to runGroup" << group << "\n\n";
#ifdef __BWTEST_HAS_ENOUGH_CXX11__
            auto t_start = std::chrono::high_resolution_clock::now();

            GroupMap::iterator  grp = TestRegister::instance()->tests_.find(group);
            if (grp != TestRegister::instance()->tests_.end()) {
                for (int j = 0; j != grp->second.size(); ++j) {
                    grp->second.at(j)->excute();
                }
            } else {
                put_out << "no such group: " << group << '\n';
            }

            auto t_end = std::chrono::high_resolution_clock::now();
            return std::chrono::duration_cast<std::chrono::seconds>(t_end - t_start).count();
#else
            std::clock_t c_start = std::clock();

            GroupMap::iterator  grp = TestRegister::instance()->tests_.find(group);
            if (grp != TestRegister::instance()->tests_.end()) {
                for (int j = 0; j != grp->size(); ++j) {
                    grp->at(j)->excute();
                }
            } else {
                put_out << "no such group: " << group << '\n';
            }

            std::clock_t c_end = std::clock();
            return (c_end - c_start) / CLOCKS_PER_SEC;
#endif
        }


        void TestRegister::runAllTests()  {
#ifdef __BWTEST_HAS_ENOUGH_CXX11__
            auto t_start = std::chrono::high_resolution_clock::now();

            TestRegister* obj = TestRegister::instance();
            for (GroupMap::iterator i = obj->tests_.begin(); i != obj->tests_.end(); ++i) {
                for (int j = 0; j != i->second.size(); ++j) {
                    i->second.at(j)->excute();
                }
            }

//                        GroupMap::iterator  grp = TestRegister::instance()->tests_.find(group);
//            if (grp != TestRegister::instance()->tests_.end()) {
//                for (int j = 0; j != grp->second.size(); ++j) {
//                    grp->second.at(j)->excute();
//                }
//            } else {
//                put_out << "no such group: " << group << '\n';
//            }

            auto t_end = std::chrono::high_resolution_clock::now();
            class_handler->time_total_
                = std::chrono::duration_cast<std::chrono::seconds>(t_end - t_start).count();
#else
            std::clock_t c_start = std::clock();

            TestRegister* obj = TestRegister::instance();
            for (GroupMap::iterator i = obj->tests_.begin(); i != obj->tests_.end(); ++i) {
                for (int j = 0; j != i->second.size(); ++j) {
                    i->second.at(j)->excute();
                }
            }

            std::clock_t c_end = std::clock();
            class_handler->time_total_
                = (c_end - c_start) / CLOCKS_PER_SEC;
#endif
        }

        void TestRegister::reportAllTests() __BW_NOEXCEPT {
//                        ::bwtest::getOutputStream() << "\n\n=============================================\n";
//            ::bwtest::getOutputStream() << ">>>\t TEST REPORT" << std::endl;
//            time_t t_now = std::time(__BW_NULL_PTR);
//            ::bwtest::getOutputStream() << ">>>  time: " << std::asctime(std::localtime(&t_now));
//            for(size_t i = 0; i != TestRegister::instance()->class_handler->list_testcases_.size(); ++i) {
//
//                ::bwtest::getOutputStream() << "--------------------------------------";
//                class_handler->list_testcases_[i]->printReport();
//            }
//
//            ::bwtest::getOutputStream() << "\n>>> Time elapsed: "
//                << class_handler->time_total_ << " seconds" << std::endl;
//
//            ::bwtest::getOutputStream() << "=============================================\n";
        }

TestRegister* TestRegister::class_handler = __BW_NULL_PTR;

/////////////////////////////////////////////////////////////////////////////////

    BWTEST_Expect_Print_Aux:: BWTEST_Expect_Print_Aux(
                bool statement,
                bool expectation,
                const char* expectText0,
                const char* expectText1,
                const char* actualText,
                const char* filePath,
                int lineNumber,
                const char* func_name)

            : expect_failed_(statement != expectation) {

        if(expect_failed_) {
           ::bwtest::getOutputStream()
                << "\n\n>>>  " << filePath
                << "\n>>>  Line: " << lineNumber << "\t function name: " << func_name
                << "\n>>>  Expect: "
                << expectText0 << expectText1
                << "\n>>>  Actual: " << actualText
                << std::endl;
        }
    }

    bool BWTEST_Expect_Print_Aux::has_failed() const __BW_NOEXCEPT {
        return expect_failed_;
    }


bool BWTEST_Expect_Print_Aux::BWTEST_bool_caughtExcepttion = false;



}// namespace BWTest_Names
