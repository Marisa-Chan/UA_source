#ifndef WORLD_CONSTS_H
#define WORLD_CONSTS_H

namespace World {
// Constant values
constexpr float CVSectorLength = 1200.0;
constexpr float CVSectorHalfLength = CVSectorLength / 2.0;
constexpr size_t CVLocaleStringsNumber = 2700;
constexpr size_t CVFractionsCount = 8;
constexpr size_t CVMaxMap = 256;
constexpr size_t CVMaxNetPlayers = 4;

/* How long wait(ms) for start demo player*/
constexpr size_t CVDemoWait = 200 * 1000;

/* How long will construct buildings */
constexpr size_t CVBuildConstructTime = 3000;

/* Target unit too far from sector target distance*/
constexpr float CVUnitFarSecDist = 3600.0;
}

#endif