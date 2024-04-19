# sam-password
A Bash-based password manager for Linux that uses (a bad implementation of) AES-128
for encryption. Generates encrypted passwords based on (less secure) user passwords, 
and permanently stores nothing but encryption keys. Thus, one or more passwords 
that are easy to remember can be used to generate far stronger passwords for many 
different sites, but neither the user's nor the final passwords will need to be 
stored on the user's device. The user will need to clear their clipboard after using 
passwords, as `sam-password` uses `xsel` to copy passwords to the clipboard so that 
they never appear on-screen.
# Setup
1. Install `xsel` with the package manager of your choice.
2. Download this repository as a zip file, then unzip it.
3. Execute `chmod +x` on the file in sam-password-main `sam-password-setup.sh`.
4. Run `sam-password-setup.sh` with `sudo`.
5. You can now delete both the repository's zip file and its unzipped directory.
`sam-password` and `sam-password-encrypter` should both be found in `/bin`.
# Usage
* Generate encryption keys for a password:
    1. Execute `sam-password -n sitename` to create a new encrypted password for
    `sitename` and copy it to the clipboard. Encryption keys will be stored in 
    `~/.sam-password-keys`.
    2. Execute `xsel -b -c` to clear the clipboard after you use the password.
* Use existing encryption keys for a password:
    1. Execute `sam-password -g sitename` to copy the password for `sitename` to 
    the clipboard. This only works if `sam-password -n sitename` has previously 
    been run, and `sitename`'s section has not been removed from 
    `~/.sam-password-keys`.
    2. Execute `xsel -b -c` to clear the clipboard after you use the password.
# Deleting encryption keys
`sam-password` writes all encryption keys to `~/.sam-password-keys`. This file's 
formatting will be as follows:
```
foo
[128 bit hex value]
[128 bit hex value]

bar
[128 bit hex value]
[128 bit hex value]

baz
[128 bit hex value]
[128 bit hex value]
```
To delete existing encryption keys for a certain site, delete that site's name 
from this file as well as the two hex values beneath it. As long as the file is 
formatted in the same way, `sam-password` should have no problems with parsing 
it.
