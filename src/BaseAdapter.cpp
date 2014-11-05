#include "BaseAdapter.hh"

void
Adapters::BaseAdapter::addConfiguration(po::options_description & desc)
{
  (void)desc;
}

void
Adapters::BaseAdapter::handleMessage(const std::string & message)
{
  (void)message;
}
