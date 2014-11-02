#include "AMQPHandler.hh"

AMQPHandler::AMQPHandler(const Configuration & conf)
{
  socket_ = std::shared_ptr<TCPClient>(new TCPClient(conf["source"].as<std::string>(), conf["port"].as<int>()));
}

/**
 *  Method that is called by the AMQP library every time it has data
 *  available that should be sent to RabbitMQ.
 *  @param  connection  pointer to the main connection object
 *  @param  data        memory buffer with the data that should be sent to RabbitMQ
 *  @param  size        size of the buffer
 */
void
AMQPHandler::onData(AMQP::Connection *connection, const char *data, size_t size)
{
    // @todo
    //  Add your own implementation, for example by doing a call to the
    //  send() system call. But be aware that the send() call may not
    //  send all data at once, so you also need to take care of buffering
    //  the bytes that could not immediately be sent, and try to send
    //  them again when the socket becomes writable again
    (void)connection;
    (void)data;
    (void)size;
}

/**
 *  Method that is called by the AMQP library when the login attempt
 *  succeeded. After this method has been called, the connection is ready
 *  to use.
 *  @param  connection      The connection that can now be used
 */
void
AMQPHandler::onConnected(AMQP::Connection *connection)
{
    // @todo
    //  add your own implementation, for example by creating a channel
    //  instance, and start publishing or consuming
    (void)connection;
}

/**
 *  Method that is called by the AMQP library when a fatal error occurs
 *  on the connection, for example because data received from RabbitMQ
 *  could not be recognized.
 *  @param  connection      The connection on which the error occured
 *  @param  message         A human readable error message
 */
void
AMQPHandler::onError(AMQP::Connection *connection, const char *message)
{
    // @todo
    //  add your own implementation, for example by reporting the error
    //  to the user of your program, log the error, and destruct the
    //  connection object because it is no longer in a usable state
    (void)connection;
    (void)message;
}

/**
 *  Method that is called when the connection was closed. This is the
 *  counter part of a call to Connection::close() and it confirms that the
 *  connection was correctly closed.
 *
 *  @param  connection      The connection that was closed and that is now unusable
 */
void
AMQPHandler::onClosed(AMQP::Connection *connection) {
  socket_->close();
  (void)connection;
}
