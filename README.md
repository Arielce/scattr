Notifier
========

A simple C++ notifier which allows you to send notifications to different devices.

Project is currently under development.

Progress
========

The architecture has been set up, and the project is almost usable right now if you feel like developing the adapters.
I still have to develop some extra classes that will help during the development of those, but it shouldn't take too long.

Feel free to participate on the project!

Requirements
============

  - C++ 11
  - Boost library (program_options, assign, asio, thread)
  - CPPUnit (unit testing library)
  - [https://github.com/CopernicaMarketingSoftware/AMQP-CPP](AMQP-CPP)

For the scripts located under `scripts`, which can be useful when developing on the project, you will need ruby with the following gems:

  - bunny
