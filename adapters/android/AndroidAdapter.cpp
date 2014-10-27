#include "AndroidAdapter.hh"

std::string
Adapters::AndroidAdapter::getName() const
{
  return "Android";
}

void
Adapters::AndroidAdapter::addConfiguration(po::options_description & desc)
{
desc.add_options()
  ("android-cert", po::value<std::string>()->default_value(CONFIG_PATH("android-cert.pem")))
  ("android-key", po::value<std::string>()->default_value(CONFIG_PATH("android-key.pem")));
}
