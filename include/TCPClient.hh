#ifndef   __TCP_CLIENT_HH__
# define  __TCP_CLIENT_HH__

# define  TCPBUFLEN 128

# include <iostream>
# include <stdio.h>
# include <ostream>

# include <boost/thread.hpp>
# include <boost/bind.hpp>
# include <boost/array.hpp>
# include <boost/asio.hpp>

using boost::asio::ip::tcp;

class TCPClient
{
public:
  enum action
  {
    CONNECTED,
    RECEIVED
  };
  typedef std::function<void(TCPClient::action, boost::array<char, TCPBUFLEN> &)> callback_t;
private:
  boost::asio::io_service io_service_;
  tcp::socket socket_;

  boost::array<char, TCPBUFLEN> buffer_;
  size_t buflen_;
  std::shared_ptr<boost::thread> thread_;
  callback_t callback_;
public:
  TCPClient(const std::string &, int);

  void write(const std::string &);
  void close();
  void onAction(callback_t);
  std::shared_ptr<boost::thread> run();
private:
  void onConnect(const boost::system::error_code &, tcp::resolver::iterator);
  void onReceive(const boost::system::error_code &);
  void onWrite(const boost::system::error_code &, std::size_t);
  void doClose();
};

#endif
