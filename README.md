# UA_source
![icon image](https://github.com/Marisa-Chan/UA_source/blob/master/svg/icon.svg?raw=true)

Opensource reimplementation of **UrbanAssault** engine. You needed copy of original game files for play.

**License** GPLv2

<a href="https://scan.coverity.com/projects/marisa-chan-ua_source">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/8507/badge.svg"/>
</a>


**Dependings**: SDL2, SDL2_ttf, OpenGL, OpenAL, vorbisfile(ogg), ffmpeg


**Building**:
- linux native: **make nix**
- windows msys compile: **make msys**

[**Wiki page with instructions**](https://github.com/Marisa-Chan/UA_source/wiki)


**Fonts** directory contains:
- Liberation Mono, Liberation Sans, Liberation Serif.  Version 2.00.1
- Xolonium
- Press Start 2P
- Textar


**Licenses of used fonts**:
- The Liberation(tm) version 2.00.0 onward are Licensed under the SIL Open Font License, Version 1.1.
- Xolonium by Severin Meyer. License: SIL Open Font License (OFL)
- Press Start 2P by Cody 'CodeMan38' Boisclair. License: SIL Open Font License (OFL)
- Textar (https://github.com/yamacraft/textar-font) License: IPA-1


**Additional keys (nucleus.ini)**:
- gfx.blending   0(default)/1/2 - blending mode for transparent things. 0 - default multiply, 1 - additive, 2 - sharp edged
- gfx.solidfont   on/off(default) - enable(on) pointy font rasterizing without of pixel blending
- gfx.vsync 0/1(default)/2 - vertical sync type. 0 - disable, 1 - default sync, 2 - adaptive(if you've already missed the vertical retrace for a given frame, it swaps buffers immediately)
- gfx.maxfps 60(default) - maximal fps. Strongly recommended do not up it more than 60 - currently it will break helicopters rotations and AI
- gfx.newsky on/off(default) - enable new fade effect of far grounds
- gfx.skydistance 3000(default) - start distance of sky fade effect (do not rise it too strong, because of limits of sky sphere)
- gfx.skylength 500(default) - length of sky fade effect
- gfx.color_effects 0(default)/1/2 - emulate color effects like in original game in software rendering. 0 - disable, 1 - affects all, 2 - do not affect GUI
- gfx.color_eff_pwr[1] 0-100(default) - power of red color effect
- gfx.color_eff_pwr[2] 0-100(default) - power of blue color effect
- gfx.color_eff_pwr[3] 0-100(default) - power of green color effect
- gfx.color_eff_pwr[4] 0-100(default) - power of inverse color effect
- gfx.color_eff_pwr[5] 0-100(default) - power of invdark color effect
- gfx.color_eff_pwr[6] 0-100(default) - power of sw color effect
- gfx.color_eff_pwr[7] 0-100(default) - power of invtuerk color effect
- particles.limit 5000(default) - limit amount of particles 
- menu.windowed on/off(default) - always open game menu in windowed mode
- gfx.vbo on(default)/off - disable modern VBO(upload vertex data into video card)



- world.ini : begin_misc : fix_weapon_radius - on/off(default) - enable(on) fix of original bug that prevent working of different weapon radius
- world.ini : begin_misc : hidden_fractions - mask for hidden fractions for whole world
- levelfile : begin_level : hidden_fractions - mask for hidden fractions for this level
- vehicle : hidden (yes/no) - hide this unit type
- vehicle : unhide_radar (0 disable) - unhide number of sectors

