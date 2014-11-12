#include "AMQPHandler.hh"

namespace ph = std::placeholders;

const std::string
AMQPHandler::VHOST = "/";
const std::string
AMQPHandler::EXCHANGE_NAME = "nb_scattr_exch";
const std::string
AMQPHandler::QUEUE_NAME = "nb_scattr_queue";
const int
AMQPHandler::QUALITY_OF_SERVICE = 10;

AMQPHandler::AMQPHandler(const Configuration & conf)
  : connection_(nullptr), channel_(nullptr)
{
  std::string source = conf["source"].as<std::string>();
  int port = conf["port"].as<int>();
  username_ = conf["user"].as<std::string>();
  password_ = conf["password"].as<std::string>();
  socket_ = std::make_shared<TCPClient>(source, port);
  socket_->onAction(std::bind(&AMQPHandler::action, this, ph::_1, ph::_2));
  socket_->run();
  nblog << "Trying to connect to AMQP " << source << ":" << port << "...";
  socket_->wait();
  nblog << "Connection successful";
}

AMQPHandler::~AMQPHandler()
{
}

void
AMQPHandler::action(TCPClient::action action, const std::string & buf)
{
  (void)buf;
  if (action == TCPClient::CONNECTED)
  {
    if (connection_) // close former connection
      connection_->close();
    connection_ = std::make_shared<AMQP::Connection>(this, AMQP::Login(username_, password_), VHOST);
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
  nblog << "Connected to AMQP (valid credentials)";

  channel_ = std::unique_ptr<AMQP::Channel>(new AMQP::Channel(connection));
  channel_->setQos(QUALITY_OF_SERVICE);

  channel_->declareExchange(EXCHANGE_NAME, AMQP::ExchangeType::direct).onSuccess([]()
  {
    nblog << "Exchange " << EXCHANGE_NAME << " declared successfully";
  }).onError([](const char *error) {
    throw AMQPHandler::Error(AMQPHandler::Error::EXCHANGE_DECL, error);
  });
  for (auto & adapter : *AdaptersFactory::getInstance())
  {
    std::ostringstream os;

    os << QUEUE_NAME << "_" << adapter.first;
    std::string queue_name = os.str();
    channel_->declareQueue(queue_name, AMQP::durable).onSuccess([&, queue_name]()
    {
      nblog << "Queue " << queue_name << " declared successfully";
    }).onError([](const char *error) {
      throw AMQPHandler::Error(AMQPHandler::Error::QUEUE_DECL, error);
    });
    channel_->bindQueue(EXCHANGE_NAME, queue_name, adapter.first).onSuccess([&, queue_name]() {
      nblog << "Queue " << queue_name << " binded successfully with routing key " << adapter.second->getName();
    }).onError([](const char *error) {
      throw AMQPHandler::Error(AMQPHandler::Error::QUEUE_BIND, error);
    });
    channel_->consume(queue_name)
    .onReceived(std::bind(&AMQPHandler::onReceived, this, ph::_1, ph::_2, ph::_3))
    .onSuccess([queue_name]() {
      nblog << "Start consuming from " << queue_name;
    })
    .onError([](const char *error) {
      throw AMQPHandler::Error(AMQPHandler::Error::CONSUMING_START, error);
    });
  }

}

void
AMQPHandler::onError(AMQP::Connection *connection, const char *message)
{
  (void)connection;
  (void)message;
}

void
AMQPHandler::onClosed(AMQP::Connection *connection) {
  (void)connection;
  if (channel_)
    channel_ = nullptr;
  if (connection_)
    connection_ = nullptr;
}

void
AMQPHandler::onReceived(const AMQP::Message &message, uint64_t deliveryTag, bool redelivered)
{
  (void)redelivered;
  try
  {
    auto adapter = AdaptersFactory::getInstance()->operator[](message.routingKey());
    adapter->handleMessage(message.message(), deliveryTag, std::bind(&AMQPHandler::onHandled, this, std::placeholders::_1));
  }
  catch (const std::out_of_range & oor)
  {
    nblog << "Use of inexistant adapter: " << message.routingKey();
  }
}

void
AMQPHandler::onHandled(uint64_t deliveryTag)
{
  channel_->ack(deliveryTag);
}

/*
 * Configuration::Error
 */
AMQPHandler::Error::Error(AMQPHandler::Error::error_type type, const std::string & error)
  : std::runtime_error(error), type_(type), error_(error)
{
}

const char*
AMQPHandler::Error::what() const throw ()
{
  std::string result;

  result = "";
  switch (type_)
  {
    case EXCHANGE_DECL:
      result = "Exchange could not be declared";
    break;
    case QUEUE_DECL:
      result = "Queue could not be declared";
    break;
    case QUEUE_BIND:
      result = "Queue could not be binded";
    break;
    case CONSUMING_START:
      result = "Impossible to start consuming messages";
    break;
    default:
      result = "Unknown error while trying to setup AMQP connection";
    break;
  }
  result += ": " + error_;
  return result.c_str();
}
