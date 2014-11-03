#ifndef   __TCP_CLIENT_HH__
# define  __TCP_CLIENT_HH__

# define  TCPBUFLEN 1024

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
  typedef std::function<void(TCPClient::action, const std::string &)> callback_t;
private:
  boost::asio::io_service io_service_;
  tcp::socket socket_;
  bool connected_;

  boost::array<char, TCPBUFLEN> buffer_;
  size_t buflen_;

  std::string server_;
  int port_;
  std::shared_ptr<boost::thread> thread_;
  callback_t callback_;
  boost::mutex mutex_;
  boost::condition_variable condition_;
public:
  TCPClient(const std::string &, int);

  void write(const std::string &);
  void close();
  void onAction(callback_t);
  bool good() const;
  void wait();
  std::shared_ptr<boost::thread> run();
private:
  void onConnect(const boost::system::error_code &, tcp::resolver::iterator);
  void onReceive(const boost::system::error_code &, std::size_t);
  void onWrite(const boost::system::error_code &, std::size_t);
  void doClose();
  void connect();
};

#endif
