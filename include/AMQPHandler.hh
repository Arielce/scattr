#ifndef   __AMQP_HANDLER_HH__
# define  __AMQP_HANDLER_HH__

# include <amqpcpp.h>
# include "Configuration.hh"
# include "TCPClient.hh"

class AMQPHandler : public AMQP::ConnectionHandler
{
protected:
  std::shared_ptr<TCPClient> socket_;
private:
  AMQPHandler(const Configuration &);
  /**
   *  Method that is called by the AMQP library every time it has data
   *  available that should be sent to RabbitMQ.
   *  @param  connection  pointer to the main connection object
   *  @param  data        memory buffer with the data that should be sent to RabbitMQ
   *  @param  size        size of the buffer
   */
  void onData(AMQP::Connection *, const char *, size_t);

  /**
   *  Method that is called by the AMQP library when the login attempt
   *  succeeded. After this method has been called, the connection is ready
   *  to use.
   *  @param  connection      The connection that can now be used
   */
  void onConnected(AMQP::Connection *);
  /**
   *  Method that is called by the AMQP library when a fatal error occurs
   *  on the connection, for example because data received from RabbitMQ
   *  could not be recognized.
   *  @param  connection      The connection on which the error occured
   *  @param  message         A human readable error message
   */
  void onError(AMQP::Connection *, const char *);
  /**
   *  Method that is called when the connection was closed. This is the
   *  counter part of a call to Connection::close() and it confirms that the
   *  connection was correctly closed.
   *
   *  @param  connection      The connection that was closed and that is now unusable
   */
  void onClosed(AMQP::Connection *);
};

#endif
