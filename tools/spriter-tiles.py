import sys
import argparse
from PIL import Image
from pathlib import Path

TRANSPARENT_COLOR = 0

class Character:
	def __init__(self, name, outputdir, binaryoutputdir):
		self.name = name
		self.outputdir = outputdir
		self.binaryoutputdir = binaryoutputdir
		self.animations = []
		self.numanimations = 0
		self.empty = True
		self.maxspritesused = 0
	def addanimation(self, animation):
		if(animation.maxspritesused > self.maxspritesused):
			self.maxspritesused = animation.maxspritesused
		self.animations.append(animation)
		self.numanimations = self.numanimations + 1
		self.empty = False
	def addcfgfile(self, configurationfilename):
		configurationfile = open(configurationfilename,'r')
		configurationcontent = configurationfile.readlines()
		configurationfile.close()
		for configurationcontentline in configurationcontent:
			configurationcontentline = configurationcontentline.strip()
			if(configurationcontentline[0] != "#"):
				configurationcontentline = configurationcontentline.split("//")[0]
				configurationvalues = configurationcontentline.split(",")
				for animation in self.animations:
					if(str(animation.animationnumber) == configurationvalues[0]):
						animation.addconfiguration(configurationvalues)
		
	def writecfile(self):
		print("Exporting character " + self.name)
		outputfile = open(self.outputdir + "/" + self.name + "character.h",'w')
		outputfile.write("static const character " + self.name + "_character = \n")
		outputfile.write("{\n")
		outputfile.write("\t" + str(self.numanimations) + ",//numanimations\n")
		outputfile.write("\t" + str(self.maxspritesused) + ",//maxsprites\n")
		outputfile.write("\t{\n")
		i = 0
		for animation in self.animations:
			animation.writecfile(outputfile)
			i = i + 1
		outputfile.write("\t}\n")	
		outputfile.write("};\n")
		outputfile.close()
	def writebinfile(self):
		for animation in self.animations:
			animation.writebinfile(self.name, self.binaryoutputdir)

class Animation:
	def __init__(self, animationnumber):
		self.frames = []
		self.mirroredframes = []
		self.animationnumber = animationnumber
		self.numframes = 0
		self.empty = True
		self.maxspritesused = 0
	def addframe(self, frame):
		if(frame.numsprites > self.maxspritesused):
			self.maxspritesused = frame.numsprites
		self.frames.append(frame)
		self.numframes = self.numframes + 1
		self.empty = False
	def addmirroredframe(self, frame):
		self.mirroredframes.append(frame)
	def addconfiguration(self,configurationvalues):
		for frame in self.frames:
			if(str(frame.framenumber) == configurationvalues[1]):
				frame.addconfiguration(configurationvalues)
		for frame in self.mirroredframes:
			if(str(frame.framenumber) == configurationvalues[1]):
				frame.addmirroredconfiguration(configurationvalues)
	def writecfile(self, outputfile):
		print("Exporting animation number " + str(self.animationnumber))
		outputfile.write("\t\t{\n")
		outputfile.write("\t\t\t" + str(self.animationnumber)+",//animation number\n")
		outputfile.write("\t\t\t" + str(self.numframes)+",//numframes\n")
		outputfile.write("\t\t\t{\n")
		i = 0
		for frame in self.frames:
			frame.writecfile(outputfile)
			i = i + 1
		outputfile.write("\t\t\t},\n")
		outputfile.write("\t\t\t{\n")
		for frame in self.mirroredframes:
			frame.writecfile(outputfile)
			i = i + 1
		outputfile.write("\t\t\t}\n")
		outputfile.write("\t\t},\n")
	def writebinfile(self, name, directory):
		for frame in self.frames:
			frame.writebinfile(name, directory)
		for frame in self.mirroredframes:
			frame.writebinfile(name, directory)


