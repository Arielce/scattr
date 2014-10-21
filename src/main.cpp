#include <iostream>
#include "Configuration.hh"
#include "AdaptersFactory.hh"

int main(int ac, char** av, char** env)
{
  try
  {
    AdaptersFactory adapters;
    Configuration conf(ac, av, env, adapters);

    adapters.run(conf);
  }
  catch (Configuration::Error & e)
  {
    std::cout << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
