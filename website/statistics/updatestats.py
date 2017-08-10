#!/usr/bin/env python
import paramiko
import getpass

servername = 'shell.xs4all.nl'
username = 'wij'

def ssh_connect(url, username, password):
    ssh = paramiko.SSHClient()
    ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    ssh.connect(url, 22, username=username, password=password)
    return ssh

password = getpass.getpass()

try:
    conn = ssh_connect(servername, username, password)
except:
    print 'Could not connect.'
else:
    stdin, stdout, stderr = conn.exec_command('cd ~/WWW/statistics/; ./update')
    data = stdout.read().splitlines()
    if data == []:
        print 'Update done.'
    else:
        for line in data:
            print line

    conn.close()

raw_input("Press <Enter> to exit ...")

