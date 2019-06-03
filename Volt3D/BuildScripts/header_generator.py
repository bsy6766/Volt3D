import glob

fileStr = "/**\n" + \
"*	@file Volt3D.h\n" + \
"*\n" + \
"*	@author Seung Youp Baek\n" + \
"*	@copyright Copyright (c) 2019 Seung Youp Baek\n" + \
"*/\n" + \
"\n" + \
"#ifndef VOLT3D_H\n" + \
"#define VOLT3D_H\n"

subDirLUT = set([])

rootSize = len("Volt3D\\")

for filename in glob.iglob('Volt3D/**/*.h', recursive=True):
	nextDir = filename[rootSize:].find('\\')
	subDir = filename[rootSize:nextDir+rootSize]
	if subDir not in subDirLUT:
		subDirLUT.add(subDir)
		fileStr += "\n// " + subDir + "\n";
	fileStr += "#include \"" + filename[rootSize:].replace("\\", "/") + "\"\n"


fileStr += "\n#endif"

file = open("Volt3D.h", 'w')
file.write(fileStr)
file.close()
