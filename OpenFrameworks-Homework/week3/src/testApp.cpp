#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
  
    for( int i=0; i < BOIDS; i++) {
        boids[i] = new boid( ofRandom(0,ofGetWidth()), ofRandom(0,ofGetHeight()), ofRandom(-10,10), ofRandom(-10,10), boids, BOIDS );
    }
    ofSetFrameRate(25);
}

//--------------------------------------------------------------
void testApp::update(){
    for( int i=0; i < BOIDS; i++) {
        boids[i]->update();
    }

}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    for( int i=0; i < BOIDS; i++) {
        boids[i]->draw();
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    for( int i=0; i < BOIDS; i++) {
        boids[i]->boo();
    }

}
