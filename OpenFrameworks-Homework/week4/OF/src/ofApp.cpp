#include "ofApp.h"
int byteData;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofBackground(0,0,0);
    ofSetLogLevel(OF_LOG_VERBOSE);
    serial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
   

    int baud = 9600;
    
    // Connect to the serial device
    // Change this line depending on your platform and device
    serial.setup("/dev/cu.usbmodem101", baud);
    
    ofSetLineWidth(1.5);
    a= 13;
    b= 12;
    c= 3;
    
    amp = 100;
    xCenter = ofGetWidth()/2;
    yCenter = ofGetHeight()/2;
    
    
    serialMessage = false;
    //Initialize the drawing variables
    for (int i = 0; i < ofGetWidth(); ++i) {
        waveform[i] = 0;
    }
    waveIndex = 0;
    
    // Maximilian audio stuff
    int sampleRate = 44100; /* Sampling Rate */
    int bufferSize= 512; /* Buffer Size. you have to fill this buffer with sound using the for loop in the audioOut method */
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
    
    
    // Setup ofSound
    ofSoundStreamSettings settings;
    settings.setOutListener(this);
    settings.sampleRate = sampleRate;
    settings.numOutputChannels = 2;
    settings.numInputChannels = 0;
    settings.bufferSize = bufferSize;
    soundStream.setup(settings);
    

    

}

//--------------------------------------------------------------
void ofApp::update(){
  // Check if there is any serial data available
    if (serial.available() < 0) {
        serialMessage = "Arduino Error";
    }else{
        while(serial.available()>0){
            byteData = serial.readByte();
            
            serialMessage = ofToString(byteData);
        }
              serial.writeByte(sendData);
       // serial.readBytes(receivedData, 10); // Getting the data from Arduino
        printf("receivedData is %d \n", receivedData);  
        
     //   cout << serialMessage << endl;
    }
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
   
    ofSetColor(162,25,255);
    ofNoFill();
    ofTranslate(xCenter,yCenter/1.5,0);
    ofTranslate(0, ofGetHeight()/8);
    ofBeginShape();
    for(float i=0.0; i<=TWO_PI + 0.001; i+= 0.01){
        auto theta = i + (sin( b*i)/c);
        auto r = amp * (2.0 + byteData * sin(a *i));
        auto x = r * cos(theta);
        auto y = r * sin(theta);
        ofVertex(x,y);
        ofDrawLine(x * 0.2, y * 0.2,x,y);
     
    }
    ofSetColor(0, 255, 0);
    ofDrawLine(0, 0, 1, waveform[1] * ofGetHeight()/2.); //first line
    for(int i = 1; i < (ofGetWidth() - 1); ++i) {
        ofDrawLine(i, waveform[i] * ofGetHeight()/4, i + 1, waveform[i] * ofGetHeight()/2.);
    }
    ofEndShape();
    
    
   /*   /////////////// waveform
   
    ofTranslate(0, ofGetHeight()/2);
    ofSetColor(0, 255, 0);
    ofFill();
    ofDrawLine(0, 0, 1, waveform[1] * ofGetHeight()/2.); //first line
    for(int i = 1; i < (ofGetWidth() - 1); ++i) {
        ofDrawLine(i, waveform[i] * ofGetHeight()/2., i + 1, waveform[i] * ofGetHeight()/2.);
    }
    */

    
}

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer& input){
    std::size_t nChannels = input.getNumChannels();
    for (size_t i = 0; i < input.getNumFrames(); i++)
    {
        // handle input here
    }
}
//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer& output){
    std::size_t outChannels = output.getNumChannels();
    if(serialMessage == "1"){
        for (int i = 0; i < output.getNumFrames(); ++i){
            
            output[i * outChannels] = oscillator1.sinewave(532) * 0.2;
            output[i * outChannels + 1] = output[i * outChannels];
            
            //Hold the values so the draw method can draw them
            waveform[waveIndex] =  output[i * outChannels];
            if (waveIndex < (ofGetWidth() - 1)) {
                ++waveIndex;
            } else {
                waveIndex = 0;
            }
    }
   
    }
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
