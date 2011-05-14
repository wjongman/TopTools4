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
    content = config[pagename]['text']
    html = markdown.markdown(content)
    st["content"] = html
#    st["content"] = config[pagename]['text']
    st["subtitle"] = config[pagename]['subtitle']
    st["menuitems"] = get_menuitems(config, pagename)
#    submenuitems =
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

    submenus = config[sectionname]['submenu']
    for submenu in submenus:
        smd = MenuItemDescriptor(submenu)
        smd.caption = submenu
        smd.pageurl = submenus[submenu]
        submenuitems.append(smd)

    return submenuitems

##-----------------------------------------------------------------------------
def main():
    render_all_pages()

##-----------------------------------------------------------------------------
main()
