#ifndef   __CONFIGURATION_HH__
# define  __CONFIGURATION_HH__

# include <boost/program_options.hpp>

# define  AMQP_DEFAULT_PORT 5672
# define  AMQP_DEFAULT_HOST "localhost"
# define  DEFAULT_LOG_PATH  "/var/log/notifier/access.log"
# define  DEFAULT_CONFIG_PATH "/etc/needbook/notifier/notifier.conf"

namespace po = boost::program_options;

class AdaptersFactory;

/*!
 * Configure the program, parse the command line options...
 */
class Configuration : public po::variables_map
{
protected:
  int     argc_; //!< Number of command line arguments
  char**  argv_; //!< Command line arguments
  char**  env_; //!< Environment variables
  po::options_description desc_; //!< Accepted arguments
public:
  /*!
   * Exception class specific to configuration errors
   */
  class Error : public std::runtime_error {
  public:
    enum error_type {
      UNKNOWN,
      PARSING_CMD,
      PARSING_FILE,
      OPENING_FILE
    };
  protected:
    error_type  type_;
    std::string error_;
  public:
    Error(error_type, const std::string &);
    virtual ~Error() throw () {};
    virtual const char* what() const throw ();
  };

protected:
  /*!
   * Parse command line arguments using a specific description
   */
  void getDesc(po::options_description &);
  /*!
   * Get the description from the adapters
   */
  void getFromAdapters(po::options_description &, const AdaptersFactory &);
public:
  /*!
   * Initialize configuration
   */
  Configuration(int, char**, char**, const AdaptersFactory &);

  /*!
   * Sends the usage to a stream
   */
  friend std::ostream & operator<<(std::ostream &, const Configuration &);
};

#endif
