#ifndef   __IOS_ADAPTER_HH__
# define  __IOS_ADAPTER_HH__

# include <string>
# include "BaseAdapter.hh"
# include "macgpusher/MMGAPNSConnection.hpp"

namespace Adapters
{
  class IOSAdapter : public BaseAdapter
  {
  protected:
    std::shared_ptr<MMGAPNSConnection> connection_;
  public:
    std::string getName() const;
    void addConfiguration(po::options_description &);
    bool init(const Configuration &);
    void message(const std::string &);
  };
};

#endif
