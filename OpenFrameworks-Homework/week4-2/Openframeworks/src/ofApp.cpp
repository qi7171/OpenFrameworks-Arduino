#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofBackground(0);
    ofSetLineWidth(1.5);
    ofSetLogLevel(OF_LOG_VERBOSE);
    serial.listDevices();
    vector<ofSerialDeviceInfo>deviceList =  serial.getDeviceList();
    
    int baud =9600;
    serial.setup("/dev/cu.usbmodem1101", baud);
    
    a= 8;
    b= 16;
    c= 16;
    
    amp = 100;
    xCenter = ofGetWidth()/2;
    yCenter = ofGetHeight()/2;

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    if(serial.available()<0){
        cout<< "ERROR ARDUINO" << endl;
    } else{
        while (serial.available() > 0) {
            //byte data is being writen into byteData as int.
            byteData = serial.readByte();
        
            //byteData is converted into a string for drawing later.
        //    sensorValue = "value: " + ofToString(byteData);
        }
    }
   // cout << sensorValue << endl; // output the sensorValue
    cout << "sensorvaluevalue: " + ofToString(sensorValue);
    
    ofSetColor(177,156,217);
    //ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255));
    ofNoFill();
    ofTranslate(xCenter,yCenter,0);
    ofBeginShape();
    for(float i=0.0; i<=TWO_PI + 0.001; i+= 0.01){
        auto theta = i + (sin( b*i)/c);
        auto r = amp * (2.0 + byteData * sin(a *i));
        auto x = r * cos(theta);
        auto y = r * sin(theta);
        ofVertex(x,y);
        ofDrawLine(x * 0.2, y * 0.2,x,y);
    }
    ofEndShape();
}




//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
