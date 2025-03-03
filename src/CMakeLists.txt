cmake_minimum_required(VERSION 3.5)
project(UA_source)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 14)

set(SOURCES 
	3ds.cpp
	ade.cpp
	area.cpp
	amesh.cpp
	bitmap.cpp
	base.cpp
	bmpAnm.cpp
	button.cpp
	crc32.cpp
	def_parser.cpp
	embed.cpp
	env.cpp
	fmtlib/format.cc
	font.cpp
	gui/root.cpp
	gui/uabutton.cpp
	gui/uacommon.cpp
	gui/uaempty.cpp
	gui/uamsgbox.cpp
	gui/uascroll.cpp
	gui/uawindow.cpp
	gui/widget.cpp
	idev.cpp
	ilbm.cpp
	image.cpp
	ini.cpp
	itimer.cpp
	iwimp.cpp
	log.cpp
	loaders.cpp
	locale/default.cpp
	locale/locale.cpp
	lstvw.cpp
	main.cpp
	IFFile.cpp
	network.cpp
	netlib/client.cpp
	netlib/client_threads.cpp
	netlib/server.cpp
	netlib/server_session.cpp
	netlib/server_threads.cpp
	netlib/single.cpp
	netlib/single_threads.cpp
	netlib/zndNet.cpp
	nucleas.cpp
	obj3d.cpp
	particle.cpp
	rsrc.cpp
	sample.cpp
	skeleton.cpp
	sklt.cpp
	system/fsmgr.cpp
	system/gfx.cpp
	system/glfuncs.cpp
	system/inivals.cpp
	system/inpt.cpp
	system/luascript.cpp
	system/movie.cpp
	system/sound.cpp
	system/stdshdr.cpp
	system/system.cpp
	system/tform.cpp
	utils.cpp
	wav.cpp
	windp.cpp
	winp.cpp
	wintimer.cpp
	world/analyzer.cpp
	world/luaevents.cpp
	world/parsers.cpp
	world/nparticle.cpp
	world/protos.cpp
	world/saveparsers.cpp
	world/tools.cpp
	wrapal.cpp
	yw.cpp
	yw_brif.cpp
	yw_funcs.cpp
	yw_func2.cpp
	yw_game.cpp
	yw_game_ui.cpp
	yw_net.cpp
	yw_save.cpp
	ypabact.cpp
	ypacar.cpp
	ypaflyer.cpp
	ypagun.cpp
	ypamissile.cpp
	yparobo.cpp
	ypatank.cpp
	ypaufo.cpp
)


find_package(PkgConfig REQUIRED)

pkg_search_module(SDL2 REQUIRED sdl2)
link_directories(${SDL2_LIBRARY_DIRS})

pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image)
link_directories(${SDL2_IMAGE_LIBRARY_DIRS})

pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)
link_directories(${SDL2_TTF_LIBRARY_DIRS})

pkg_search_module(SDL2_NET REQUIRED SDL2_net)
link_directories(${SDL2_NET_LIBRARY_DIRS})

pkg_search_module(VORBISFILE REQUIRED vorbisfile)
link_directories(${VORBISFILE_LIBRARY_DIRS})

#find_package(FFmpeg REQUIRED avformat avcodec avutil swscale swresample OPTIONAL avresample)
find_package(FFmpeg REQUIRED)

find_package(OpenGL REQUIRED)

find_package(OpenAL REQUIRED)

find_package(Lua REQUIRED)


if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    enable_language("RC")
    set (WIN32_RESOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../svg/icon/icon.rc")
endif()


if (WIN32)
  add_executable(UA_source WIN32
                       ${SOURCES}
                       ${WIN32_RESOURCES}
                )
else()
  add_executable(UA_source ${SOURCES})
endif()




target_link_libraries(UA_source ${SDL2_LIBRARIES}
							 ${SDL2_IMAGE_LIBRARIES}
							 ${SDL2_TTF_LIBRARIES}
							 ${SDL2_NET_LIBRARIES}
							 ${OPENGL_LIBRARIES}
							 ${VORBISFILE_LIBRARIES}
							 ${OPENAL_LIBRARY}
							 ${LUA_LIBRARIES}
							 FFmpeg::avcodec
							 FFmpeg::avformat
							 FFmpeg::avutil
							 FFmpeg::swscale
							 FFmpeg::swresample)

target_include_directories(UA_source PUBLIC
										${PROJECT_SOURCE_DIR} 
										${SDL2_INCLUDE_DIRS} 
										${SDL2_IMAGE_INCLUDE_DIRS} 
										${SDL2_TTF_INCLUDE_DIRS}
										${SDL2_NET_INCLUDE_DIRS}
										${OpenGL_INCLUDE_DIR}
										${VORBISFILE_INCLUDE_DIRS}
										${OPENAL_INCLUDE_DIR}
										${LUA_INCLUDE_DIR})

if(CMAKE_BUILD_TYPE MATCHES [dD][eE][bB][uU][gG])
   target_compile_options(UA_source PUBLIC "-g" "-O0")
   
   if(NOT WIN32)
      target_compile_options(UA_source PUBLIC "-fsanitize=address")
      target_link_libraries(UA_source "-fsanitize=address")
   endif()
else()
	target_compile_options(UA_source PUBLIC "-g" "-O0")
	#target_compile_options(UA_source PUBLIC "-O2" "-s")
endif()

