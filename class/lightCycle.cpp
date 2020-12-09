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
    update(0.f);
}

void LightCycle::update(const double deltaTime) {
    //move sun
    g_mapData->lightDirection = glm::vec3(24.f / (float)(hour), 1.f, 0.f);
    //set the correct color to the background according to the time
    if(hour >= -1 && hour <= 1) {
        //set night color
        g_mapData->skyColor = glm::vec3(0.01f, 0.01f, 0.14f);
    } else if(hour >= -3 && hour <= 3) {
        //set night color
        g_mapData->skyColor = glm::vec3(0.01f, 0.01f, 0.44f);
    } else if(hour >= -6 && hour <= 6) {
        //set morning/evening color
        g_mapData->skyColor = glm::vec3(0.f, 0.75f, 1.f);
    } else {
        //set day color
        g_mapData->skyColor = glm::vec3(0.7f, 1.0f, 1.0f);
    }
    //either increment or decrement according to cycle
    if(increment) {
        hour++;
        if(hour == 12) increment = false;
    } else {
        hour--;
        if(hour == -12) increment = true;
    }
}