class Frame:
	def __init__(self, name, animationnumber, framenumber, mirrored):
		self.sprites = []
		self.framewidth = 0
		self.frameheight = 0
		self.animationnumber = animationnumber
		self.framenumber = framenumber
		self.mirrored = mirrored
		self.numsprites = 0
		self.framename = name + "animation" + str(animationnumber) + "frame" +str(self.framenumber)
		if(self.mirrored):
				self.framename = self.framename + "mirrored"
		self.frametime = 0
		self.collisionx1 = 0
		self.collisiony1 = 0
		self.collisionx2 = 0
		self.collisiony2 = 0
		self.hitboxx1 = 0
		self.hitboxy1 = 0
		self.hitboxx2 = 0
		self.hitboxy2 = 0
		self.empty = True
	def quantize(self, frameimageobject):
		(self.framewidth, self.frameheight) = frameimageobject.size
		numrcolumns = int(self.framewidth / 8)
		numrows = int(self.frameheight / 8)
		currenttilenumber = 0
		for row in range(0,numrows):
			for column in range(0, numrcolumns):
				if(not self.mirrored):
					spriteimageobject = frameimageobject.crop((column*8,row*8,(column+1)*8,(row+1)*8))
				else:
					spriteimageobject = frameimageobject.crop((self.framewidth - (column+1)*8, row*8, self.framewidth - column*8, (row+1)*8))
				sprite = Sprite(currenttilenumber, column, row)
				sprite.quantize(spriteimageobject, self.mirrored)
				if(sprite.empty == False):
					self.sprites.append(sprite)
					currenttilenumber = currenttilenumber + 1
		self.numsprites = len(self.sprites)
		if(self.numsprites > 0):
			self.empty = False
	def addconfiguration(self,configurationvalues):
		self.frametime = int(configurationvalues[2])
		self.collisionx1 = int(configurationvalues[3])
		self.collisiony1 = int(configurationvalues[4])
		self.collisionx2 = int(configurationvalues[5])
		self.collisiony2 = int(configurationvalues[6])
		self.hitboxx1 = int(configurationvalues[7])
		self.hitboxy1 = int(configurationvalues[8])
		self.hitboxx2 = int(configurationvalues[9])
		self.hitboxy2 = int(configurationvalues[10])
	def addmirroredconfiguration(self, configurationvalues):
		self.frametime = configurationvalues[2]
		self.collisionx1 = int(configurationvalues[3]) * -1
		self.collisiony1 = int(configurationvalues[4]) 
		self.collisionx2 = int(configurationvalues[5]) * -1
		self.collisiony2 = int(configurationvalues[6])
		self.hitboxx1 = int(configurationvalues[7]) * -1
		self.hitboxy1 = int(configurationvalues[8]) 
		self.hitboxx2 = int(configurationvalues[9]) * -1
		self.hitboxy2 = int(configurationvalues[10]) 
	def writecfile(self, outputfile):
		outputfile.write("\t\t\t\t{\n")
		outputfile.write("\t\t\t\t\t" + str(self.framenumber)+",//frame number\n")
		if(self.mirrored):
			outputfile.write("\t\t\t\t\ttrue,//frame mirrored\n")
		else:
			outputfile.write("\t\t\t\t\tfalse,//frame mirrored\n")
		outputfile.write("\t\t\t\t\t" + str(self.numsprites)+",//numsprites\n")
		if(self.empty):
			outputfile.write("\t\t\t\t\t0,//frame name\n")
			outputfile.write("\t\t\t\t\t0,//frame size\n")
			outputfile.write("\t\t\t\t\t0,//frame bank\n")
		else:
			outputfile.write("\t\t\t\t\t" + str(self.framename)+"_bin,//frame name\n")
			outputfile.write("\t\t\t\t\t" + str(self.framename)+"_bin_size,//frame size\n")
			outputfile.write("\t\t\t\t\t" + str(self.framename)+"_bin_bank,//frame bank\n")
		outputfile.write("\t\t\t\t\t" + str(self.frametime)+",//frame time\n")
		outputfile.write("\t\t\t\t\t" + str(self.collisionx1)+",//collision x1\n")
		outputfile.write("\t\t\t\t\t" + str(self.collisiony1)+",//collision y1\n")
		outputfile.write("\t\t\t\t\t" + str(self.collisionx2)+",//collision x2\n")
		outputfile.write("\t\t\t\t\t" + str(self.collisiony2)+",//collision y2\n")
		outputfile.write("\t\t\t\t\t" + str(self.hitboxx1)+",//hitbox x1\n")
		outputfile.write("\t\t\t\t\t" + str(self.hitboxy1)+",//hitbox y1\n")
		outputfile.write("\t\t\t\t\t" + str(self.hitboxx2)+",//hitbox x2\n")
		outputfile.write("\t\t\t\t\t" + str(self.hitboxy2)+",//hitbox y2\n")
		outputfile.write("\t\t\t\t\t{\n")
		i = 0
		for sprite in self.sprites:
			sprite.writecfile(outputfile)
			i = i + 1
		outputfile.write("\t\t\t\t\t}\n")
		outputfile.write("\t\t\t\t},\n")
	def writebinfile(self, name, directory):
		outputfile = open(directory + "/" + self.framename + ".bin",'wb')
		for sprite in self.sprites:
			sprite.writebinfile(outputfile)
		outputfile.close()

class Sprite:
	def __init__(self, tilenumber, column, row):
		self.tilenumber = tilenumber
		self.dpx = column * 8
		self.dpy = row * 8
		self.empty = True
	def quantize(self, spriteimageobject, mirrored):
		self.data = []
		rawdata = spriteimageobject.getdata()
		numberpixels = len(rawdata)
		for pixel in range(0, numberpixels):
			pixeldata = rawdata[pixel]
			if(pixeldata != TRANSPARENT_COLOR):
				self.empty = False
			self.data.append(pixeldata)
		if(mirrored):
			self.dpx = - self.dpx
	def writecfile(self, outputfile):
		outputfile.write("\t\t\t\t\t\t{\n")
		outputfile.write("\t\t\t\t\t\t\t" + str(self.tilenumber)+ ",//tile number\n")
		outputfile.write("\t\t\t\t\t\t\t" + str(self.dpx)+",//desplazamiento x\n")
		outputfile.write("\t\t\t\t\t\t\t" + str(self.dpy)+"//desplazamiento y\n")
		outputfile.write("\t\t\t\t\t\t},\n")
	def writebinfile(self, outputfile):
		masks = [0x01, 0x02, 0x04, 0x08]
		for row in range(0,8):
			for bit in range(0,4):
				mask = masks[bit]
				byte = 0
				for column in range(0,8):
					data = self.data[(row*8) + column]
					data = data & mask
					data = data >> bit
					byte = byte + (data << (7 - column))
				byte = byte.to_bytes(1, byteorder='big')
				outputfile.write(byte)

