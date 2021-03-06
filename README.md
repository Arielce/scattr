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

Docker
------

A Dockerfile is there for you to use:

```bash
cd scattr
docker build -t scattr .
# Run scattr this way:
docker run scattr --help
# or, for it to run in the background:
docker run -d scattr # ... params
```


DIY
---

If you want to install *Scattr* yourself, please install all the dependencies (see Requirements below) and then:

```bash
make
```

The binary file will be located in `./bin/scattr`

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

How to send a message to an adapter
-----------------------------------

You will have to send the messages to queues named this way: `nb_scattr_queue_adaptername`.  
The format is json in all the adapters that are available in this repository. Pay attention to custom adapters as they may not be using JSON for their adapters.  
If you want more information about the fields that an adapter awaits, please see its own README, which should be located in its directory.

Libraries exist in the following languages (all adapters may not be supported, as they will only support JSON):

  - Node.js, [scattr-nodejs](https://github.com/needbook/scattr-nodejs).

How to add my own adapter
-------------------

Just create your own adapter under the adapters directory. You **must** have a class inheriting from the `Adapters::BaseAdapter` abstract class. Preferably, you'll add your adapter in the `Adapters` namespace.

When you're done, in order for Scattr to use it, you must add it in the `AdaptersFactory` class. To do so, open `src/AdaptersFactory.cpp`, `#include` your header file,
and in the `AdaptersFactory::AdaptersFactory` constructor function, add you adapter this way:

```cpp
AdaptersFactory::AdaptersFactory()
{
  //...
  add<Adapters::YourAdapter>();
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
  - Boost library (program_options, assign, asio, thread) ***>= 1.54.0***
  - CPPUnit (unit testing library)
  - [AMQP-CPP](https://github.com/needbook/AMQP-CPP)
  - [MacGPusher](https://github.com/needbook/MacGPusher)
  - [jsoncpp](https://github.com/needbook/jsoncpp)

For the scripts located under `scripts`, which can be useful when developing on the project, you will need ruby with the following gems:

  - bunny
