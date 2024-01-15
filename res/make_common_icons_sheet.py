
import os
from PIL import Image

from datetime import datetime



class IconDefinition:

    def __init__(self, iconFile: str, iconName: str):
        self.iconFile = iconFile
        self.iconImage = Image.open(iconFile)

        words = iconName.split('_');
        self.iconName = ''
        for word in words:
            self.iconName += word.capitalize()

        self.iconName = self.iconName[:-4]
        

    def getName(self):        
        return self.iconName

    def getDimensions(self) -> (int,int): 
        return (self.iconImage.width,self.iconImage.height)
    
    def getImage(self) -> Image:
        return self.iconImage
    
    def setSheetPosition(self, sheetPos: (int,int)):
        self.sheetX = sheetPos[0]
        self.sheetY = sheetPos[1]

    def getSheetPosition(self) -> (int,int): 
        return (self.sheetX, self.sheetY)

def IsPixelEmpty(pixel: (int,int,int,int)):
    return pixel[0] == 255 and pixel[1] == 0 and pixel[2] == 255 and pixel[3] == 255

def findFreeSpace(sheet: Image, dimensions: (int,int) ) -> (int,int):

    for y in range(0, sheet.height -  (dimensions[1] + 1 )):
        for x in range(0, sheet.width - (dimensions[0] + 1)):
            
            pixel = sheet.getpixel((x,y))
            
            if IsPixelEmpty(pixel) is False:
                continue

            for tx in range(x, x + dimensions[0]):
                for ty in range(y, y + dimensions[1]):
                    pixel = sheet.getpixel((x,y))

                    if IsPixelEmpty(pixel) is False:
                        continue

            
            return (x,y)
        

    return None

def writeBanner(file):

    file.write(
"""/*
    LightBaker3000 Frontend project,
    (c) {0}-{1}, CrazyRussian
*/

#pragma once

""".format(2022, datetime.now().year))

def makeHeaderFile(icons: [IconDefinition], iconSetName, fileName):

    f = open(fileName,"w")
    writeBanner(f)

    f.write("enum class {0}\n".format(iconSetName))
    f.write("{\n")

    for icon in icons:
        f.write("\t{0},\n".format(icon.getName()))

    f.write("};\n")

    f.write("\n")

    f.write("extern ImVec4 g_{0}Rects[{1}];\n".format(iconSetName, len(icons)))


def makeSourceFile(icons: [IconDefinition], iconSetName, fileName):

    f = open(fileName,"w")
    writeBanner(f)

    f.write("#include \"{0}.h\"\n".format(iconSetName))

    f.write("\n")
    
    f.write("ImVec4 g_{0}Rects[{1}] = {{\n".format(iconSetName, len(icons)))
    
    for icon in icons:

        pos = icon.getSheetPosition()
        dims = icon.getDimensions()

        rect = (pos[0] / 512, pos[1] / 512, (pos[0] + dims[0]) / 512, (pos[1] + dims[1]) / 512)

        f.write("\t{{ {0}, {1}, {2}, {3} }},\n".format(rect[0], rect[1], rect[2], rect[3]))

    f.write("};")


def BuildIconSheet(lookupDir: str, outputImage: str, outputFileName: str):
    files = [f for f in os.listdir(lookupDir) ]

    icons = []

    sheet = Image.new("RGBA",(512,512), (255,0,255,255))
    

    for file in files:

        print('Placing {0}\n'.format(file))

        icon = IconDefinition(lookupDir + '/' + file, file)
        icons.append(icon)
        
        dimensions = icon.getDimensions()
        freeSpace = findFreeSpace(sheet,dimensions) 

        icon.setSheetPosition(freeSpace)

        if freeSpace != None:
            sheet.paste(icon.getImage(), (freeSpace[0], freeSpace[1]))

    ### Header file

    makeHeaderFile(icons, outputFileName, outputFileName + ".h");
    makeSourceFile(icons, outputFileName, outputFileName + ".cpp");

    sheet.save(outputImage)



BuildIconSheet("./common_icons", "common_icons.png", "CommonIcons")