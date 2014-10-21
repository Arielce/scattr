#ifndef   __BASE_ADAPTER_HH__
# define  __BASE_ADAPTER_HH__

namespace Adapters
{
  class BaseAdapter
  {
  public:
    virtual std::string getName() const = 0;
  };
};

#endif
