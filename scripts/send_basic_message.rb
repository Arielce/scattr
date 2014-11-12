#!/usr/bin/env ruby
# encoding: utf-8

require "bunny"
conn = Bunny.new
conn.start

ch   = conn.create_channel
x    = ch.direct("nb_scattr_exch")

x.publish("Hello World!", :routing_key => 'android', persistent: true)
puts " [x] Sent 'Hello World on Android queue!'"

conn.close
