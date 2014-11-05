#ifndef   __ANDROID_ADAPTER_HH__
# define  __ANDROID_ADAPTER_HH__

# include <string>
# include "BaseAdapter.hh"

namespace Adapters
{
  class AndroidAdapter : public BaseAdapter
  {
    std::string getName() const;
    void addConfiguration(po::options_description &);
    void message(const std::string &);
  };
};

#endif
