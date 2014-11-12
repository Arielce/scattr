#include <boost/assign.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/empty_deleter.hpp>
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
  std::cout << "Initializing logger..." << std::endl;

  bl::core::get()->add_global_attribute("TimeStamp", boost::log::attributes::local_clock());
  bl::formatter formatter = bl::expressions::stream << bl::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S") << "> " << bl::expressions::smessage;
  bl::add_file_log
  (
    bl::keywords::file_name = conf["log"].as<std::string>(),
    bl::keywords::rotation_size = 10 * 1024 * 1024,
    bl::keywords::time_based_rotation = bl::sinks::file::rotation_at_time_point(0, 0, 0),
    bl::keywords::auto_flush = true
  )->set_formatter(formatter);
  typedef bl::sinks::synchronous_sink<bl::sinks::text_ostream_backend> text_sink;
  boost::shared_ptr<text_sink> sink = boost::make_shared<text_sink>();
  boost::shared_ptr<std::ostream> stream(&std::clog, bl::empty_deleter());
  sink->locked_backend()->add_stream(stream);
  bl::core::get()->add_sink(sink);
}

void
AdaptersFactory::logging(bool st)
{
  bl::core::get()->set_logging_enabled(st);
}

bl::sources::logger_mt &
AdaptersFactory::logger()
{
  static bl::sources::logger_mt lg;

  return lg;
}

void
AdaptersFactory::initAdapters(const Configuration & conf)
{
  auto it = adapters_.begin();
  bool result;
  bool disabled;
  while (it != adapters_.end())
  {
    disabled = false;
    result = true;
    if (conf.count("disable"))
    {
      auto & ad_dis = conf["disable"].as<std::vector<std::string>>();
      if (std::find(ad_dis.begin(), ad_dis.end(), it->first) != ad_dis.end())
        disabled = true;
    }
    if (disabled == false)
      result = it->second->init(conf);
    nblog << "Initializing adapter " << it->first << "... " << (result ? (!disabled ? "Done!" : "Disabled.") : "Error!");
    if (result == false)
      it = adapters_.erase(it);
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
