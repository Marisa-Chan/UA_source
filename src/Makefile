CDEFS = 
CFLAGS = -std=c++11 -O0 -g -Wall -fexceptions -mno-ms-bitfields -Wno-class-memaccess -Wshadow
TARGET = UA_source
LDFLAGS = 
OBJDIR := obj

nix: CXX = g++
nix: CC = gcc
nix: LIBS = -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_net -lGL -lopenal -lvorbisfile -lavformat -lavcodec -lavutil -lswscale -lswresample

win32: MINGW = /usr/i686-w64-mingw32
win32: CXX = i686-w64-mingw32-g++
win32: CC = i686-w64-mingw32-gcc
win32: CDEFS = -DWIN32
win32: TARGET := $(TARGET).exe
win32: LIBS = -lmingw32 -mwindows -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_net -lOpenAL32 -lopengl32 -lvorbisfile -lavformat -lavcodec -lavutil -lswscale -lswresample
win32: LDFLAGS = -L$(MINGW)/lib/
win32: CFLAGS := $(CFLAGS) -I$(MINGW)/include/

msys: MINGW = /usr/i686-w64-mingw32
msys: CXX = i686-w64-mingw32-g++
msys: CC = i686-w64-mingw32-gcc
msys: CDEFS = -DWIN32
msys: TARGET := $(TARGET).exe
msys: LIBS = -lmingw32 -mwindows -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_net -lopenal -lopengl32 -lvorbisfile -lavformat -lavcodec -lavutil -lswscale -lswresample
msys: LDFLAGS = -L$(MINGW)/lib/
msys: CFLAGS := $(CFLAGS) -I$(MINGW)/include/

mac: CXX = clang++
mac: CC = clang
mac: LIBS = -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_net -framework OpenGL -framework OpenAL -lvorbisfile -lavformat -lavcodec -lavutil -lswscale -lswresample

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
	gui/oldcompat.cpp\
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
	particle.cpp\
	rsrc.cpp\
	sample.cpp\
	skeleton.cpp\
	sklt.cpp\
	system/fsmgr.cpp\
	system/gfx.cpp\
	system/inivals.cpp\
	system/inpt.cpp\
	system/movie.cpp\
	system/sound.cpp\
	system/system.cpp\
	system/tform.cpp\
	utils.cpp\
	wav.cpp\
	windp.cpp\
	winp.cpp\
	wintimer.cpp\
	world/analyzer.cpp\
	world/parsers.cpp\
	world/nparticle.cpp\
	world/saveparsers.cpp\
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

win32 msys: OBJS := $(OBJS) win_funct.cpp

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
	echo -n "$(OBJDIR)/" > $@
	$(CXX) $(CFLAGS) $(CDEFS) -c $< -MM >> $@

-include $(DEPFILES)


win32 nix mac msys: $(OBJFILES)
	$(CXX) $(OBJFILES) $(LDFLAGS) $(LIBS) -o $(TARGET)

clean:
	rm -f $(OBJFILES) $(DEPFILES)
