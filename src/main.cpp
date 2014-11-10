#include <iostream>
#include <amqpcpp.h>
#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include "Configuration.hh"
#include "AdaptersFactory.hh"
#include "AMQPHandler.hh"

#ifdef __APPLE__
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

# define TIME_REFRESH_IN_MS 1000
# define NBR_LOOPS_DISPLAY  10

int main(int ac, char** av, char** env)
{
  try
  {
    Configuration conf(ac, av, env);

    if (conf.count("help"))
    {
      std::cout << conf << std::endl;
      return EXIT_SUCCESS;
    }
    AdaptersFactory::getInstance()->initLogging(conf);
    nblog << "Initializing ssl...";
    SSL_load_error_strings();
    SSL_library_init();
    AdaptersFactory::getInstance()->initAdapters(conf);
    nblog << "Initializing AMQP...";
    AMQPHandler handler(conf);
    char loops = 0;
    while (42)
    {
      loops += 1;
      boost::this_thread::sleep(boost::posix_time::milliseconds(TIME_REFRESH_IN_MS));
      // refresh adapters
      for (auto & adapter : *AdaptersFactory::getInstance())
      {
        if (loops == NBR_LOOPS_DISPLAY)
          nblog << "Adapter " << adapter.first << " handled " << adapter.second->getNbr() << " messages";
        adapter.second->refresh();
      }
      if (loops == NBR_LOOPS_DISPLAY)
        loops = 0;
    }
  }
  catch (std::exception & e)
  {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
