CC = gcc
CXX = g++
MINGW = /usr/i686-w64-mingw32
CC_WIN = i686-w64-mingw32-gcc
CXX_WIN = i686-w64-mingw32-g++
LIBS_WIN = -L$(MINGW)/lib/ -lmingw32 -mwindows -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_net -lOpenAL32 -lopengl32 -lvorbisfile

CLANG_WIN = i686-w64-mingw32-clang++

LIBS_NIX = -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_net -lGL -lopenal -lvorbisfile

CC_MAC = clang
CXX_MAC = clang++
LIBS_MAC = -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_net -framework OpenGL -framework OpenAL -lvorbisfile

TARGET = UA_source
OBJS =  \
	3ds.cpp\
	ade.cpp\
	area.cpp\
	amesh.cpp\
	bitmap.cpp\
	base.cpp\
	bmpAnm.cpp\
	button.cpp\
	crc32.cpp\
	def_parser.cpp\
	display.cpp\
	embed.cpp\
	engine_gfx.cpp\
	engine_input.cpp\
	engine_miles.cpp\
	engine_tform.cpp\
	font.cpp\
	fsmgr.cpp\
	glob_funcs.cpp\
	idev.cpp\
	ilbm.cpp\
	image.cpp\
	ini.cpp\
	input.cpp\
	itimer.cpp\
	iwimp.cpp\
	log.cpp\
	lstvw.cpp\
	main.cpp\
	MC.cpp\
	IFFile.cpp\
	network.cpp\
	netlib/client.cpp\
	netlib/client_threads.cpp\
	netlib/server.cpp\
	netlib/server_session.cpp\
	netlib/server_threads.cpp\
	netlib/single.cpp\
	netlib/single_threads.cpp\
	netlib/zndNet.cpp\
	nlist.cpp\
	nucleas.cpp\
	particle.cpp\
	rsrc.cpp\
	sample.cpp\
	skeleton.cpp\
	sklt.cpp\
	utils.cpp\
	wav.cpp\
	win3d.cpp\
	windp.cpp\
	winp.cpp\
	wintimer.cpp\
	wrapal.cpp\
	wrapSDL.cpp\
	yw.cpp\
	yw_brif.cpp\
	yw_funcs.cpp\
	yw_func2.cpp\
	yw_game.cpp\
	yw_game_ui.cpp\
	yw_net.cpp\
	yw_protos.cpp\
	yw_save.cpp\
	ypabact.cpp\
	ypacar.cpp\
	ypaflyer.cpp\
	ypagun.cpp\
	ypamissile.cpp\
	yparobo.cpp\
	ypatank.cpp\
	ypaufo.cpp
	
WINOBJ = \
	win_funct.cpp

win32: $(OBJS)
	$(CXX_WIN) -std=c++11 -DWIN32 -mno-ms-bitfields -O0 -g -o $(TARGET).exe $(OBJS) $(LIBS_WIN) -I $(MINGW)/include/

nix: $(OBJS)
	$(CXX) -std=c++11 -mno-ms-bitfields -O0 -g -o $(TARGET) $(OBJS) $(LIBS_NIX)

mac: $(OBJS)
	$(CXX_MAC) -std=c++11 -mno-ms-bitfields -O0 -g -o $(TARGET) $(OBJS) $(LIBS_MAC)

wclang: $(OBJS)
	$(CLANG_WIN) -mno-ms-bitfields -O0 -g -o $(TARGET).exe $(OBJS) $(LIBS_WIN) -m32 -I $(MINGW)/include/

clean:
	rm -f $(TARGET) $(TARGET_DINGOO) *.o
