CDEFS = 
CFLAGS = -std=c++11 -O0 -g -Wall -fexceptions -mno-ms-bitfields -Wno-class-memaccess -Wshadow -I./
TARGET = UA_source
LDFLAGS = 
OBJDIR := obj

nix: CXX = g++
nix: CC = gcc
nix: LIBS = -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_net -lGL -lopenal -lvorbisfile -lavformat -lavcodec -lavutil -lswscale -lswresample -llua5.3

win32: MINGW = /usr/i686-w64-mingw32
win32: CXX = i686-w64-mingw32-g++
win32: CC = i686-w64-mingw32-gcc
win32: CDEFS = -DWIN32
win32: TARGET := $(TARGET).exe
win32: LIBS = -lmingw32 -mwindows -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_net -lOpenAL32 -lopengl32 -lvorbisfile -lavformat -lavcodec -lavutil -lswscale -lswresample -llua5.3
win32: LDFLAGS = -L$(MINGW)/lib/
win32: CFLAGS := $(CFLAGS) -I$(MINGW)/include/

msys: MINGW = /usr/i686-w64-mingw32
msys: CXX = i686-w64-mingw32-g++
msys: CC = i686-w64-mingw32-gcc
msys: CDEFS = -DWIN32 -DNOLUAVERSION
msys: TARGET := $(TARGET).exe
msys: LIBS = -lmingw32 -mwindows -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_net -lopenal -lopengl32 -lvorbisfile -lavformat -lavcodec -lavutil -lswscale -lswresample -llua
msys: LDFLAGS = -L$(MINGW)/lib/
msys: CFLAGS := $(CFLAGS) -I$(MINGW)/include/

mac: CXX = clang++
mac: CC = clang
mac: LIBS = -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_net -framework OpenGL -framework OpenAL -lvorbisfile -lavformat -lavcodec -lavutil -lswscale -lswresample -llua5.3

OBJS =  \
	3ds.cpp\
	ade.cpp\
	area.cpp\
	amesh.cpp\
	bitmap.cpp\
	base.cpp\
	bmpAnm.cpp\
	button.cpp\
	common.cpp\
	crc32.cpp\
	def_parser.cpp\
	embed.cpp\
	env.cpp\
	fmtlib/format.cc\
	font.cpp\
	glob_funcs.cpp\
	gui/root.cpp\
	gui/uabutton.cpp\
	gui/uacommon.cpp\
	gui/uaempty.cpp\
	gui/uamsgbox.cpp\
	gui/uascroll.cpp\
	gui/uawindow.cpp\
	gui/widget.cpp\
	idev.cpp\
	ilbm.cpp\
	image.cpp\
	ini.cpp\
	input.cpp\
	itimer.cpp\
	iwimp.cpp\
	log.cpp\
	loaders.cpp\
	lstvw.cpp\
	main.cpp\
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
	nucleas.cpp\
	obj3d.cpp\
	particle.cpp\
	rsrc.cpp\
	sample.cpp\
	skeleton.cpp\
	sklt.cpp\
	system/fsmgr.cpp\
	system/gfx.cpp\
	system/glfuncs.cpp\
	system/inivals.cpp\
	system/inpt.cpp\
	system/luascript.cpp\
	system/movie.cpp\
	system/sound.cpp\
	system/stdshdr.cpp\
	system/system.cpp\
	system/tform.cpp\
	utils.cpp\
	wav.cpp\
	windp.cpp\
	winp.cpp\
	wintimer.cpp\
	world/analyzer.cpp\
	world/luaevents.cpp\
	world/parsers.cpp\
	world/nparticle.cpp\
	world/protos.cpp\
	world/saveparsers.cpp\
	world/tools.cpp\
	wrapal.cpp\
	yw.cpp\
	yw_brif.cpp\
	yw_funcs.cpp\
	yw_func2.cpp\
	yw_game.cpp\
	yw_game_ui.cpp\
	yw_net.cpp\
	yw_save.cpp\
	ypabact.cpp\
	ypacar.cpp\
	ypaflyer.cpp\
	ypagun.cpp\
	ypamissile.cpp\
	yparobo.cpp\
	ypatank.cpp\
	ypaufo.cpp

CCFILES     := $(filter %.cc, $(OBJS))
CPPFILES    := $(filter %.cpp, $(OBJS))
CCOBJFILES  := $(addprefix $(OBJDIR)/,$(CCFILES:.cc=.o))
CPPOBJFILES := $(addprefix $(OBJDIR)/,$(CPPFILES:.cpp=.o))
OBJFILES    := $(CCOBJFILES) $(CPPOBJFILES)

CCDEPFILES  := $(addprefix $(OBJDIR)/,$(CCFILES:.cc=.d))
CPPDEPFILES := $(addprefix $(OBJDIR)/,$(CPPFILES:.cpp=.d))
DEPFILES    := $(CCDEPFILES) $(CPPDEPFILES)

$(CCOBJFILES): $(OBJDIR)/%.o: %.cc
$(CPPOBJFILES): $(OBJDIR)/%.o: %.cpp
$(OBJFILES):
	mkdir -p $(@D)
	$(CXX) $(CFLAGS) $(CDEFS) -c $< -o $@

$(CCDEPFILES): $(OBJDIR)/%.d: %.cc
$(CPPDEPFILES): $(OBJDIR)/%.d: %.cpp

$(DEPFILES):
	mkdir -p $(@D)
	printf "$(OBJDIR)/" > $@
	$(CXX) $(CFLAGS) $(CDEFS) -c $< -MM >> $@

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPFILES)
endif


win32 nix mac msys: $(OBJFILES)
	$(CXX) $(OBJFILES) $(LDFLAGS) $(LIBS) -o $(TARGET)

clean:
	rm -f $(OBJFILES) $(DEPFILES)
