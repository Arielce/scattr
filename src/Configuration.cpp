#include <fstream>
#include "Configuration.hh"
#include "AdaptersFactory.hh"

Configuration::Configuration(int argc, char** argv, char** env, const AdaptersFactory & adapters)
  : po::variables_map(), argc_(argc), argv_(argv), env_(env), desc_("Common")
{
  try
  {
    namespace po = boost::program_options;

    desc_.add_options()
    ("help", "Print usage")
    ("source,s", po::value<std::string>()->default_value(AMQP_DEFAULT_HOST), "Set the AMQP host source")
    ("port", po::value<int>()->default_value(AMQP_DEFAULT_PORT), "Set the AMQP port")
    ("user,u", po::value<std::string>(), "Set the AMQP user credential")
    ("password,p", po::value<std::string>(), "Set the AMQP password credential")
    ("log,o", po::value<std::string>()->default_value(DEFAULT_LOG_PATH), "Set the log file")
    ("config,c", po::value<std::string>()->default_value(DEFAULT_CONFIG_PATH), "Set the config file path");

    this->getFromAdapters(desc_, adapters);
    this->getDesc(desc_);

    po::notify(*this);
  }
  catch(Configuration::Error & e)
  {
    throw e;
  }
  catch(std::exception & e)
  {
    throw Configuration::Error(Configuration::Error::UNKNOWN, e.what());
  }
}

void
Configuration::getDesc(po::options_description & desc)
{
  try
  {
    po::store(po::parse_command_line(argc_, argv_, desc), *this);
  }
  catch(po::error& e)
  {
    throw Configuration::Error(Configuration::Error::PARSING_CMD, e.what());
  }
  try
  {
    std::string filename(DEFAULT_CONFIG_PATH);

    if (this->count("config"))
      filename = this->operator[]("config").as<std::string>();

    std::ifstream file(filename);
    if (!file.is_open() && filename != DEFAULT_CONFIG_PATH)
      throw Configuration::Error(Configuration::Error::OPENING_FILE, filename);
    po::store(po::parse_config_file(file, desc), *this);
  }
  catch (po::error& e)
  {
    throw Configuration::Error(Configuration::Error::PARSING_FILE, e.what());
  }
}

void
Configuration::getFromAdapters(po::options_description & base, const AdaptersFactory & adapters)
{
  for (auto & adapter : adapters.getAdapters())
  {
    po::options_description desc(adapter.second->getName());
    adapter.second->addConfiguration(desc);
    base.add(desc);
  }
}

std::ostream &
operator<<(std::ostream & os, const Configuration & conf)
{
  os << conf.desc_;
  return os;
}

/*
 * Configuration::Error
 */
Configuration::Error::Error(Configuration::Error::error_type type, const std::string & error)
  : std::runtime_error(error), type_(type), error_(error)
{
}

const char*
Configuration::Error::what() const throw ()
{
  std::string result;

  result = "";
  switch (type_)
  {
    case PARSING_CMD:
      result = "Error while parsing command line arguments";
    break;
    case PARSING_FILE:
      result = "Error while parsing configuration file";
    break;
    case OPENING_FILE:
      result = "Error when opening configuration file";
    break;
    default:
      result = "Unknown error while getting configuration";
    break;
  }
  result += ": " + error_;
  return result.c_str();
}
