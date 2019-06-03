import os, shutil

# export includes

src = 'Volt3D/'
dst = 'bin/include/Volt3D/'

for src_dir, dirs, files in os.walk(src):
	dst_dir = src_dir.replace(src, dst, 1)
	if not os.path.exists(dst_dir):
		os.makedirs(dst_dir)
	for file_ in files:
		ext = os.path.splitext(file_)[1]
		if ext == ".h":
			src_file = os.path.join(src_dir, file_)
			dst_file = os.path.join(dst_dir, file_)
			if os.path.exists(dst_file):
				os.remove(dst_file)
			shutil.copy(src_file, dst_dir)

if(os.path.exists(dst + '/Volt3D.h')):
	os.remove(dst + '/Volt3D.h')
shutil.copy('Volt3D.h', dst)
