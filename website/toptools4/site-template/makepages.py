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
    st["menuitems"] = get_menuitems(config, pagename)

    return str(st)

##-----------------------------------------------------------------------------
def get_menuitems(config, pagename):
    menuitems = []

    sectionnames = config.sections()
    for sectionname in sectionnames:
        md = MenuItemDescriptor(sectionname)
        md.pageurl = sectionname
        md.caption = config.get(sectionname, 'title')
        md.downloadurl = config.get(sectionname, 'downloadurl')
        md.selected = (sectionname == pagename)

        menuitems.append(md)

    return menuitems

##-----------------------------------------------------------------------------
def main():
    render_all_pages()

##-----------------------------------------------------------------------------
main()
