#!/bin/bash

if [ "$EUID" -ne 0 ]
then
    echo $'sam-password-setup: Run sam-password-setup as root.'
    exit
fi

cp -r `find /home/ -type d -name sam-password-main` /tmp/sam-password
mv /tmp/sam-password/sam-password.sh /bin/sam-password
gcc -o /bin/sam-password-encrypter /tmp/sam-password/sam-password-encrypter.c
chmod 755 /bin/sam-password
chmod 755 /bin/sam-password-encrypter
rm -r /tmp/sam-password
