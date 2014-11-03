#include "TCPClient.hh"

TCPClient::TCPClient(const std::string & server, int port)
  : socket_(io_service_), connected_(false), buflen_(TCPBUFLEN), server_(server), port_(port)
{
  this->connect();
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
    condition_.notify_one();
    if (this->callback_)
      this->callback_(TCPClient::CONNECTED, std::string());
    socket_.async_receive(boost::asio::buffer(buffer_.data(), buflen_),
      boost::bind(&TCPClient::onReceive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    return;
  }
  if (end_point_iter != tcp::resolver::iterator())
  {
    socket_.close();
    connected_ = false;
    socket_.async_connect(*end_point_iter,
      boost::bind(&TCPClient::onConnect, this, boost::asio::placeholders::error, ++end_point_iter));
    return;
  }
  boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
  this->connect();
}

void
TCPClient::connect()
{
  std::ostringstream os;

  os << port_;
  this->close();
  connected_ = false;
  tcp::resolver resolver(io_service_);
  tcp::resolver::query query(server_, os.str());
  tcp::resolver::iterator end_point_iter = resolver.resolve(query);

  socket_.async_connect(*end_point_iter,
    boost::bind(&TCPClient::onConnect, this, boost::asio::placeholders::error, ++end_point_iter));
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
  {
    this->close();
    this->connect();
  }
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

void
TCPClient::wait()
{
  boost::unique_lock<boost::mutex> lock(mutex_);
  while (!this->good())
    condition_.wait(lock);
}
