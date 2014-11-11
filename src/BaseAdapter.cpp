#include "BaseAdapter.hh"

Adapters::BaseAdapter::BaseAdapter()
  : nbr_(0)
{
}

Adapters::BaseAdapter::~BaseAdapter()
{
}

void
Adapters::BaseAdapter::addConfiguration(po::options_description & desc)
{
  (void)desc;
}

bool
Adapters::BaseAdapter::init(const Configuration & configuration)
{
  (void)configuration;
  return true;
}

void
Adapters::BaseAdapter::handleMessage(const std::string & str, uint64_t tag, handled_t finished)
{
  {
    boost::mutex::scoped_lock lock(mutex_);
    Adapters::BaseAdapter::Message message(str, tag, finished);

    messages_.push(message);
  }
  condition_.notify_one();
}

void
Adapters::BaseAdapter::run()
{
  Adapters::BaseAdapter::Message message;

  while (42)
  {
    {
      boost::mutex::scoped_lock  lock(mutex_);
      if (threads_.size() > (messages_.size() / MAX_ELEMS_IN_QUEUE))
        return ;
      while (messages_.empty())
        condition_.wait(lock);
      boost::this_thread::disable_interruption di;
      nbr_++;
      message = messages_.front();
      messages_.pop();
    }
    this->message(message.message);
    message.handled();
  }
}

void
Adapters::BaseAdapter::launchThread()
{
  threads_.push_back(std::make_shared<boost::thread>(boost::bind(&Adapters::BaseAdapter::run, this)));
}

void
Adapters::BaseAdapter::refresh()
{
  auto it = threads_.begin();
  while (it != threads_.end())
  {
    if (!(*it)->timed_join(boost::posix_time::milliseconds(0)))
      ++it; // still alive
    else
      it = threads_.erase(it);
  }
  if (threads_.size() < (messages_.size() / MAX_ELEMS_IN_QUEUE))
    this->launchThread();
}

size_t
Adapters::BaseAdapter::getNbr() const
{
  return nbr_;
}
