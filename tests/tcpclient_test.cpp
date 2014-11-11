#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>
#include "TCPClient.hh"

class TCPClientTest : public CppUnit::TestFixture
{
public:
  std::shared_ptr<TCPClient> client;
  bool connected;
  bool received;

  void setUp()
  {
    client = std::shared_ptr<TCPClient>(new TCPClient("google.com", 80));
    TCPClient::callback_t callback = std::bind(&TCPClientTest::action, this, std::placeholders::_1, std::placeholders::_2);
    client->onAction(callback);
    connected = false;
    received = false;
    client->run();
  }

  void action(TCPClient::action action, const std::string & buf)
  {
    (void)buf;
    if (action == TCPClient::CONNECTED)
      connected = true;
    if (action == TCPClient::RECEIVED)
      received = true;
  }

  void tearDown()
  {
  }

  void testConnected()
  {
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
    CPPUNIT_ASSERT(connected == true);
  }

  void testReceived()
  {
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
    client->write("GET /\n");
    boost::this_thread::sleep(boost::posix_time::milliseconds(2000));
    CPPUNIT_ASSERT(received == true);
  }

  static CppUnit::Test *suite()
  {
    CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TCPClient Test");
    suiteOfTests->addTest( new CppUnit::TestCaller<TCPClientTest>(
                          "Test if connected to google",
                          &TCPClientTest::testConnected ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TCPClientTest>(
                          "Test if received data from google",
                          &TCPClientTest::testReceived ) );
    return suiteOfTests;
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TCPClientTest);
