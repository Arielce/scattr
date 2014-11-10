#define  BOOST_LOG_DYN_LINK

#include <boost/assign.hpp>
#include <boost/log/core.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include "AdaptersFactory.hh"
#include "Configuration.hh"

namespace bl = boost::log;

/*
 * Include Adapters
 */
#include "adapters/android/AndroidAdapter.hh"
#include "adapters/ios/IOSAdapter.hh"

/*
 * AdaptersFactory
 */

std::unique_ptr<AdaptersFactory>
AdaptersFactory::instance_ = nullptr;

AdaptersFactory::AdaptersFactory()
{
  add<Adapters::AndroidAdapter>();
  add<Adapters::IOSAdapter>();
  // Add your adapters here
}

const AdaptersFactory::AdaptersContainer &
AdaptersFactory::getAdapters() const
{
  return adapters_;
}

std::shared_ptr<Adapters::BaseAdapter> &
AdaptersFactory::operator[](const std::string & key)
{
  return adapters_.at(key);
}

void
AdaptersFactory::initLogging(const Configuration & conf)
{
  std::ostringstream os(conf["log"].as<std::string>());
  os << "/notifier.%N.log";

  bl::add_file_log
  (
    bl::keywords::file_name = os.str(),
    bl::keywords::rotation_size = 10 * 1024 * 1024,
    bl::keywords::time_based_rotation = bl::sinks::file::rotation_at_time_point(0, 0, 0),
    bl::keywords::format = "[%TimeStamp%]: %Message%"
  );
}

void
AdaptersFactory::initAdapters(const Configuration & conf)
{
  this->initLogging(conf);
  auto it = adapters_.begin();
  while (it != adapters_.end())
  {
    if (it->second->init(conf) == false)
    {
      std::cerr << "Unable to init adapter " << it->first << std::endl;
      it = adapters_.erase(it);
    }
    else
      ++it;
  }
}

/**
 * ITERATORS
 */

AdaptersFactory::iterator
AdaptersFactory::begin()
{
  return adapters_.begin();
}

AdaptersFactory::const_iterator
AdaptersFactory::begin() const
{
  return adapters_.begin();
}

AdaptersFactory::iterator
AdaptersFactory::end()
{
  return adapters_.end();
}

AdaptersFactory::const_iterator
AdaptersFactory::end() const
{
  return adapters_.end();
}

template<typename A>
void
AdaptersFactory::add()
{
  std::shared_ptr<Adapters::BaseAdapter> adapter = std::make_shared<A>();
  adapters_[adapter->getName()] = adapter;
}

std::unique_ptr<AdaptersFactory>&
AdaptersFactory::getInstance()
{
  if (!instance_)
    AdaptersFactory::instance_ = std::unique_ptr<AdaptersFactory>(new AdaptersFactory());
  return AdaptersFactory::instance_;
}
