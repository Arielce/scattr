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
Adapters::BaseAdapter::handleMessage(const std::string & str, uint64_t tag, handled_t finished)
{
  std::cout << "Received message, queue size is: " << messages_.size() << std::endl;
  boost::mutex::scoped_lock lock(mutex_);

  if (messages_.size() >= MAX_ELEMS_IN_QUEUE)
    condition_.wait(lock);
  Adapters::BaseAdapter::Message message(str, tag, finished);
  messages_.push(message);
  condition_.notify_one();
  std::cout << "Message handled" << std::endl;
}

void
Adapters::BaseAdapter::run()
{
  Adapters::BaseAdapter::Message message;
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
    this->message(message.message);
    condition_.notify_one();
    message.handled();
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
  if (!thread_->timed_join(boost::posix_time::milliseconds(0)))
    return; // still alive
  this->launchThread();
}
