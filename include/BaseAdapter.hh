#ifndef   __BASE_ADAPTER_HH__
# define  __BASE_ADAPTER_HH__

# include <string>
# include <boost/program_options.hpp>

# define  CONFIG_PATH(file)   "/etc/needbook/notifier/adapters/" file

namespace po = boost::program_options;

namespace Adapters
{
  class BaseAdapter
  {
  public:
    /*!
     * Get the name of the adapter, will also be the name of the AMQP queue.
     */
    virtual std::string getName() const = 0;
    /*!
     * Add configuration parameters, such as command line options,
     * which the Adapter will be available to retrieve later on.
     * You should override this method in your Adapter.
     */
    virtual void addConfiguration(po::options_description &);
  };
};

#endif