def managespritesheet(spritesheetfilename, name, outputdir, binaryoutputdir, widthframe, heightframe, mirrorframe, verbose):
		try:
			spritesheetimageobject = Image.open(spritesheetfilename).copy()
		except:
			print("Cannot find file called " + spritesheetfilename)
			exit()
		if spritesheetimageobject.mode != "P":
			print("Sprite sheet must be in indexed mode!")
			exit()
		(spritesheetwidth, spritesheetheight) = spritesheetimageobject.size
		if(verbose):
			print("SpriteSheet is " + str(spritesheetwidth) + " pixels wide and " + str(spritesheetheight) + " pixels tall")
		heightframepixels = heightframe * 8
		widthframepixels = widthframe * 8
		numanimations = int(spritesheetheight / heightframepixels)
		maxnumframes = int(spritesheetwidth / widthframepixels)
		if verbose:
			print("num animations is " + str(numanimations))
			print("longest animation has " + str(maxnumframes) + " frames")
		animationnumber = 0
		if(verbose):
			print("Creating character " + name)
		character = Character(name, outputdir, binaryoutputdir)
		for animationnumber in range(0, numanimations):
			animationimageobject = spritesheetimageobject.crop((0,animationnumber * heightframepixels,spritesheetwidth,(animationnumber + 1) * heightframepixels))
			animation = manageanimation(animationimageobject, character.name, widthframe, heightframe, animationnumber, mirrorframe)
			if(animation.empty == False):
				if verbose:
					print("Animation number " + str(animation.animationnumber) + " is " + str(animation.numframes) + " frames long")
				character.addanimation(animation)
		return character

def manageanimation(animationimageobject, name, widthframe, heightframe, animationnumber, mirrorframe):
	(imagewidth, imageheight) = animationimageobject.size
	heightframepixels = heightframe * 8
	widthframepixels = widthframe * 8
	numframes = int(imagewidth / widthframepixels)
	animation = Animation(animationnumber)
	for framenumber in range(0, numframes):
		frameimageobject = animationimageobject.crop((framenumber*widthframepixels,0,(framenumber + 1) * widthframepixels,heightframepixels))
		frame = manageframe(frameimageobject, name, animationnumber, framenumber, False)
		if(frame.empty == False):
			animation.addframe(frame)
			if(mirrorframe):
				mirroredframeimageobject = frameimageobject.transpose(Image.FLIP_LEFT_RIGHT)
				mirroredframe = manageframe(mirroredframeimageobject, name, animationnumber, framenumber, True)
				animation.addmirroredframe(mirroredframe)
	return animation

def manageframe(frameimageobject, name, animationnumber, framenumber, mirrored):
	frame = Frame(name, animationnumber, framenumber, mirrored)
	frame.quantize(frameimageobject)
	return frame

def writeanimations(animationlist):
	for animation in animationlist:
		print(animation)

def main():
	print("\n****Kusfo's Sprite Splitter***********\n")
	parser = argparse.ArgumentParser(description='Process spritesheets for MSB project')
	parser.add_argument('spritesheetfilename', metavar='F',
	        help='the filename for the spritesheet')
	parser.add_argument('-w','--widthframe', metavar='w', type=int,
	                    help='width of frame in sprites', default=4)
	parser.add_argument('-e','--heightframe', metavar='e', type=int,
	                    help='height of frame in sprites', default=4)
	parser.add_argument('-m','--mirrorframe', metavar='m', 
	                    help='mirror every animation', default=False)
	parser.add_argument('-c','--configuration', metavar='c',
	                    help='configuration file to use', default="")
	parser.add_argument('-o','--outputdir', metavar='o',
	                    help='outputdirectory for h file', default="")
	parser.add_argument('-b','--binaryoutputdir', metavar='b',
	                    help='output directory for bin files', default="")
	parser.add_argument('-v','--verbose',metavar='v',
						help='verbose process for debug purpouse', default=False)
	parser.add_argument('-n','--name',metavar='n',
						help='character name', default="character")



	args = parser.parse_args()
	print("processing: " +args.spritesheetfilename)
	character = managespritesheet(args.spritesheetfilename, args.name, args.outputdir, args.binaryoutputdir, args.widthframe, args.heightframe, args.mirrorframe, args.verbose)
	if(args.configuration != ""):
		character.addcfgfile(args.configuration)
	character.writecfile()
	character.writebinfile()

if __name__ == '__main__':
    main() 