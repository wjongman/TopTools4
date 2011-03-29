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
def render_all_pages():

    config = ConfigParser.ConfigParser()
    config.read('./content/content.ini')
    pagenames = config.sections()
    for pagename in pagenames:
        page = render_page(config, pagename)
        f = open(pagename, 'w+')
        f.write(page)
        f.close()

##-----------------------------------------------------------------------------
def render_page(config, pagename):

    stgroup = StringTemplateGroup("groupName", "./templates")
    st = stgroup.getInstanceOf("main")

    st["title"] = config.get(pagename, 'title')
    st["content"] = config.get(pagename, 'text')
    st["subtitle"] = config.get(pagename, 'subtitle')
    st["menuitems"] = get_menuitems(config.get(pagename, 'id'))

    return str(st)

##-----------------------------------------------------------------------------
def get_menuitems(page_id):
    items = []

    config = ConfigParser.ConfigParser()
    config.read('./content/menu.ini')
    sections = config.sections()
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
    render_all_pages()
##     f = open("test.htm", 'w+')
##     f.write(render_page(4))
##     f.close()

##-----------------------------------------------------------------------------
main()
