#include <iostream>
#include "Configuration.hh"

int main(int ac, char** av, char** env)
{
  try
  {
    Configuration conf(ac, av, env);
  }
  catch (Configuration::Error & e)
  {
    std::cout << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
