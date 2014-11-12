#!/usr/bin/env ruby
# encoding: utf-8

require "bunny"
conn = Bunny.new
conn.start

keys = ['android', 'ios']

ch   = conn.create_channel
x    = ch.direct("nb_scattr_exch")

[*0..20].each do |i|
  key = keys.sample
  x.publish("Hello World!", :routing_key => key, persistent: true)
  puts " [x] Sent 'Hello World on #{key} queue!'"
end

conn.close
