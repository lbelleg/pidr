    
require 'pathname'

def absolute(f)
    Pathname.new(f).realpath.to_s
end

here = File.dirname(__FILE__)
serveur = File.join(here, "/Server.jar")
client = File.join(here, "/Client.jar")
serveur = absolute(serveur)
client = absolute(client)
$ssh = ssh = "ssh root@"

nodes = File.read(ARGV.first).split().uniq
fichier = absolute(ARGV.last)

puts "We have #{nodes.length} nodes..."

first = nodes.first
rest = nodes[1...-1]

puts "First = #{first}"
puts "Rest = #{rest.inspect}"

def launch(cmd)
    puts "Running: #{cmd}"
    `#{cmd}`
end

def killall(where)
    launch "#{$ssh}#{where} -- killall chaisend 2> /dev/null"
end

port = 10000
rest.each do |node|
    killall node
    launch "#{ssh}#{node} -- screen -d -m -S ScreenServeur -- java -jar #{serveur} #{port} fichierRecu.iso"
end

puts "Waiting..."
sleep 4

tempsDebut=Time.now.to_f
rest.each do |node|
    launch "#{ssh}#{first} -- screen -d -m -S ScreenFirst#{node} -- java -jar #{client} #{node} #{port} #{port}"
end


rest.each do |node|
    launch "#{ssh}#{node} -- screen -d -m -S ScreenServeur -- sync"
    port += 1
    current = node
end

tempsFin=Time.now.to_f

puts "Time : #{tempsFin-tempsDebut}s"

