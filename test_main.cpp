#include <UnitTest++/UnitTest++.h>
#include "logger.h"
#include "base.h"
#include "calc.h"
#include "communicator.h"
#include "error.h"
#include "interface.h"
#include <filesystem>
#include <fstream>
// Тестирование класса Logger
SUITE(LoggerTests)
{
    TEST(Empty_SetPath) 
    {   
        Logger logger;
        CHECK_THROW(logger.setPath(""), CritError);
    }

    TEST(wrongExtension_SetPath) 
    {   
        Logger logger;
        CHECK_THROW(logger.setPath("log.png"), CritError);
    }

    TEST(pathNotExist_SetPath) 
    {   
        Logger logger;
        CHECK_THROW(logger.setPath("______.txt"), CritError);
    }

    TEST(Good_SetPath) 
    {   
        Logger logger;
        logger.setPath("/home/stud/kurs/log.txt");
        CHECK_EQUAL("/home/stud/kurs/log.txt", logger.getPath());
    }

    TEST(Empty_WriteLog) 
    {   
        Logger logger("");
        CHECK_THROW(logger.writeLog("1"), CritError);
    }

    TEST(CantOpen_WriteLog) 
    {   
        Logger logger("notExistPath.txt");
        CHECK_THROW(logger.writeLog("2"), CritError);
    }

    TEST(Good_WriteLog)
    {   
    Logger logger;
    logger.setPath("/home/stud/kurs/log.txt");
    logger.writeLog("Good_WriteLog");
    CHECK_EQUAL("/home/stud/kurs/log.txt", logger.getPath());
}

    TEST(Good_GetPath) {   
        Logger logger("log.txt");
        CHECK_EQUAL("log.txt", logger.getPath());
    }   
}
// Тестирование класса Base
SUITE(BaseTests)
{
	TEST(EmptyPath_Connect)
	{   
        Base base;
        CHECK_THROW(base.connect(""), CritError);
  }
	TEST(WrongExtension_Connect)
	{   
        Base base;
        CHECK_THROW(base.connect("baseTXT"), CritError);
  }
  TEST(NoExistPath_Connect)
	{   
        Base base;
        CHECK_THROW(base.connect("this_path_does_not_exist.txt"), CritError);
  }
  TEST(NoDoublePoint_Connect)
	{   
        Base base;
        CHECK_THROW(base.connect("base_bad.txt"), CritError);
  }
  TEST(NoLogin_Connect)
	{   
        Base base;
        CHECK_THROW(base.connect("base_bad.txt"), CritError);
  }
  TEST(NoPassword_Connect)
	{   
        Base base;
        CHECK_THROW(base.connect("base_bad.txt"), CritError);
  }
  TEST(EmptyBase_Connect)
	{   
        Base base;
        CHECK_THROW(base.connect("base_empty.txt"), CritError);
  }
  TEST(Good_GetData)
	{   
    Base base;
    base.connect("base.txt");
    std::map<std::string, std::string> expectedData = {{"user", "P@ssW0rd"}};
    std::map<std::string, std::string> actualData = base.getData();
    for (auto& pair : expectedData)
    {
        auto it = actualData.find(pair.first);
        CHECK_EQUAL(pair.second, it->second);
    }
	}
}
// Тестирование класса Calc
SUITE(CalcTests)
{
	TEST(Empty_Calc)
  {
      CHECK_THROW(Calc calc(std::vector<double>{}), NoCritError);
  }
  TEST(Good_Calc)
  {
      Calc calc({1.0, 2, 3.0 ,4});
      CHECK_EQUAL(calc.getResult(), 30.0);
  }
  TEST(Overflow_Calc)
  {
      Calc calc({1.7e+308});
      CHECK_EQUAL(calc.getResult(), std::numeric_limits<double>::max());
  }
}
// Тестирование класса Interface
SUITE(InterfaceTests)
{
    TEST(h_parser)
		{
    		Interface intrfc;
    		const char* argv[] = {"program", "-h"};
    		int argc = sizeof(argv) / sizeof(argv[0]);
    		CHECK_EQUAL(true, intrfc.parseArguments(argc, argv));
		}	
		TEST(help_parser)
		{
    		Interface intrfc;
    		const char* argv[] = {"program", "--help"};
    		int argc = sizeof(argv) / sizeof(argv[0]);
    		CHECK_EQUAL(true, intrfc.parseArguments(argc, argv));
		}
		TEST(Default_Port_parser)
    {
        Interface intrfc;
        CHECK_EQUAL(intrfc.getServerPort(), 33333);
    }
     
    TEST(Invalid_Port_Lower_Bound)
    {
        Interface intrfc;
        const char* argv[] = {"program", "--port", "10"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        CHECK_THROW(intrfc.parseArguments(argc, argv), CritError);
    }
    TEST(big_Port_parser)
    {
        Interface intrfc;
        const char* argv[] = {"program", "--port", "999999"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        CHECK_THROW(intrfc.parseArguments(argc, argv), CritError);
    }
    TEST(normal_p_parser)
    {
        Interface intrfc;
        const char* argv[] = {"program", "-p", "33330"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        intrfc.parseArguments(argc, argv);
        CHECK_EQUAL(intrfc.getServerPort(), 33330);
    }
    TEST(normal_Port_parser)
    {
        Interface intrfc;
        const char* argv[] = {"program", "--port", "33330"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        intrfc.parseArguments(argc, argv);
        CHECK_EQUAL(intrfc.getServerPort(), 33330);
    }
    TEST(Default_DBPath_parser)
    {
        Interface intrfc;
        CHECK_EQUAL(intrfc.getDatabaseFile(), "base.txt");
    }
    TEST(Default_LogPath_parser)
    {
        Interface intrfc;
        CHECK_EQUAL(intrfc.getLogFile(), "log.txt");
    }
		TEST(unknown_option_parser)
    {
        Interface intrfc;
        const char* argv[] = {"program", "--unknown_option"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        
        CHECK_THROW(intrfc.parseArguments(argc, argv), CritError);
    }
    TEST(l_getLogFile_parser)
    {
        Interface intrfc;
        const char* argv[] = {"program", "-l", "test.txt"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        intrfc.parseArguments(argc, argv);
        CHECK_EQUAL(intrfc.getLogFile(), "test.txt");
    }
    TEST(logFile_getLogFile_parser)
    {
        Interface intrfc;
        const char* argv[] = {"program", "--logFile", "test.txt"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        intrfc.parseArguments(argc, argv);
        CHECK_EQUAL(intrfc.getLogFile(), "test.txt");
    }
    TEST(b_getDatabaseFile_parser)
    {
        Interface intrfc;
        const char* argv[] = {"program", "-b", "test.txt"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        intrfc.parseArguments(argc, argv);
        CHECK_EQUAL(intrfc.getDatabaseFile(), "test.txt");
    }
    TEST(databaseFile_getDatabaseFile_parser)
    {
        Interface intrfc;
        const char* argv[] = {"program", "--databaseFile", "test.txt"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        intrfc.parseArguments(argc, argv);
        CHECK_EQUAL(intrfc.getDatabaseFile(), "test.txt");
    }
    TEST(TwoParam_parser)
    {
        Interface intrfc;
        const char* argv[] = {"program", "-b", "test.txt", "-l", "log.txt"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        intrfc.parseArguments(argc, argv);
        CHECK_EQUAL(intrfc.getDatabaseFile(), "test.txt");
        CHECK_EQUAL(intrfc.getLogFile(), "log.txt");
    }
    TEST(ThreeParam_parser)
    {
        Interface intrfc;
        const char* argv[] = {"program", "-b", "test.txt", "-l", "log.txt","-p", "33330"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        intrfc.parseArguments(argc, argv);
        CHECK_EQUAL(intrfc.getDatabaseFile(), "test.txt");
        CHECK_EQUAL(intrfc.getLogFile(), "log.txt");
        CHECK_EQUAL(intrfc.getServerPort(), 33330);
    }
    TEST(ZeroParam_parser)
		{
    		Interface intrfc;
        const char* argv[] = {"program"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        intrfc.parseArguments(argc, argv);
        CHECK_EQUAL(intrfc.getDatabaseFile(), "base.txt");
        CHECK_EQUAL(intrfc.getLogFile(), "log.txt");
        CHECK_EQUAL(intrfc.getServerPort(), 33333);;
		}
}
// Тестирование класса Communicator
SUITE(CommunicatorTests)
{
		
    TEST(TimeSalt_generateSalt)
    {
        Communicator communicator;
        std::string salt1 = communicator.generateSalt();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::string salt2 = communicator.generateSalt();
        CHECK(salt1 != salt2);
    }
    TEST(SixteenLen_generateSalt)
    {
        Communicator communicator;
        std::string salt = communicator.generateSalt();
        CHECK(salt.length() == 16);
    }
    TEST(Alphabet_generateSalt)
    {
        Communicator communicator;
        std::string salt = communicator.generateSalt();
        for (char c : salt)
        	{CHECK((std::isdigit(c) || (c >= 'a' && c <= 'f')));}
    }
    TEST(MD5)
		{
    		Communicator communicator;
    		std::string password = "P@ssW0rd";
    		std::string salt = "fb25e3f495048eed";
    		std::string expected_hash = "4FA6326AB57EF791605029017DA9CC3B";
    		std::string hash = communicator.MD5(salt + password);
    		CHECK_EQUAL(hash, expected_hash);
		}
}
int main(int argc, char* argv[])
	{return UnitTest::RunAllTests();}
