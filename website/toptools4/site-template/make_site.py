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

    config = ConfigObj('./content/index.ini')

    for sectionname in config.sections:
        section = config[sectionname]
        if section.has_key('targetfile'):
            render_page(config, section)

        for subsectionname in section.sections:
            subsection = section[subsectionname]
            if subsection.has_key('targetfile'):
                render_page(config, subsection)

##-----------------------------------------------------------------------------
def render_page(config, section):
    stgroup = StringTemplateGroup("groupName", "./templates")
    st = stgroup.getInstanceOf("index")

    pagename = section['targetfile']

    st["title"] = section['title']
    st["subtitle"] = section['subtitle']
    st["content"] = mark_down(section['content'])
    st["menuitems"] = get_menuitems(config, pagename)

    save_page(pagename, str(st))

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

    return markdown.markdown(text)

##-----------------------------------------------------------------------------
def get_menuitems(config, pagename):
    menuitems = []

    for sectionname in config.sections:
        section = config[sectionname]
        md = MenuItemDescriptor(sectionname)
        md.caption = sectionname
#        md.pageurl = section.iteritems[0]['targetfile']
        md.pageurl = section['targetfile']

        values = section.itervalues()
        for value in values:
            if value == pagename:
                md.selected= True
##        md.selected = (pagename in values)
        md.selected = (pagename == md.pageurl)

##         for subsectionname in section.sections:
##             subsection = section[subsectionname]
##
##             if subsection.has_key('targetfile'):
##                 md.pageurl = subsection['targetfile']
##                 md.selected = (subsection['targetfile'] == pagename)
##             else:
##                 if subsection.has_key('url'):
##                     md.pageurl = subsection['url']
##                     md.selected = False

        md.submenu = get_submenuitems(section, pagename)

        menuitems.append(md)

    return menuitems

##-----------------------------------------------------------------------------
def get_submenuitems(section, pagename):
    submenuitems = []

    for subsectionname in section.sections:
        subsection = section[subsectionname]
        md = MenuItemDescriptor(subsectionname)
        md.caption = subsectionname

        if subsection.has_key('targetfile'):
            md.pageurl = subsection['targetfile']
            md.selected = (subsection['targetfile'] == pagename)
        else:
            if subsection.has_key('url'):
                md.pageurl = subsection['url']
                md.selected = False

        submenuitems.append(md)

    return submenuitems

##-----------------------------------------------------------------------------
def find_section(config, pagename):

    for sectionname in config.sections:
        section = config[sectionname]
        if section.has_key('targetfile') and section['targetfile'] == pagename:
            return section

        for subsectionname in section.sections:
            subsection = section[subsectionname]
            if subsection.has_key('targetfile') and subsection['targetfile'] == pagename:
                return subsection

    return None

##-----------------------------------------------------------------------------
def main():
    render_all_pages()

##-----------------------------------------------------------------------------
main()
