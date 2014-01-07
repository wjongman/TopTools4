#!C:/Python27/python.exe

from stringtemplate3 import StringTemplateGroup
from configobj import ConfigObj
from datetime import datetime
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

    config = ConfigObj('./content/index.ini')
##     config = ConfigObj('./content/toptools_single.ini')

    for sectionname in config.sections:
        section = config[sectionname]
        if section.has_key('targetfile'):
            render_page(config, section, sectionname)

        for subsectionname in section.sections:
            subsection = section[subsectionname]
            if subsection.has_key('targetfile'):
                render_page(config, subsection, sectionname)

##-----------------------------------------------------------------------------
def render_page(config, section, sectionname):
    stgroup = StringTemplateGroup("groupName", "./templates")
    st = stgroup.getInstanceOf("index")

    pagename = section['targetfile']

    st["title"] = section['title']
    st["subtitle"] = section['subtitle']
    st["content"] = mark_down(section['content'])
    st["menuitems"] = get_menuitems(config, pagename, sectionname)
    st["lastupdate"] = datetime.now().ctime()
    st["year"] = datetime.now().year
    st["version"] = '4.0.1.67'
    save_page(pagename, str(st).strip())

##-----------------------------------------------------------------------------
def get_menuitems(config, pagename, parentname):
    menuitems = []

    for sectionname in config.sections:
        section = config[sectionname]
        md = MenuItemDescriptor(sectionname)
        md.caption = sectionname
        if section.has_key('targetfile'):
            md.pageurl = section['targetfile']
        else:
            md.pageurl = section['url']

        md.selected = (sectionname == parentname)
        md.submenu = get_submenuitems(section, pagename, parentname)

        menuitems.append(md)

    return menuitems

##-----------------------------------------------------------------------------
def get_submenuitems(section, pagename, parentname):
    submenuitems = []

    for subsectionname in section.sections:
        subsection = section[subsectionname]
        md = MenuItemDescriptor(subsectionname)
        md.caption = subsectionname

        if subsection.has_key('targetfile'):
            md.pageurl = subsection['targetfile']
            md.selected = (subsection['targetfile'] == pagename and
                           subsection['menu'] == parentname)

        elif subsection.has_key('url'):
            md.pageurl = subsection['url']
            md.selected = False

        submenuitems.append(md)

    return submenuitems

##-----------------------------------------------------------------------------
def save_page(pagename, page):
    f = open(pagename, 'w+')
    f.write(page)
    f.close()

##-----------------------------------------------------------------------------
def mark_down(filename):
    f = open('./content/' + filename, 'r+')
    text = f.read()
    f.close()

##     return markdown.markdown(text)
    return markdown.markdown(text, ['tables'])
## ##-----------------------------------------------------------------------------
## def timestamp():
##     return datetime.now()
##
## ##-----------------------------------------------------------------------------
## def current_year():
##     return datetime.now().year
##
##-----------------------------------------------------------------------------
def main():
    render_all_pages()

##-----------------------------------------------------------------------------
main()
