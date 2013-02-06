#!/bin/bash

set -x

for n in $(cat $OAR_FILE_NODES | sort | uniq)
do
   scp ~/.ssh/id_rsa root@${n}:.ssh/ > /dev/null
   ssh-copy-id root@${n} > /dev/null
done
