#ifndef __LIGHTCYCLE_H
#define __LIGHTCYCLE_H
/* library */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
/**
 * @brief Light cycle class.
 * 
 */
class LightCycle {
    private:
        /* private data */
        int hour = 1;
    public:
        /* public functionality */
        ~LightCycle();
        LightCycle();
        void update();
};
#endif
