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
  ("ios-ca", po::value<std::string>()->default_value(CONFIG_PATH("")))
  ("ios-cert", po::value<std::string>()->default_value(CONFIG_PATH("ios-cert.pem")))
  ("ios-key", po::value<std::string>()->default_value(CONFIG_PATH("ios-key.pem")))
  ("ios-passphrase", po::value<std::string>()->default_value(""));
}

bool
Adapters::IOSAdapter::init(const Configuration & configuration)
{
  std::string ca = configuration["ios-ca"].as<std::string>();
  std::string cert = configuration["ios-cert"].as<std::string>();
  std::string key = configuration["ios-key"].as<std::string>();
  std::string passphrase = configuration["ios-passphrase"].as<std::string>();

  connection_ = std::make_shared<MMGAPNSConnection>(ca, cert, key, passphrase, true);
  if (connection_->OpenConnection() != MMGConnectionError::MMGNoError)
		return false;
  return true;
}

void
Adapters::IOSAdapter::message(const std::string & message)
{
  (void)message;
  boost::this_thread::sleep(boost::posix_time::milliseconds(2000));
  std::cout << "IOS handled message" << std::endl;
}
