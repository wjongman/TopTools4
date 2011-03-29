#!C:/Python27/python.exe

from stringtemplate3 import StringTemplateGroup
import ConfigParser

##-----------------------------------------------------------------------------
class MenuItemDescriptor(object):
    def __init__(self, id):
        self.id = id
        self.caption = ''
        self.href = ''
        self.subtitle = ''
        self.selected = False

##-----------------------------------------------------------------------------
def render_page():

    stgroup = StringTemplateGroup("groupName", "./templates")
    st = stgroup.getInstanceOf("main")

    config = ConfigParser.ConfigParser()
    config.read('./content/content.ini')

    st["title"] = config.get('section1', 'title')
    st["content"] = config.get('section1', 'text')
    st["subtitle"] = config.get('section1', 'subtitle')

##     config.read('./content/menu.ini')
    for i in range(1,4):
        md = MenuItemDescriptor(i)
        md.caption = 'titel' + str(i)
        md.pageurl = 'http://titel' + str(i)
        md.downloadurl = 'http://titel/download' + str(i)
        md.selected = (i == 1)

        st["menuitems"] = md

    return str(st)

##-----------------------------------------------------------------------------
def main():
    f = open("test.htm", 'w+')
    f.write(render_page())
    f.close()
##     print get_content('content.ini', 'section1')

##-----------------------------------------------------------------------------
main()
