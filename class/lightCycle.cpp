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
    if(hour <= 4) {
        g_mapData->lightDirection = glm::vec3(1.04f, 1.f, 0.f);
    } else g_mapData->lightDirection = glm::vec3(((float)(hour) / 24.f) * hour, 1.f, 0.f);
    //set color to the background according to the time
    if(hour <= 5) {
        //set night color
        g_mapData->skyColor = glm::vec3(0.01f, 0.01f, 0.14f);
        g_mapData->lightColor = glm::vec3(1.f, 1.f, 1.f);
    } else if(hour <= 7) {
        //set mild night color
        g_mapData->skyColor = glm::vec3(0.01f, 0.01f, 0.44f);
        g_mapData->lightColor = glm::vec3(1.f, 0.9f, 0.65f);
    } else if(hour <= 10) {
        //set morning/evening color
        g_mapData->skyColor = glm::vec3(0.f, 0.75f, 1.f);
        g_mapData->lightColor = glm::vec3(1.f, 0.9f, 0.65f);
    } else {
        //set day color
        g_mapData->skyColor = glm::vec3(0.7f, 1.0f, 1.0f);
        g_mapData->lightColor = glm::vec3(1.f, 1.f, 1.f);
    }
    //either increment or decrement according to cycle
    if(increment) {
        hour++;
        if(hour == 12) increment = false;
    } else {
        hour--;
        if(hour == 0) increment = true;
    }
}
