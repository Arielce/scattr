#include "BaseAdapter.hh"

Adapters::BaseAdapter::BaseAdapter()
{
  this->launchThread();
}

void
Adapters::BaseAdapter::addConfiguration(po::options_description & desc)
{
  (void)desc;
}

void
Adapters::BaseAdapter::handleMessage(const std::string & message)
{
  std::cout << "Received message, queue size is: " << messages_.size() << std::endl;
  boost::mutex::scoped_lock lock(mutex_);

  if (messages_.size() >= MAX_ELEMS_IN_QUEUE)
    condition_.wait(lock);
  messages_.push(message);
  condition_.notify_one();
  std::cout << "Message handled" << std::endl;
}

void
Adapters::BaseAdapter::run()
{
  std::string message;
  boost::this_thread::disable_interruption di;

  while (42)
  {
    {
      boost::mutex::scoped_lock  lock(mutex_);

      if (messages_.empty())
        condition_.wait(lock);
      message = messages_.front();
      messages_.pop();
    }
    this->message(message);
    condition_.notify_one();
  }
}

void
Adapters::BaseAdapter::launchThread()
{
  thread_ = std::make_shared<boost::thread>(boost::bind(&Adapters::BaseAdapter::run, this));
}

void
Adapters::BaseAdapter::refresh()
{
  if (thread_->timed_join(boost::posix_time::milliseconds(0)))
    return; // still alive
  this->launchThread();
}
