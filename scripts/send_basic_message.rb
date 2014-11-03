#!/usr/bin/env ruby
# encoding: utf-8

require "bunny"
conn = Bunny.new
conn.start

ch   = conn.create_channel
q    = ch.queue("nb_notifier_queue")
q.publish("Hello World!", :routing_key => 'Android')
puts " [x] Sent 'Hello World!'"

conn.close
