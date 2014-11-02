#include "AMQPHandler.hh"

AMQPHandler::AMQPHandler(const Configuration & conf)
  : connection_(nullptr)
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
  if (action == TCPClient::RECEIVED)
  {
    if (connection_)
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
  (void)connection;
  std::cout << "Connected" << std::endl;
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
