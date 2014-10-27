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
private:
  boost::asio::io_service io_service_;
  tcp::socket socket_;

  boost::array<char, TCPBUFLEN> buffer_;
  size_t buflen_;
  std::shared_ptr<boost::thread> thread_;
public:
  TCPClient(const std::string &, int);

  void write();
  void close();
private:
  void onConnect(const boost::system::error_code &, tcp::resolver::iterator);
  void onReceive(const boost::system::error_code &);
  void doClose();
  std::shared_ptr<boost::thread> run();
};

#endif
