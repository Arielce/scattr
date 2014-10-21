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
  ("android", std::shared_ptr<Adapters::AndroidAdapter>(new Adapters::AndroidAdapter));
  // Add your adapters here
}

void
AdaptersFactory::run(const Configuration & conf)
{
  (void)conf;
}

const AdaptersFactory::AdaptersContainer &
AdaptersFactory::getAdapters() const
{
  return adapters_;
}
