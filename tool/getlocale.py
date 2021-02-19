#!/usr/bin/python

import sys
import os
import re

dryrun = False


class PESection:
	VAddr = 0
	VSize = 0
	Addr = 0
	Size = 0
	Name = ""

class FileHash:
	filename = ""
	hsh = ""

class ResNamed:
	nameOFf = 0
	sz      = 0

class ResID:
	ID = 0
	off = 0

class ResDir:
	named = None
	ids   = None

Sections = dict()

def read4int(fl):
	return int.from_bytes( fl.read(4) , byteorder='little' )

def read2int(fl):
	return int.from_bytes( fl.read(2) , byteorder='little' )

def readSections(fl):
	global Sections
	Sections.clear()
	
	fl.seek(0)
	if fl.read(2) != b'MZ':
		return False
	
	fl.seek(0x3C) #e_lfanew
	PE_POS = read4int(fl)
	
	fl.seek(PE_POS)
	if read4int(fl) != 0x4550:
		return False
	
	fl.seek(PE_POS + 6) #sec Numb
	secNumb = read2int(fl)
	
	i = 0
	while i < secNumb:
		fl.seek(PE_POS + 0xF8 + i * 0x28) #sections
		name = fl.read(8)
		pes = PESection()
		pes.VSize = read4int(fl)
		pes.VAddr = read4int(fl)
		pes.Size  = read4int(fl)
		pes.Addr  = read4int(fl)
		pes.Name = name.decode("ascii", errors='ignore').strip('\x00')
		
		Sections[pes.Name] = pes
		
		i += 1
	
	return True

def ReadResDir(fl):
	fl.seek(12, 1)
	nmd = read2int(fl)
	ids = read2int(fl)
	
	dr = ResDir()
	dr.named = list()
	dr.ids = list()
	
	for i in range(nmd):
		r = ResNamed()
		r.nameOFf = read4int(fl) & 0xFFFFFF
		r.sz = read4int(fl) & 0xFFFFFF
		dr.named.append(r)
	
	for i in range(ids):
		r = ResID()
		r.ID = read4int(fl) & 0xFFFFFF
		r.off = read4int(fl) & 0xFFFFFF
		dr.ids.append(r)
	
	return dr

##### main:
if len(sys.argv) < 2:
	print("Usage: python3 getlocale.py locale.dll [locale.lng]")
	os.exit(0)
	
f = open(sys.argv[1], "rb")
readSections(f)

out = None

if len(sys.argv) > 2:
	out = open(sys.argv[2], "wb")
else:
	nm = sys.argv[1]
	nm = re.sub("\.(...)", ".", nm) + "lng"
	out = open(nm, "wb")


if ".rsrc" in Sections:
	rsrc = Sections[".rsrc"]
	f.seek(rsrc.Addr)
	
	doff = rsrc.Addr - rsrc.VAddr

	root = ReadResDir(f)
	for rt in root.ids:
		if (rt.ID == 6):
			f.seek(rsrc.Addr + rt.off)
			dir1 = ReadResDir(f)
			
			for d1 in dir1.ids:
				
				#print(hex(d1.ID))
				f.seek(rsrc.Addr + d1.off)
				
				dir2 = ReadResDir(f) #Here ID - is langID?
								
				for d2 in dir2.ids:
					
					f.seek(rsrc.Addr + d2.off)
					
					
					strsOff = read4int(f) + doff
					sz = read4int(f)
					
					txtID = 0
					
					f.seek(strsOff)
					while(sz > 0):
						strLen = read2int(f)
						sz -= 2
						s = f.read(strLen * 2).decode("UTF-16LE")
						sz -= strLen * 2
						
						if strLen:
							s = s.replace("\n", "\\")
							fm = "{:d}  = {}\n".format(((d1.ID - 1) << 4) + txtID, s)
							out.write(fm.encode("UTF-8"))
						
						txtID += 1

out.close()
