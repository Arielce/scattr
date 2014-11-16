#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>
#include "Configuration.hh"
#include "AMQPHandler.hh"

#include <openssl/crypto.h>
#include <openssl/ssl.h>

#ifdef __APPLE__
  #pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

class IOSTest : public CppUnit::TestFixture
{
public:
  char *av[4];
  Configuration * conf;
  AMQPHandler * h;

  void setUp()
  {
    char *args[4] = { strdup("tests"), strdup("--help"), strdup("-o"), strdup("file") };
    memcpy(av, args, sizeof(av));
    conf = new Configuration(4, av, nullptr);
    SSL_load_error_strings();
    SSL_library_init();
    AdaptersFactory::getInstance()->initAdapters(*conf);
    h = new AMQPHandler(*conf);
  }

  void tearDown()
  {
    delete conf;
    delete h;
  }

  void testInit()
  {
    bool init = false;
    try {
      AdaptersFactory::getInstance()->operator[]("ios");
      init = true;
    }
    catch(std::exception &){}
    CPPUNIT_ASSERT(init == true);
  }

  static CppUnit::Test *suite()
  {
    CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("IOS Test");
    suiteOfTests->addTest( new CppUnit::TestCaller<IOSTest>(
                          "Test if init passed (must have a configuration file with good parameters)",
                          &IOSTest::testInit));
    return suiteOfTests;
  }
};

//CPPUNIT_TEST_SUITE_REGISTRATION(IOSTest);
