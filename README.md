Notifier
========

Notifier is the perfect program to send notifications to different devices in a scalable and distributed way!  
If you're already using a message broker (AMQP, such as RabbitMQ) or want to use one, then install notifier and let it do the job for you!

Developed in C++, the project should be capable to handle a crazy number of notifications, and if it is still not enough for you,
just lauch another instance of the program on a different server!

**This project is currently under development**, and even though it is already a good codebase if you feel like developing your own adapter,
it is still not usable right away. Wait for us to make the main adapters, such as *Android Cloud Messaging* and *Apple push notification*.

Progress
========

The architecture has been set up, and the project is almost usable right now if you feel like developing the adapters.
I still have to develop some extra classes that will help during the development of those, but it shouldn't take too long.

Help
====

Add you own adapter
-------------------

Just create your own adapter under the adapters directory. You **must** have a class inheriting from the `Adapters::BaseAdapter` abstract class.

When you're done, in order for notifier to use it, you must add it in the `AdaptersFactory` class. To do so, open `src/AdaptersFactory.cpp`, `#include` your header file,
and in the `AdaptersFactory::AdaptersFactory` constructor function, add you adapter this way:

```cpp
AdaptersFactory::AdaptersFactory()
{
  //...
  add<Adapters::AndroidAdapter>();
  //...
}
```

I recommend to take a look at the already existing adapters as an example, though a precise documentation will be available when this will be stable.

Contribute
==========

This project is maintained by the Needbook team, but every contribution is welcome, just for the project and send pull requests!

If you develop your own adapter and you think it can be useful to other people, you are encourage to send a pull request with it.

Requirements
============

  - C++ 11
  - Boost library (program_options, assign, asio, thread)
  - CPPUnit (unit testing library)
  - [https://github.com/CopernicaMarketingSoftware/AMQP-CPP](AMQP-CPP)

For the scripts located under `scripts`, which can be useful when developing on the project, you will need ruby with the following gems:

  - bunny
