#include "TCPClient.hh"

TCPClient::TCPClient(const std::string & server, int port)
  : socket_(io_service_), buflen_(TCPBUFLEN)
{
  std::ostringstream os(port);

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
      socket_.async_receive(boost::asio::buffer(buffer_.data(), buflen_),
        boost::bind(&TCPClient::onReceive, this, boost::asio::placeholders::error));
    }
    else if (end_point_iter != tcp::resolver::iterator())
    {
      socket_.close();
      tcp::endpoint end_point = *end_point_iter;

      socket_.async_connect(end_point,
        boost::bind(&TCPClient::onConnect, this, boost::asio::placeholders::error, ++end_point_iter));
    }
}

void
TCPClient::onReceive(const boost::system::error_code& error_code)
{
    if (error_code == 0)
    {
      std::cout << buffer_.data() << std::endl;

      socket_.async_receive(boost::asio::buffer(buffer_.data(), buflen_),
        boost::bind(&TCPClient::onReceive, this, boost::asio::placeholders::error));
    }
    else
      this->doClose();
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
