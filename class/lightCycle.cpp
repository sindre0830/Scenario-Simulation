/* library */
#include "header/lightCycle.h"
#include "dictionary.h"
#include "camera.h"
#include "mapData.h"
#include "functionality.h"
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <map>
/* global data */
extern Camera *g_camera;
extern MapData* g_mapData;

LightCycle::~LightCycle() {}

LightCycle::LightCycle() {
    update();
}

void LightCycle::update() {
    if(hour == 24) hour = 1;

    g_mapData->lightDirection = glm::vec3(0.f, 3.f, 0.f);
    g_mapData->skyColor = glm::vec3(0.7f, 1.0f, 1.0f);

    hour++;
}
