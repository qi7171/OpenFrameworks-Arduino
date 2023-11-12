#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include <vector>
#include "boids.h"

#define BOIDS 100

class testApp : public ofBaseApp{
private:
       boid *boids[BOIDS];

   public:
       void setup();
       void update();
       void draw();

       void keyPressed  (int key);
};

#endif	
