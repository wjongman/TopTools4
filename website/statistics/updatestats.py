#!/usr/bin/env python

import ssh
import getpass

password = getpass.getpass()

conn = ssh.Connection('xs4all.nl', 'wij', password = password)

result = conn.execute('cd ~/WWW/statistics/; update')
if result == []:
    print 'Update done.'
else:
    print result

conn.close()

## try:
##     conn = ssh.Connection('xs4all.nl', 'wij', password = password)
## except BadAuthenticationType:
##     print 'Authentication failed.'
## else:
##     result = conn.execute('cd ~/WWW/statistics/; update')
##     if result == []:
##         print 'Update done.'
##     else:
##         print result
##
##     conn.close()

