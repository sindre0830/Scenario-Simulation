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
    //move sun
    if(hour <= 4) {
        g_mapData->lightDirection = glm::vec3(1.04f, 1.f, 0.f);
    } else g_mapData->lightDirection = glm::vec3(((float)(hour) / 24.f) * hour, 1.f, 0.f);
    //set color to the background according to the time
    if(hour <= 6) {
        //set night color
        g_mapData->lightColor = glm::vec3(0.5f, 0.5f, 0.5f);
    } else if(hour <= 10) {
        //set dusk/dawn color
        g_mapData->lightColor = glm::vec3(1.f, 0.9f, 0.65f);
    } else {
        //set day color
        g_mapData->lightColor = glm::vec3(1.f, 1.f, 1.f);
    }
    g_mapData->skyColor = glm::vec3(0.01f, 0.01f, (float)(hour) / 12.f);
    //either increment or decrement according to cycle
    if(increment) {
        hour++;
        if(hour == 12) increment = false;
    } else {
        hour--;
        if(hour == 0) increment = true;
    }
}
