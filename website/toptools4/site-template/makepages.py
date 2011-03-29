#!C:/Python27/python.exe

from stringtemplate3 import StringTemplateGroup
import ConfigParser

##-----------------------------------------------------------------------------
class MenuItemDescriptor(object):
    def __init__(self, id):
        self.id = id
        self.caption = ''
        self.pageurl = ''
        self.downloadurl = ''
        self.selected = False

##-----------------------------------------------------------------------------
def render_page(page_id):

    stgroup = StringTemplateGroup("groupName", "./templates")
    st = stgroup.getInstanceOf("main")

    config = ConfigParser.ConfigParser()
    config.read('./content/content.ini')

    section = 'section' + str(page_id)
    st["title"] = config.get(section, 'title')
    st["content"] = config.get(section, 'text')
    st["subtitle"] = config.get(section, 'subtitle')
    st["menuitems"] = get_menuitems(page_id)

    return str(st)

##-----------------------------------------------------------------------------
def get_menuitems(page_id):
    config = ConfigParser.ConfigParser()
    config.read('./content/menu.ini')
    sections = config.sections()

    items = []

    for section in sections:
        md = MenuItemDescriptor(section)
        md.caption = config.get(section, 'caption')
        md.pageurl = config.get(section, 'pageurl')
        md.downloadurl = config.get(section, 'downloadurl')
        md.selected = (section == str(page_id))
        items.append(md)

    return items

##-----------------------------------------------------------------------------
def main():
    f = open("test.htm", 'w+')
    f.write(render_page(4))
    f.close()

##-----------------------------------------------------------------------------
main()
