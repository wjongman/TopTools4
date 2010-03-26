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
class FullTraffic:
    def __init__(self, daydata):
        self.date = daydata[0]
        self.traffic = daydata[1]
        self.downloads = daydata[2]
        self.month = daydata[3]
        self.hits243 = daydata[4]
        self.hits300 = daydata[5]
        self.hits400 = daydata[6]
        self.sofar = daydata[7]

    def printCSV(self):
        """
        Print as comma separated values
        """
        print self.date + ';' + \
              self.traffic + ';' + \
              self.downloads + ';' + \
              self.month + ';' + \
              self.hits243 + ';' + \
              self.hits300 + ';' + \
              self.hits400 + ';' + \
              self.sofar + '\n'

#------------------------------------------------------------------------------
class DayTraffic:
    def __init__(self, daydata):
        self.date = daydata[0]
        self.traffic = daydata[1]
        self.downloads = daydata[2]

    def printCSV(self):
        """
        Print as comma separated values
        """
        print self.date + ';' + \
              self.traffic + ';' + \
              self.downloads

#------------------------------------------------------------------------------
class MonthTraffic:
    def __init__(self, data):
        self.month = data[0]
        self.hits243 = data[1]
        self.hits300 = data[2]
        self.hits400 = data[3]

    def printCSV(self):
        """
        Print as comma separated values
        """
        print self.month + ';' + \
              self.hits243 + ';' + \
              self.hits300 + ';' + \
              self.hits400

#------------------------------------------------------------------------------
def getPreSections(html):
    """
    Extract all <pre> sections and return their content as a list
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
    """
    Fill traffic tables
    """
    fullname = os.path.join(path, file)
    html = open(fullname).read()

    month = dateFromFilename(file)
    month_downloads[month] = getMonthDownloads(html, [])
    day_traffic.update(getDayTraffic(html))

#------------------------------------------------------------------------------
def getMonthDownloads(html, filenames):
    """
    Count number of downloads for some hardcoded
    filenames in month this file is covering
    """
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

    return [hits243, hits300, hits400]

#------------------------------------------------------------------------------
def getDayTraffic(html):
    """
    Count number of downloads per day
    in month this file is covering
    """
    # third <pre> element contains downloads per day (in Kb)
    pre = getPreSections(html)[2]
    lines = str(pre).splitlines()

    daytraffic = {}

    for line in lines:
        # format of line:  18 Nov 2009       278  (5423Kb)
        parts = line.strip().split()
        if len(parts) > 3:
            date = convertDate(parts[0] + ' ' + parts[1] + ' ' + parts[2])
            hits = parts[3]
            kbstr = parts[4]
            daytraffic[date] = [hits, kbstr]

    return daytraffic

#------------------------------------------------------------------------------
def processFile0(arg, path, file):
    """
    Fill traffic tables
    """
    monthstats = []
    countsofar = 0

    monthcount = getMonthCount(arg, path, file)
    month_traffic.append(monthcount)

    daycounts = getDayCount(arg, path, file)
    for daycount in daycounts:
        traffic_per_day.append(daycount)

#------------------------------------------------------------------------------
def getMonthCount(arg, path, file):
    """
    Count number of downloads for some hardcoded
    filenames in month this file is covering
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
    """
    Count number of downloads per day
    in month this file is covering
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
    """
    Convert date format; ie. 27 Dec 2009 becomes 2009-12-27
    """
    ts = time.strptime(rawdate, "%d %b %Y")
    return time.strftime("%Y-%m-%d", ts)

#------------------------------------------------------------------------------
def dateFromFilename(filename):
    """
    Convert filename; ie. Dec2009.html becomes 2009-12
    """
    ts = time.strptime(filename, "%b%Y.html")
    return time.strftime("%Y-%m", ts)

#------------------------------------------------------------------------------
def printCSV_0(traffic):
    """
    Print as comma separated values
    """
    print traffic[0] + ';' + \
          traffic[1] + ';' + \
          traffic[2] + ';' + \
          traffic[4] + ';' + \
          traffic[5] + ';' + \
          traffic[6] + ';' + \
          traffic[7]

#------------------------------------------------------------------------------
def buildBluffRepr(trafficdata):
    """
    Reorder data so it can be used by the Bluff graphing script
    trafficdata is a list of lists
    """
##     g.data('Apples', [1, 2, 3, 4, 4, 3]);
##     g.data('Oranges', [4, 8, 7, 9, 8, 9]);
##     g.data('Watermelon', [2, 3, 1, 5, 6, 8]);
##     g.data('Peaches', [9, 9, 10, 8, 7, 9]);
##     g.labels = {0: '2003', 2: '2004', 4: '2005'};

##     daytraffic = DayTraffic(alltraffic)
##     fulltraffic = FullTraffic(alltraffic)

    ## Have a dictionary indexed by date
    stats = {}

    for traffic in trafficdata:
        monthtraffic = MonthTraffic(traffic)
        stats[monthtraffic.month] = [monthtraffic.hits243, monthtraffic.hits300 , monthtraffic.hits400]

    return stats

## ToDo: retrieve data directly in appropriate class and maintain list of class-instances
##


#------------------------------------------------------------------------------
traffic_per_day = []
month_traffic = []
all_traffic = []

month_downloads = {}
day_traffic = {}
#------------------------------------------------------------------------------
if __name__ == '__main__':
    folder = os.curdir + '/test'
    os.path.walk(folder, processDir, None)

    print month_downloads
    print day_traffic

##     month_traffic.sort()
##     for traffic in month_traffic:
##         monthtraffic = MonthTraffic(traffic)
##         monthtraffic.printCSV()
## ##         buildBluffRepr(traffic)
##
##     traffic_per_day.sort()
##     for traffic in traffic_per_day:
##         daytraffic = DayTraffic(traffic)
##         daytraffic.printCSV()
##
##     print buildBluffRepr(month_traffic)

