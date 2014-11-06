#include "IOSAdapter.hh"

std::string
Adapters::IOSAdapter::getName() const
{
  return "ios";
}

void
Adapters::IOSAdapter::addConfiguration(po::options_description & desc)
{
  desc.add_options()
  ("ios-cert", po::value<std::string>()->default_value(CONFIG_PATH("ios-cert.pem")))
  ("ios-key", po::value<std::string>()->default_value(CONFIG_PATH("ios-key.pem")));
}

void
Adapters::IOSAdapter::message(const std::string & message)
{
  (void)message;
  boost::this_thread::sleep(boost::posix_time::milliseconds(2000));
  std::cout << "IOS handled message" << std::endl;
}
