CC = gcc
CXX = g++
MINGW = /usr/i686-w64-mingw32
CC_WIN = i686-w64-mingw32-gcc
CXX_WIN = i686-w64-mingw32-g++
LIBS_WIN = -L$(MINGW)/lib/ -lmingw32  -mwindows -lole32 -lgdi32 -ldxguid -ldinput -lddraw -limm32
TARGET = UA_source
OBJS =  \
	ade.cpp\
	area.cpp\
	amesh.cpp\
        bitmap.cpp\
	base.cpp\
	bmpAnm.cpp\
	button.cpp\
	classes.cpp\
	def_parser.cpp\
	display.cpp\
	embed.cpp\
	engine_gfx.cpp\
	engine_input.cpp\
	engine_miles.cpp\
	engine_tform.cpp\
	font.cpp\
	glob_funcs.cpp\
	idev.cpp\
	ilbm.cpp\
	ini.cpp\
	input.cpp\
	itimer.cpp\
	iwimp.cpp\
	log.cpp\
	main.cpp\
	MC.cpp\
	MFILE.cpp\
	nlist.cpp\
	nucleas.cpp\
	particle.cpp\
	raster.cpp\
	rsrc.cpp\
	sample.cpp\
	skeleton.cpp\
	sklt.cpp\
	utils.cpp\
	wav.cpp\
	win3d.cpp\
	windd.cpp\
	winp.cpp\
	wintimer.cpp\
	yw.cpp\
	yw_funcs.cpp\
	yw_func2.cpp\
	yw_protos.cpp
	
WINOBJ = \
	win_funct.cpp

win32: $(OBJS)
	$(CXX_WIN) -mno-ms-bitfields -O0 -g -o $(TARGET).exe $(OBJS) $(LIBS_WIN) -m32 -I $(MINGW)/include/

clean:
	rm -f $(TARGET) $(TARGET_DINGOO) *.o