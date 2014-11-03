#include "AMQPHandler.hh"

const std::string
AMQPHandler::EXCHANGE_NAME = "nb_notifier_exch";
const std::string
AMQPHandler::QUEUE_NAME = "nb_notifier_queue";

AMQPHandler::AMQPHandler(const Configuration & conf)
  : connection_(nullptr), channel_(nullptr)
{
  std::string source = conf["source"].as<std::string>();
  int port = conf["port"].as<int>();
  socket_ = std::shared_ptr<TCPClient>(new TCPClient(source, port));
  socket_->onAction(std::bind(&AMQPHandler::action, this, std::placeholders::_1, std::placeholders::_2));
  socket_->run();
  std::cout << "AMQP " << source << ":" << port << "." << std::endl;
  while (!socket_->good())
  {
    std::cout << "Trying to connect to AMQP." << std::endl;
    boost::this_thread::sleep(boost::posix_time::milliseconds(500));
  }
}

void
AMQPHandler::action(TCPClient::action action, const std::string & buf)
{
  (void)buf;
  if (action == TCPClient::CONNECTED)
    return;
  if (action == TCPClient::RECEIVED && connection_)
  {
    connection_->parse(buf.data(), buf.size());
  }
}

void
AMQPHandler::onData(AMQP::Connection *connection, const char *data, size_t size)
{
  connection_ = connection;
  socket_->write(std::string(data, size));
}

void
AMQPHandler::onConnected(AMQP::Connection *connection)
{
  std::cout << "Connected to AMQP (valid credentials)" << std::endl;
  channel_ = std::unique_ptr<AMQP::Channel>(new AMQP::Channel(connection));
  channel_->declareExchange(EXCHANGE_NAME).onSuccess([&]() {
    std::cout << "Exchange " << EXCHANGE_NAME << " declared successfully" << std::endl;
    channel_->declareQueue(QUEUE_NAME).onSuccess([&]() {
      std::cout << "Queue " << QUEUE_NAME << " declared successfully" << std::endl;
      for (auto & adapter : *AdaptersFactory::getInstance())
      {
        channel_->bindQueue(EXCHANGE_NAME, QUEUE_NAME, adapter.second->getName()).onSuccess([&]() {
          std::cout << "Queue " << QUEUE_NAME << " binded successfully with routing key " << adapter.second->getName() << std::endl;
        }).onError([](const char *error) {
          std::ostringstream os;
          os << "Queue could not be binded: " << error;
          throw std::runtime_error(os.str());
        });
      }
    }).onError([](const char *error) {
      std::ostringstream os;
      os << "Queue could not be declared: " << error;
      throw std::runtime_error(os.str());
    });
  }).onError([](const char *error) {
    std::ostringstream os;
    os << "Exchange could not be declared: " << error;
    throw std::runtime_error(os.str());
  });
}

void
AMQPHandler::onError(AMQP::Connection *connection, const char *message)
{
  (void)connection;
  (void)message;
}

void
AMQPHandler::onClosed(AMQP::Connection *connection) {
  socket_->close();
  (void)connection;
}
