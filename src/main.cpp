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
    while (42)
    {
      boost::this_thread::sleep(boost::posix_time::milliseconds(TIME_REFRESH_IN_MS));
      // refresh adapters
      for (auto & adapter : *AdaptersFactory::getInstance())
        adapter.second->refresh();
    }
  }
  catch (Configuration::Error & e)
  {
    std::cout << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
