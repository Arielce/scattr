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
  typedef AdaptersContainer::iterator iterator;
  typedef AdaptersContainer::const_iterator const_iterator;
private:
  /*!
   * A map containing the adapters
   */
  AdaptersContainer adapters_;
  static std::unique_ptr<AdaptersFactory> instance_;
  /*!
   * Creates a new AdaptersFactory and initializes adapters
   */
  AdaptersFactory();
public:
  /*!
   * Get adapters_
   */
  const AdaptersContainer & getAdapters() const;
  std::shared_ptr<Adapters::BaseAdapter> operator[](const std::string &);
  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;
  /*!
   * Get the unique instance of the Factory
   */
  static std::unique_ptr<AdaptersFactory>& getInstance();
};

#endif
