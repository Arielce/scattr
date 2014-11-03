#include <iostream>
#include <amqpcpp.h>
#include "Configuration.hh"
#include "AdaptersFactory.hh"
#include "AMQPHandler.hh"

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
    AMQPHandler handler(conf);
    while (42)
    {
      boost::this_thread::sleep(boost::posix_time::milliseconds(500));
    }
  }
  catch (Configuration::Error & e)
  {
    std::cout << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
