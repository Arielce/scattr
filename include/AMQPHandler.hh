#ifndef   __AMQP_HANDLER_HH__
# define  __AMQP_HANDLER_HH__

# include <amqpcpp.h>
# include "Configuration.hh"
# include "AdaptersFactory.hh"
# include "TCPClient.hh"

class AMQPHandler : public AMQP::ConnectionHandler
{
private:
  static const std::string EXCHANGE_NAME;
  static const std::string QUEUE_NAME;
  static const int QUALITY_OF_SERVICE;
protected:
  std::shared_ptr<TCPClient> socket_;
  std::shared_ptr<AMQP::Connection> connection_;
  std::unique_ptr<AMQP::Channel> channel_;
private:
  void action(TCPClient::action, const std::string &);
  void onData(AMQP::Connection *, const char *, size_t);
  void onConnected(AMQP::Connection *);
  void onError(AMQP::Connection *, const char *);
  void onClosed(AMQP::Connection *);
  void onReceived(const AMQP::Message &, uint64_t, bool);
public:
  AMQPHandler(const Configuration &);
};

#endif
