#include <boost/assign.hpp>
#include "AdaptersFactory.hh"

/*
 * Include Adapters
 */
#include "adapters/android/AndroidAdapter.hh"

/*
 * AdaptersFactory
 */

AdaptersFactory::AdaptersFactory(const Configuration & conf)
  : configuration_(conf)
{
  boost::assign::insert(adapters_)
  ("android", std::shared_ptr<Adapters::AndroidAdapter>(new Adapters::AndroidAdapter));
  // Add your adapters here
}

void
AdaptersFactory::run()
{

}
