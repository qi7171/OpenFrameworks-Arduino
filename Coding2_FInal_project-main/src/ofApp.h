#pragma once

#include "ofMain.h"
#include "ofxGui.h"



class box{
public:
    int width;
    int height;
    int x;
    int y;
    ofColor color;
    void draw();
    void wobble();
    box();
    ~box();
  //  ofImage texture;
    
};


class particle{
public:
    ofColor color;
    float size;
    glm::vec2 force, position, direction;
    
    void update();
    void draw();
    
    particle(int x, int y, int hue);
    ~particle();
};



class ofApp : public ofBaseApp{
    
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
		ofxPanel gui;
		ofParameterGroup paramGroupTransformFeedback;
		ofParameter<float> paramMaxLife;
		ofParameter<float> paramNoiseStrength;
		ofParameter<float> paramUpwardForce;
		ofParameter<float> paramNoiseTimeOffsetMultiplier;
	
		ofParameterGroup paramGroupRender;
		ofParameter<float> paramPointSize;
		ofParameter<ofFloatColor> paramRenderStartColor1;
		ofParameter<ofFloatColor> paramRenderStartColor2;
		ofParameter<ofFloatColor> paramRenderEndColor;

		vector< shared_ptr<ofVbo> > vbos;
		vector<ofShader::TransformFeedbackBaseBinding> transformFeedbackVertexBindings;
		vector<ofShader::TransformFeedbackBaseBinding> transformFeedbackNormalBindings;
		ofEasyCam cam;
		ofShader shaderTransformFeedback;
		ofShader shaderRender;
		int bufferIndex = 0;
		int numVertices = 1381;
		ofTexture dotTexture;
   
    //sound
    ofSoundPlayer  sound2;
    ofSoundPlayer  sound1;
    ofSoundPlayer  sound3;

    ofTrueTypeFont    font;
    float             synthPosition;
    
    //image import
    ofImage but;
    ofImage but2;
    ofImage but3;
    
   //particles
    vector<box> boxes;
    vector<particle> particles;
    int hue;
    
    
    int a;
    int b;
    int c;
    int amp;
    
    float xCenter;
    float yCenter;
    
    ofSerial serial;
    int sensorValue;
    int byteData;
    
 
    string serialMessage;
    string msg;
  
};
