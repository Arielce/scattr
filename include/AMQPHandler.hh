#ifndef   __AMQP_HANDLER_HH__
# define  __AMQP_HANDLER_HH__

# include <amqpcpp.h>
# include "Configuration.hh"
# include "AdaptersFactory.hh"
# include "TCPClient.hh"

class AMQPHandler : public AMQP::ConnectionHandler
{
public:
  /*!
   * Exception class specific to AMQP errors
   */
  class Error : public std::runtime_error {
  public:
    enum error_type {
      UNKNOWN,
      EXCHANGE_DECL,
      QUEUE_DECL,
      QUEUE_BIND,
      CONSUMING_START
    };
  protected:
    error_type  type_;
    std::string error_;
  public:
    Error(error_type, const std::string &);
    virtual ~Error() throw () {};
    virtual const char* what() const throw ();
  };
private:
  static const std::string VHOST;
  static const std::string EXCHANGE_NAME;
  static const std::string QUEUE_NAME;
  static const int QUALITY_OF_SERVICE;
protected:
  std::shared_ptr<TCPClient>    socket_;
  std::shared_ptr<AMQP::Connection> connection_;
  std::unique_ptr<AMQP::Channel> channel_;
  std::string                   username_;
  std::string                   password_;
  void action(TCPClient::action, const std::string &);
  void onData(AMQP::Connection *, const char *, size_t);
  void onConnected(AMQP::Connection *);
  void onError(AMQP::Connection *, const char *);
  void onClosed(AMQP::Connection *);
  void onReceived(const AMQP::Message &, uint64_t, bool);
  void onHandled(uint64_t);
public:
  AMQPHandler(const Configuration &);
  virtual ~AMQPHandler();
};

#endif
