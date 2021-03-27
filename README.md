# UA_source
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
- tune.weapon_radius   on/off(default) - enable(on) fix of original bug that prevent working of different weapon radius
- gfx.blending   0(default)/1/2 - blending mode for transparent things. 0 - default multiply, 1 - additive, 2 - sharp edged
- gfx.solidfont   on/off(default) - enable(on) pointy font rasterizing without of pixel blending
- gfx.vsync 0/1(default)/2 - vertical sync type. 0 - disable, 1 - default sync, 2 - adaptive(if you've already missed the vertical retrace for a given frame, it swaps buffers immediately)
- gfx.maxfps 60(default) - maximal fps. Strongly recommended do not up it more than 60 - currently it will break helicopters rotations and AI
- gfx.newsky on/off(default) - enable new fade effect of far grounds
- gfx.skydistance 3000(default) - start distance of sky fade effect (do not rise it too strong, because of limits of sky sphere)
- gfx.skylength 500(default) - length of sky fade effect
- gfx.color_effects 0(default)/1/2 - emulate color effects like in original game in software rendering. 0 - disable, 1 - affects all, 2 - do not affect GUI

