#ifndef   __ADAPTERS_FACTORY_HH__
# define  __ADAPTERS_FACTORY_HH__

# include <map>
# include <memory>
# include "BaseAdapter.hh"

class Configuration;

/*!
 * Handle all adapters
 */
class AdaptersFactory
{
public:
  typedef std::map<std::string, std::shared_ptr<Adapters::BaseAdapter>> AdaptersContainer;
private:
  /*!
   * A map containing the adapters
   */
  AdaptersContainer adapters_;
public:
  /*!
   * Creates a new AdaptersFactory and initializes adapters
   */
  AdaptersFactory();
  /*!
   * Get adapters_
   */
  const AdaptersContainer & getAdapters() const;
};

#endif
