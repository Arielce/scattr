#ifndef   __CONFIGURATION_HH__
# define  __CONFIGURATION_HH__

# include <boost/program_options.hpp>

# define  AMQP_DEFAULT_PORT 5672
# define  AMQP_DEFAULT_HOST "localhost"
# define  DEFAULT_LOG_PATH  "/var/log/notifier/access.log"
# define  DEFAULT_CONFIG_PATH "/etc/needbook/notifier/notifier.conf"

namespace po = boost::program_options;

class Configuration : public po::variables_map
{
protected:
  po::variables_map   vm_;
public:
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

  Configuration(int, char**, char**);
};

#endif
