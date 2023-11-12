/*

 *
 */
#ifndef _BOID
#define _BOID

#include <vector>
#include "ofMain.h"

class boid {
    private:
      float r1;
      float r2;
      float r3;

      float x,y;
      float vx,vy;
      boid** others;
      int num_others;

    public:
        boid( float x, float y, float vx, float vy, boid* others[], int num_others );
        void boo();
        void update();
        void draw();

        void sep();
        void align();
        void coh();
};


#endif
