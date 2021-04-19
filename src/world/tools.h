#ifndef WORLD_TOOLS_H
#define WORLD_TOOLS_H

#include "../common.h"
#include "../vectors.h"
#include "consts.h"

namespace World {

inline Common::Point PositionToSectorID(const vec3d &pos) 
{ return Common::Point(pos.x / SectorLength, -pos.z / SectorLength); };

inline Common::Point PositionToSectorID(const vec2d &pos)
{ return Common::Point(pos.x / SectorLength, -pos.y / SectorLength); };

inline Common::Point PositionToSectorID(float x, float z)
{ return Common::Point(x / SectorLength, -z / SectorLength); };


inline vec3d SectorIDToPos3(const Common::Point &sec)
{ return vec3d((float)sec.x * SectorLength, 0.0, -((float)sec.y * SectorLength)); };

inline vec2d SectorIDToPos2(const Common::Point &sec)
{ return vec2d((float)sec.x * SectorLength, -((float)sec.y * SectorLength)); };


inline vec3d SectorIDToCenterPos3(const Common::Point &sec)
{ return vec3d((float)sec.x * SectorLength + SectorHalfLength, 0.0, -((float)sec.y * SectorLength + SectorHalfLength)); };

inline vec2d SectorIDToCenterPos2(const Common::Point &sec)
{ return vec2d((float)sec.x * SectorLength + SectorHalfLength, -((float)sec.y * SectorLength + SectorHalfLength)); };


}

#endif