#ifndef   __ADAPTERS_FACTORY_HH__
# define  __ADAPTERS_FACTORY_HH__

# include <map>
# include <memory>
# include "BaseAdapter.hh"

class Configuration;

class AdaptersFactory
{
public:
  typedef std::map<std::string, std::shared_ptr<Adapters::BaseAdapter>> AdaptersContainer;
private:
  AdaptersContainer adapters_;
public:
  AdaptersFactory();
  void run(const Configuration &);
  const AdaptersFactory::AdaptersContainer & getAdapters() const;
};

#endif
