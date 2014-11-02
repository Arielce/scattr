#include "TCPClient.hh"

TCPClient::TCPClient(const std::string & server, int port)
  : socket_(io_service_), connected_(false), buflen_(TCPBUFLEN)
{
  std::ostringstream os;
  os << port;

  tcp::resolver resolver(io_service_);
  tcp::resolver::query query(server, os.str());

  tcp::resolver::iterator end_point_iter = resolver.resolve(query);
  tcp::endpoint end_point = *end_point_iter;

  socket_.async_connect(end_point,
    boost::bind(&TCPClient::onConnect, this, boost::asio::placeholders::error, ++end_point_iter));
}

void
TCPClient::close()
{
  io_service_.post(boost::bind(&TCPClient::doClose, this));
}

void
TCPClient::onConnect(const boost::system::error_code& error_code, tcp::resolver::iterator end_point_iter)
{
  if (error_code == 0)
  {
    connected_ = true;
    if (this->callback_)
      this->callback_(TCPClient::CONNECTED, std::string());
    socket_.async_receive(boost::asio::buffer(buffer_.data(), buflen_),
      boost::bind(&TCPClient::onReceive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
  }
  else if (end_point_iter != tcp::resolver::iterator())
  {
    connected_ = false;
    socket_.close();
    tcp::endpoint end_point = *end_point_iter;

    socket_.async_connect(end_point,
      boost::bind(&TCPClient::onConnect, this, boost::asio::placeholders::error, ++end_point_iter));
  }
}

void
TCPClient::onReceive(const boost::system::error_code& error_code, std::size_t bytes_transferred)
{
  if (error_code == 0)
  {
    if (this->callback_)
      this->callback_(TCPClient::RECEIVED, std::string(buffer_.data(), bytes_transferred));
    socket_.async_receive(boost::asio::buffer(buffer_.data(), buflen_),
      boost::bind(&TCPClient::onReceive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
  }
  else
    this->doClose();
}

void
TCPClient::onWrite(const boost::system::error_code& error_code, std::size_t bytes_transferred)
{
  if (error_code == 0)
  {
    (void)bytes_transferred;
  }
}

void
TCPClient::onAction(TCPClient::callback_t callback)
{
  this->callback_ = callback;
}

void
TCPClient::write(const std::string & data)
{
  boost::system::error_code ignored_error;
  boost::asio::async_write(socket_, boost::asio::buffer(data, data.size()), boost::bind(&TCPClient::onWrite, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void
TCPClient::doClose()
{
  socket_.close();
}

std::shared_ptr<boost::thread>
TCPClient::run()
{
  if (thread_)
    return thread_;

  thread_ = std::shared_ptr<boost::thread>(new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_)));
  return thread_;
}

bool
TCPClient::good() const
{
  return connected_;
}
