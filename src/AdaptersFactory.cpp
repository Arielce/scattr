#include <boost/assign.hpp>
#include "AdaptersFactory.hh"
#include "Configuration.hh"
/*
 * Include Adapters
 */
#include "adapters/android/AndroidAdapter.hh"

/*
 * AdaptersFactory
 */

AdaptersFactory::AdaptersFactory()
{
  boost::assign::insert(adapters_)
  ("Android", std::shared_ptr<Adapters::AndroidAdapter>(new Adapters::AndroidAdapter));
  // Add your adapters here
}

const AdaptersFactory::AdaptersContainer &
AdaptersFactory::getAdapters() const
{
  return adapters_;
}

std::shared_ptr<Adapters::BaseAdapter>
AdaptersFactory::operator[](const std::string & key)
{
  return adapters_[key];
}
