#!/usr/bin/env python

import os
import sys
import os.path
import fnmatch
import re
import time

#------------------------------------------------------------------------------
def isHtml(file):
    return fnmatch.fnmatch(file,'*.html')

#------------------------------------------------------------------------------
def isZip(file):
    return fnmatch.fnmatch(file,'*.zip')

#------------------------------------------------------------------------------
def hasExtension(file, ext):
    return fnmatch.fnmatch(file,'*.' + ext)

#------------------------------------------------------------------------------
def getPreSections(html):
    """Extract all <pre> sections and return their content as a list
    """
    pre = []

    end = 0
    while 1:
        start = html.find('<pre>', end)
        if start < 0:
            break
        end = html.find('</pre>', start)
        pre.append(html[start+6:end])

    return pre

#------------------------------------------------------------------------------
def processDir(arg, path, files):
    for file in filter(isHtml, files):
        processFile(arg, path, file)

#------------------------------------------------------------------------------
def processFile(arg, path, file):
    countsofar = 0
    counts = getMonthCount(arg, path, file)
    results = getDayCount(arg, path, file)

    for result in results:
        daystat = result + counts
        countsofar += int(result[2])
        daystat.append(str(countsofar))

        day_traffic.append(daystat)

#------------------------------------------------------------------------------
def getMonthCount(arg, path, file):
    """Count number of downloads for certain files
    """
    fullname = os.path.join(path, file)
    html = open(fullname).read()

    # first <pre> element contains list of downloads in month
    pre = getPreSections(html)[0]
    lines = str(pre).splitlines()

    hits243 = '0'
    hits300 = '0'
    hits400 = '0'

    for line in lines:
        if line.find('toptools243.exe') > 0:
            parts = line.strip().split()
            hits243 = parts[3]

        if line.find('Setup_TopTools30.exe') > 0:
            parts = line.strip().split()
            hits300 = parts[3]

        if line.find('TopTools4_00_52.zip') > 0:
            parts = line.strip().split()
            hits400 = parts[3]

    month = dateFromFilename(file)
    return [month, hits243, hits300, hits400]

#------------------------------------------------------------------------------
def getDayCount(arg, path, file):
    """Count number of downloads per day for month this file is covering
    """
    fullname = os.path.join(path, file)
    html = open(fullname).read()

    # third <pre> element contains downloads per day (in Kb)
    pre = getPreSections(html)[2]
    lines = str(pre).splitlines()

    monthhits = []

    for line in lines:
        parts = line.strip().split()
        if len(parts) > 3:
            date = convertDate(parts[0] + ' ' + parts[1] + ' ' + parts[2])
            hits = parts[3]
            kbstr = parts[4]
            monthhits.append([date, str(approxDownloads(kbstr, 1)), str(approxDownloads(kbstr, 600))])

    return monthhits

#------------------------------------------------------------------------------
def approxDownloads(kbstr, size=1):
    kbytes = kbstr[1:-3]
    return int(kbytes) / size

#------------------------------------------------------------------------------
def convertDate(rawdate):
    """Convert date format; ie. 27 Dec 2009 becomes 2009-12-27
    """
    ts = time.strptime(rawdate, "%d %b %Y")
    return time.strftime("%Y-%m-%d", ts)

#------------------------------------------------------------------------------
def dateFromFilename(filename):
    """Convert filename; ie. Dec2009.html becomes 2009-12
    """
    ts = time.strptime(filename, "%b%Y.html")
    return time.strftime("%Y-%m", ts)

#------------------------------------------------------------------------------
day_traffic = []

#------------------------------------------------------------------------------
if __name__ == '__main__':
    folder = os.curdir
    os.path.walk(folder, processDir, None)

    day_traffic.sort()
    for traffic in day_traffic:
        print traffic[0] + ';' + traffic[1] + ';' + traffic[2] + ';' + traffic[4] + ';' + traffic[5] + ';' + traffic[6] + ';' + traffic[7]


