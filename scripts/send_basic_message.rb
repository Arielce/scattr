#!/usr/bin/env ruby
# encoding: utf-8

require "bunny"
conn = Bunny.new
conn.start

ch   = conn.create_channel
x    = ch.fanout("nb_notifier_exch")
q    = ch.queue("nb_notifier_queue", { durable: true })
x.publish("Hello World!", :routing_key => 'Android')
puts " [x] Sent 'Hello World!'"

conn.close
