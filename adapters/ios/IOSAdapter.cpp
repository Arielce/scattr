#include "IOSAdapter.hh"

std::string
Adapters::IOSAdapter::getName() const
{
  return "ios";
}

void
Adapters::IOSAdapter::addConfiguration(po::options_description & desc)
{
  desc.add_options()
  ("ios-ca", po::value<std::string>()->default_value(CONFIG_PATH("")))
  ("ios-cert", po::value<std::string>()->default_value(CONFIG_PATH("ios-cert.pem")))
  ("ios-key", po::value<std::string>()->default_value(CONFIG_PATH("ios-key.pem")))
  ("ios-passphrase", po::value<std::string>()->default_value(""));
}

bool
Adapters::IOSAdapter::init(const Configuration & configuration)
{
  std::string ca = configuration["ios-ca"].as<std::string>();
  std::string cert = configuration["ios-cert"].as<std::string>();
  std::string key = configuration["ios-key"].as<std::string>();
  std::string passphrase = configuration["ios-passphrase"].as<std::string>();

  connection_ = std::make_shared<MMGAPNSConnection>(ca, cert, key, passphrase, true);
  if (connection_->OpenConnection() != MMGConnectionError::MMGNoError)
  {
    nblog << "Could not open connection to APN server";
    return false;
  }
  return true;
}

/**
 * Message format should be:
 * char: number of recipients (thus max 255)
 * recipients comma separated uint badge, device token
 * message title followed by a comma (thus do not put ',' in your message title)
 * sound followed by a comma
 * message content
 */
void
Adapters::IOSAdapter::message(const std::string & message)
{
  char buf[BUFFER_SIZE];
  char nbr = message[0];
  std::string title;
  std::string content;
  std::string sound;
  size_t notif = 1;
  if (nbr <= 0)
  {
    std::cerr << "iOS handler: Bad number of recipients" << int(nbr) << std::endl;
    return;
  }
  std::vector<MMGDevice*> devices;
  std::istringstream is;

  is.str(std::string(message.c_str() + 1, message.size() - 1));
  while (nbr-- > 0 && is.good() && !is.eof())
  {
    is.getline(buf, BUFFER_SIZE, ',');
    uint badge = *((uint*)buf);
    devices.push_back(new MMGDevice(std::string(buf + sizeof(uint)), badge));
  }
  is.getline(buf, BUFFER_SIZE, ',');
  title = buf;
  is.getline(buf, BUFFER_SIZE, ',');
  sound = buf;
  if (is.eof())
  {
    std::cerr << "iOS handler: Bad message format" << std::endl;
    return;
  }
  while (!is.eof())
  {
    is.getline(buf, BUFFER_SIZE);
    content += buf;
  }
  MMGIOSPayload payload(title, content, 1, sound);
  for (auto & device : devices)
  {
    payload.SetBadgeNumber(device->GetBadge());
    connection_->SendPayloadToDevice(payload, *device, notif++);
    delete device;
  }
}
