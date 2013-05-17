#!/bin/bash
#Param 1 = nb node, 2 = fichier à envoyer
echo "Nb Node       : $1"
echo "Source        : $2"


head -n $1 listNode > nodes

ruby runUdp.rb nodes $2

rm nodes