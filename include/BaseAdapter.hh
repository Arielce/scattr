#ifndef   __BASE_ADAPTER_HH__
# define  __BASE_ADAPTER_HH__

# include <string>
# include <boost/program_options.hpp>
# include <boost/thread.hpp>
# include <queue>
# include "Configuration.hh"

# define  CONFIG_PATH(file)   "/etc/needbook/scattr/adapters/" file
# define  MAX_ELEMS_IN_QUEUE  3

namespace po = boost::program_options;

namespace Adapters
{
  class BaseAdapter
  {
  public:
    typedef std::function<void(uint64_t)> handled_t;
  private:
    struct Message {
      std::string message;
      uint64_t tag;
      handled_t function;

      Message() {}
      Message(const std::string & message, uint64_t tag, handled_t function) : message(message), tag(tag), function(function) {}
      void handled() const
      {
        function(tag);
      }
    };

    std::queue<const Message> messages_;
    std::list<std::shared_ptr<boost::thread>> threads_;
    boost::mutex                  mutex_;
    boost::condition_variable     condition_;
    size_t                        nbr_;
  public:
    BaseAdapter();
    /*!
     * Will interupt the threads before exiting (not during a message handing)
     *
     */
    virtual ~BaseAdapter();
    /*!
     * Get the name of the adapter, will also be the name of the AMQP routing key.
     */
    virtual std::string getName() const = 0;
    /*!
     * Add configuration parameters, such as command line options,
     * which the Adapter will be available to retrieve later on.
     * You should override this method in your Adapter.
     */
    virtual void addConfiguration(po::options_description &);
    /*!
     * Init your adapter with given configuration.
     * Refer to the documentation for examples on how to use the Configuration class.
     */
    virtual bool init(const Configuration &);
    /*!
     * This function is not virtual because it is not aimed at being overrided.
     * It will secure the environment and your message() function will be called
     * when it is time.
     */
    void handleMessage(const std::string & message, uint64_t, handled_t);
    /*!
     * Careful, if your adapter fails, the message will be lost!
     * Your algorithm must start in this function, you can use whatever you want
     */
    virtual void message(const std::string & message) = 0;
    /*!
     * This function is used to relaunch the adapter thread if it has exited
     * for some reason. You don't need to worry about that if you're creating
     * your own adapter. You should not override this function, thus why it is
     * not virtual.
     */
    void refresh();
    /*!
     * Get the number of messages handled by this adapter
     * since the instance beginning.
     */
    size_t getNbr() const;
  private:
    void run();
    void launchThread();
  };
};

#endif
