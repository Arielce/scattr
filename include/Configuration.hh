#ifndef   __CONFIGURATION_HH__
# define  __CONFIGURATION_HH__

# include <boost/program_options.hpp>

# define  AMQP_DEFAULT_PORT 5672
# define  AMQP_DEFAULT_HOST "localhost"
# define  DEFAULT_LOG_PATH  "/var/log/notifier/access.log"
# define  DEFAULT_CONFIG_PATH "/etc/needbook/notifier/notifier.conf"

namespace po = boost::program_options;

class AdaptersFactory;

class Configuration : public po::variables_map
{
protected:
  int     argc_;
  char**  argv_;
  char**  env_;
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

  Configuration(int, char**, char**, const AdaptersFactory &);
  void getDesc(po::options_description &);
  void getFromAdapters(const AdaptersFactory &);
};

#endif
