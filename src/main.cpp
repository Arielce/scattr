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
  std::cout << "Initializing ssl..." << std::endl;
  SSL_library_init();
  try
  {
    Configuration conf(ac, av, env);

    if (conf.count("help"))
    {
      std::cout << conf << std::endl;
      return EXIT_SUCCESS;
    }
    AdaptersFactory::getInstance()->initAdapters(conf);

    AMQPHandler handler(conf);
    char loops = 1;
    while (42)
    {
      boost::this_thread::sleep(boost::posix_time::milliseconds(TIME_REFRESH_IN_MS));
      // refresh adapters
      for (auto & adapter : *AdaptersFactory::getInstance())
      {
        if (loops == NBR_LOOPS_DISPLAY)
          std::cout << "Adapter " << adapter.first << " handled " << adapter.second->getNbr() << " messages" << std::endl;
        adapter.second->refresh();
      }
      if (loops == NBR_LOOPS_DISPLAY)
        loops = 0;
      loops += 1;
    }
  }
  catch (Configuration::Error & e)
  {
    std::cout << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
