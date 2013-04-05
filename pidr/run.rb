
require 'pathname'

def absolute(f)
    Pathname.new(f).realpath.to_s
end

here = File.dirname(__FILE__)
chainsend = File.join(here, "chainsend")
chainsend = absolute(chainsend)
$ssh = ssh = "oarsh"

nodes = File.read(ARGV.first).split().uniq
fichier = absolute(ARGV.last)

puts "We have #{nodes.length} nodes..."

first = nodes.first
last = nodes.last
rest = nodes[1...-1]

puts "First = #{first}"
puts "Last = #{last}"
puts "Rest = #{rest.inspect}"

def launch(cmd)
    puts "Running: #{cmd}"
    `#{cmd}`
end

def killall(where)
    launch "#{$ssh} #{where} -- killall chaisend 2> /dev/null"
end

killall first
launch "#{ssh} #{first} -- screen -d -m -S first -- #{chainsend} -p 10000 --file /tmp/qq-first -v"

port = 10000
current = first
rest.each do |node|
    killall node
    launch "#{ssh} #{node} -- screen -d -m -S node-#{port} -- #{chainsend} -p #{port+1} --next #{current}:#{port} --file /tmp/qq-#{port} -v"
    port += 1
    current = node
end

puts "Waiting..."
sleep 4

File.open("temporary", "w") do |f|
    l = "cat #{fichier} | #{chainsend} -p #{port+1} --next #{current}:#{port} --stdin"
    f.write(l)
end

killall last
launch "cat temporary | #{ssh} #{last} -- tee /tmp/launcher"
exec "#{ssh} #{last} -- time bash /tmp/launcher"

