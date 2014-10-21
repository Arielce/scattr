#ifndef   __BASE_ADAPTER_HH__
# define  __BASE_ADAPTER_HH__

# include <string>
# include <boost/program_options.hpp>

namespace po = boost::program_options;

namespace Adapters
{
  class BaseAdapter
  {
  public:
    virtual std::string getName() const = 0;
    virtual void addConfiguration(po::options_description &);
  };
};

#endif
