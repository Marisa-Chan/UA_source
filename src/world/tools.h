#ifndef WORLD_TOOLS_H
#define WORLD_TOOLS_H

#include "../common.h"
#include "../vectors.h"
#include "consts.h"

namespace World {

inline Common::Point PositionToSectorID(const vec3d &pos) 
{ return Common::Point(pos.x / CVSectorLength, -pos.z / CVSectorLength); };

inline Common::Point PositionToSectorID(const vec2d &pos)
{ return Common::Point(pos.x / CVSectorLength, -pos.y / CVSectorLength); };

inline Common::Point PositionToSectorID(float x, float z)
{ return Common::Point(x / CVSectorLength, -z / CVSectorLength); };


inline vec3d SectorIDToPos3(const Common::Point &sec)
{ return vec3d((float)sec.x * CVSectorLength, 0.0, -((float)sec.y * CVSectorLength)); };

inline vec2d SectorIDToPos2(const Common::Point &sec)
{ return vec2d((float)sec.x * CVSectorLength, -((float)sec.y * CVSectorLength)); };


inline vec3d SectorIDToCenterPos3(const Common::Point &sec)
{ return vec3d((float)sec.x * CVSectorLength + CVSectorHalfLength, 0.0, -((float)sec.y * CVSectorLength + CVSectorHalfLength)); };

inline vec2d SectorIDToCenterPos2(const Common::Point &sec)
{ return vec2d((float)sec.x * CVSectorLength + CVSectorHalfLength, -((float)sec.y * CVSectorLength + CVSectorHalfLength)); };


}

#endif