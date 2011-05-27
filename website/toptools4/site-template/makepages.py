#!C:/Python27/python.exe

from stringtemplate3 import StringTemplateGroup
from configobj import ConfigObj
import markdown

##-----------------------------------------------------------------------------
class MenuItemDescriptor(object):
    def __init__(self, id):
        self.id = id
        self.caption = ''
        self.pageurl = ''
        self.selected = False
        self.submenu = []

##-----------------------------------------------------------------------------
class SubMenuItemDescriptor(object):
    def __init__(self, id):
        self.id = id
        self.caption = ''
        self.pageurl = ''
        self.selected = False
        self.parentmenu = ''

##-----------------------------------------------------------------------------
def render_all_pages():
    config = ConfigObj('./content/content.ini')
    pagenames = config.sections
    for pagename in pagenames:
        page = render_page(config, pagename)
        f = open(pagename, 'w+')
        f.write(page)
        f.close()

##-----------------------------------------------------------------------------
def render_page(config, pagename):
    stgroup = StringTemplateGroup("groupName", "./templates")
    st = stgroup.getInstanceOf("index")

    st["title"] = config[pagename]['title']
    st["subtitle"] = config[pagename]['subtitle']

    text = config[pagename]['text']
    st["content"] = markdown.markdown(text)
    st["menuitems"] = get_menuitems(config, pagename)
    return str(st)

##-----------------------------------------------------------------------------
def get_menuitems(config, pagename):
    menuitems = []

    sectionnames = config.sections
    for sectionname in sectionnames:
        md = MenuItemDescriptor(sectionname)
        md.pageurl = sectionname
        md.caption = config[sectionname]['title']
        md.selected = (sectionname == pagename)
        md.submenu = get_submenuitems(config, sectionname)

        menuitems.append(md)

    return menuitems

##-----------------------------------------------------------------------------
def get_submenuitems(config, sectionname):
    submenuitems = []

    if config[sectionname].has_key('submenu'):
        submenus = config[sectionname]['submenu']
        for submenu in submenus:
            smd = SubMenuItemDescriptor(submenu)
            smd.caption = submenu
            smd.pageurl = submenus[submenu]
            smd.selected = (sectionname == smd.pageurl)
#            smd.parentmenu = submenu['parentmenu']

            submenuitems.append(smd)

    return submenuitems

##-----------------------------------------------------------------------------
def main():
    render_all_pages()

##-----------------------------------------------------------------------------
main()
