#include "AMQPHandler.hh"

namespace ph = std::placeholders;

const std::string
AMQPHandler::EXCHANGE_NAME = "nb_notifier_exch";
const std::string
AMQPHandler::QUEUE_NAME = "nb_notifier_queue";
const int
AMQPHandler::QUALITY_OF_SERVICE = 5;

AMQPHandler::AMQPHandler(const Configuration & conf)
  : connection_(nullptr), channel_(nullptr)
{
  std::string source = conf["source"].as<std::string>();
  int port = conf["port"].as<int>();
  socket_ = std::make_shared<TCPClient>(source, port);
  socket_->onAction(std::bind(&AMQPHandler::action, this, std::placeholders::_1, std::placeholders::_2));
  socket_->run();
  std::cout << "Trying to connect to AMQP " << source << ":" << port << "." << std::endl;
  socket_->wait();
  std::cout << "Connection successful" << std::endl;
}

void
AMQPHandler::action(TCPClient::action action, const std::string & buf)
{
  (void)buf;
  if (action == TCPClient::CONNECTED)
  {
    if (connection_) // close former connection
      connection_->close();
    connection_ = std::make_shared<AMQP::Connection>(this, AMQP::Login("guest","guest"), "/");
  }
  else if (action == TCPClient::RECEIVED && connection_)
  {
    connection_->parse(buf.data(), buf.size());
  }
}

void
AMQPHandler::onData(AMQP::Connection *connection, const char *data, size_t size)
{
  (void)connection;
  socket_->write(std::string(data, size));
}

void
AMQPHandler::onConnected(AMQP::Connection *connection)
{
  std::cout << "Connected to AMQP (valid credentials)" << std::endl;
  channel_ = std::unique_ptr<AMQP::Channel>(new AMQP::Channel(connection));
  channel_->setQos(QUALITY_OF_SERVICE);
  channel_->declareExchange(EXCHANGE_NAME).onSuccess([&]() {
    std::cout << "Exchange " << EXCHANGE_NAME << " declared successfully" << std::endl;
    channel_->declareQueue(QUEUE_NAME, AMQP::durable).onSuccess([&]() {
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
      channel_->consume(QUEUE_NAME)
      .onReceived(std::bind(&AMQPHandler::onReceived, this, ph::_1, ph::_2, ph::_3))
      .onError([](const char *error) {
        std::ostringstream os;
        os << "Impossible to start consuming messages on queue " << QUEUE_NAME << ": " << error;
        throw std::runtime_error(os.str());
      });
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

void
AMQPHandler::onReceived(const AMQP::Message &message, uint64_t deliveryTag, bool redelivered)
{
  (void)redelivered;
  std::cout << "received message " << message.message() << std::endl;
  channel_->ack(deliveryTag);
}
