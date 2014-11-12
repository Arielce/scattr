Scattr
========

Scattr is the perfect program to send notifications to different devices in a scalable and distributed way!  
If you're already using a message broker (AMQP, such as RabbitMQ) or want to use one, then install Scattr and let it do the job for you!

Developed in C++, the project should be capable to handle a crazy number of notifications, and if it is still not enough for you,
just lauch another instance of the program on a different server!

**This project is currently under development**, and even though it is already a good codebase if you feel like developing your own adapter,
it is still not usable right away. Wait for us to make the main adapters, such as *Android Cloud Messaging* and *Apple push notification*.

Progress
========

The architecture has been set up, and the project is almost usable right now if you feel like developing the adapters.
I still have to develop some extra classes that will help during the development of those, but it shouldn't take too long.

Install
=======

*A docker file and a vagrant file are coming*, but if you want to install *Scattr* yourself, please install all the dependencies (see Requirements below) and then:

```bash
make
```

The binary file will be locater in `./bin/notiifer`

FAQ
===

How does it work?
-----------------

*Scattr* will get every message sent to a specific queue in your RabbitMQ and will call the adapter you want, passing it the message.  
The adapter can makes whatever it wants, even though this program has been thought to send notifications to devices, you can also use an
email adapter, or do time-consuming tasks.

Why is it fast?
---------------

*Scattr* has been coded in C++ to improve general performances, but that's not all. This program is multithreaded and threads are created when you need them!
If an adapter has too much messages to handle, a new thread will be created to accelerate it, and the thread will terminate itself once everything is back to normal.

**Some load tests are yet to come**

How to add my own adapter
-------------------

Just create your own adapter under the adapters directory. You **must** have a class inheriting from the `Adapters::BaseAdapter` abstract class.

When you're done, in order for Scattr to use it, you must add it in the `AdaptersFactory` class. To do so, open `src/AdaptersFactory.cpp`, `#include` your header file,
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

If you develop your own adapter and you think it can be useful to other people, you are encouraged to send a pull request with it.

Requirements
============

If you choose to install **Scattr** yourself, here are the dependencies:

  - C++ 11
  - Boost library (program_options, assign, asio, thread) **HAS to be compiled using c++11!!**
  - CPPUnit (unit testing library)
  - [AMQP-CPP](https://github.com/CopernicaMarketingSoftware/AMQP-CPP)
  - [MacGPusher](https://github.com/needbook/MacGPusher)
  - [jsoncpp](https://github.com/open-source-parsers/jsoncpp)

For the scripts located under `scripts`, which can be useful when developing on the project, you will need ruby with the following gems:

  - bunny
