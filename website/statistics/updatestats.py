#!/usr/bin/env python

import ssh
import getpass

password = getpass.getpass()

try:
    conn = ssh.Connection('xs4all.nl', 'wij', password = password)

## except paramiko.AuthenticationException, msg:
##     print msg
##
## except TypeError, msg:
##     print msg

except:
    print 'Authentication failed.'

else:
    result = conn.execute('cd ~/WWW/statistics/; ./update')
    if result == []:
        print 'Update done.'
    else:
        print result

    conn.close()

raw_input("press any key to exit ...")

