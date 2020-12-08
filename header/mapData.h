#ifndef __MAPDATA_H
#define __MAPDATA_H
/* library */
#include "dictionary.h"
#include <vector>
#include <map>
/**
 * @brief Global structure with all level related data
 * 
 */
struct MapData {
    /* data */
    int gridWidth = 200, gridHeight = 200;
    std::map<std::pair<int, int>, std::vector<std::vector<float>>> gridElement;
};
#endif
