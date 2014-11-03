#include "IOSAdapter.hh"

std::string
Adapters::IOSAdapter::getName() const
{
  return "iOS";
}

void
Adapters::IOSAdapter::addConfiguration(po::options_description & desc)
{
  desc.add_options()
  ("ios-cert", po::value<std::string>()->default_value(CONFIG_PATH("ios-cert.pem")))
  ("ios-key", po::value<std::string>()->default_value(CONFIG_PATH("ios-key.pem")));
}
