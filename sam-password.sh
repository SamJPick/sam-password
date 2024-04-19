#!/bin/bash

if [ "$EUID" -eq 0 ]
then
    echo $'sam-password: Run sam-password as a non-root user.'
    exit
fi

if [ $# -lt 1 ]
then
	echo $'sam-password: Invalid number of arguments. Check inputs and try again or run "sam-password -h" for more information.'
else
	while getopts "hn:g:" o; do
		case $o in
			h)
				echo $'Usage:\nsam-password [option]\n\nOptions:\n-h\t\tDisplay this help information\n-n sitename\tGenerate an encrypted password for a specific site and copy this password to the clipboard (replace "sitename" with the name of the site that the password is for)\n-g sitename\tCopy an existing encrypted password for a specific site to the clipboard (replace "sitename" with the name of the site that the password is for)'
				;;
			n)
				site=$2
				read -s -p $'Enter master password\n' passwd
				sam-password-encrypter r $passwd $site | xsel -b
				echo $'Encrypted password copied to clipboard. Run "xsel -b -c" to clear clipboard.'
				;;
			g)
				site=$2
				read -s -p $'Enter master password.\n' passwd
				out=`sam-password-encrypter g $passwd $site`
				if [ $? -eq 1 ]
				then
					echo 'sam-password: site name' $site 'not found'
				else
					echo -n $out | xsel -b
					echo $'Encrypted password copied to clipboard. Run "xsel -b -c" to clear clipboard.'
				fi
				;;
		esac
	done
fi
