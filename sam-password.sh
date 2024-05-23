#!/bin/bash

if [ "$EUID" -eq 0 ]
then
    echo $'sam-password: Run sam-password as a non-root user.'
    exit
fi

OPTS=$(getopt -o hn:g:e: --long help,new:,get:,exists: -n 'sam-password' -- $@)
if [ $# -lt 1 ]
then
	echo $'sam-password: Invalid number of arguments. Check inputs and try again or run "sam-password -h" for more information.'
else
	eval set -- "$OPTS"
	while true; do
		case "$1" in
			-h | --help)
				echo $'Usage:\n sam-password [option] [sitename]\n\nOptions:\n -h, --help\t\tPrint this help message\n -n, --new [sitename]\tGenerate an encrypted password for a specific site name\n\t\t\tand copy this password to the clipboard\n -g, --get [sitename]\tCopy an existing encrypted password for a specific site\n\t\t\tname to the clipboard\n -e, --exists [sitename]\tCheck if password encryption keys exist for a\n\t\t\t\tspecific site name'
				;;
			-n | --new)
				shift
				site=$1
				read -s -p $'Enter master password\n' passwd
				sam-password-encrypter r $passwd $site | xsel -b
				echo $'Encrypted password copied to clipboard. Execute "xsel -b -c" to clear clipboard.'
				;;
			-g | --get)
				shift
				site=$1
				sam-password-encrypter ' ' ' ' $site ' ' &> /dev/null
				if [ $? -eq 1 ]
				then
					echo "sam-password: Password does not exist for site name \"$site\""
				else
					read -s -p $'Enter master password.\n' passwd
					echo -n `sam-password-encrypter g $passwd $site` | xsel -b
					echo $'Encrypted password copied to clipboard. Execute "xsel -b -c" to clear clipboard.'
				fi
				;;
			-e | --exists)
				shift
				site=$1
				sam-password-encrypter ' ' ' ' $site ' ' &> /dev/null
				if [ $? -eq 1 ]
				then
					echo "Password does not exist for site name \"$site\""
				else
					echo "Password exists for site name \"$site\""
				fi
				;;
			--)
				shift
				break
				;;
		esac
		shift
	done
fi
