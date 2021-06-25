#!/usr/bin/python

import xml.dom.minidom
import xml.etree.ElementTree as ET
import os
import sys


def createResource(path):
    root = ET.Element('RCC')

    attrib = {'prefix': '/'}
    doc = ET.SubElement(root, 'qresource', attrib)

    if traverseDirectory(path, doc):
        print(f"updating {path}")
        xml_str = prettify(root)
        qrc_file_name = 'resource.qrc'
        f = open(path + '/' + qrc_file_name, 'w+')
        f.write(xml_str)
        f.close()


def prettify(elem):
    """Return a pretty-printed XML string for the Element.
    """
    rough_string = ET.tostring(elem, 'utf-8')
    reparsed = xml.dom.minidom.parseString(rough_string)
    return reparsed.toprettyxml(indent="\t")


def traverseDirectory(dir, root):
    files = []
    for r, _, f in os.walk(dir):
        for file in f:
            file_path = os.path.join(r, file)
            file_path = file_path.replace('\\', '/')
            file_path = file_path[len(dir)+1:]
            if file_path.endswith(".qml") | file_path.endswith(".jpg") | file_path.endswith(".png"):
                files.append(file_path)

    if not files:
        return False

    for f in files:
        elem = ET.SubElement(root, 'file')
        elem.text = f
    
    return True

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print(f"invalid arguments")
        exit

    createResource(sys.argv[1])
