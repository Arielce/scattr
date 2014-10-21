#ifndef   __ADAPTERS_FACTORY_HH__
# define  __ADAPTERS_FACTORY_HH__

# include <map>
# include <memory>
# include "Configuration.hh"
# include "BaseAdapter.hh"

class AdaptersFactory
{
private:
  std::reference_wrapper<const Configuration> configuration_;
  std::map<std::string, std::shared_ptr<Adapters::BaseAdapter>> adapters_;
public:
  AdaptersFactory(const Configuration &);
  void run();
};

#endif
