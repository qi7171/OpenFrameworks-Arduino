#include "ofApp.h"
int byteData;

//--------------------------------------------------------------
void ofApp::setup(){
    
    //arduino flower interaction-------------------------------------------
    ofSetVerticalSync(true);
    //it sets the width of the lines drawn by the application's  graphics context to 1.5 pixels.
    ofSetLineWidth(1.5);
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    serial.listDevices();
    //retrieve a list of available serial devices
    vector<ofSerialDeviceInfo>deviceList =  serial.getDeviceList();
    //arduino flower interaction-------------------------------------------

    
    //connect with arduino
    int baud =9600;
    serial.setup("/dev/cu.usbmodem1101", baud);
    
    
    //arduino flower interaction---geometric shape----------------------------------------
    a= 6;
    b= 18;
    c= 18;
    amp = 20;
    xCenter = ofGetWidth()/1.95;
    yCenter = ofGetHeight()/2.1;
    //arduino flower interaction---geometric shape---------------------------------------
    
    //load sound
    sound1.load("sounds/butterfly2.flac");
    sound2.load("sounds/butterfly.wav");
    sound3.load("sounds/wind.wav");
    sound1.setVolume(0.5f);
    sound2.setVolume(0.5f);
    sound3.setVolume(0.75f);
    sound2.setMultiPlay(false);
    sound3.setMultiPlay(true);
    
    //load sound image
    but.load("butterfly.webp");
    but2.load("butterfly.webp");
    but3.load("butterfly.webp");
    
    
    //shader--------------------------------------------------------------------
	ofShader::TransformFeedbackSettings settings;
	// vertex shader source file
	settings.shaderFiles[GL_VERTEX_SHADER] = "vert.glsl";
    //shader program should use the default uniform variable bindings provided by openFrameworks.
	settings.bindDefaults = true;
	// we define 2 outputs of the transform feedback objects
	settings.varyingsToCapture = { "TF_Position", "TF_Color" };
	settings.bufferMode = GL_SEPARATE_ATTRIBS;
	shaderTransformFeedback.setup(settings);
	
	// create some vectors to hold the initial data
	// we are using glm::vec4 because it gives us some more data to use in the shaders
	// we don't use a vector of ofFloatColor because it can clamp the value to its color range 0 -> 1
	vector<glm::vec4> colorData;
	// we are going to use the ofVbo.setVertexData function that expects glm::vec3
	vector<glm::vec3> positionData;
	
	ofPixels logoPixels;
	
    // This loads the "butterfly.png" image file and stores the pixel data in the logoPixels object. If the image is loaded successfully, the code inside the curly braces will execute.
	if( ofLoadImage(logoPixels, "butterfly.png")) {
		float pixWidth = logoPixels.getWidth();// width of the loaded image in pixels and stores it in a variable called pixWidth.
		float pixHeight = logoPixels.getHeight();// height of the loaded image in pixels and stores it in a variable called pixHeight.

		for( int ix = 0; ix < pixWidth; ix += 2 ) {
			for( int iy = 0; iy < pixHeight; iy += 2 ) {
				ofColor pixColor = logoPixels.getColor( ix, iy ); //This gets the color of the pixel at (ix, iy) in the logoPixels object and stores it in an ofColor object called pixColor.
                
                //  This checks if the red channel of the current pixel's color is greater than 200 (i.e., if it's a light color).
				if( pixColor.r > 200 ) {
                    
                    //creates a new 4D vector newColorData that stores the x, y, z, and w values of the particle's initial state. The x and y values are based on the current pixel's position, and the z and w values are randomly generated.
                    glm::vec4 newColorData = glm::vec4(ix-pixWidth*0.5,iy-pixHeight*0.5,0,ofRandom(0.5,1.5));
                   
                    //scale the x and y values of the particle's position by a factor of 2.
                    newColorData.x *= 2.f;
					newColorData.y *= 2.f;
					
                    //adds the particle's initial state to a vector called colorData
					colorData.push_back(newColorData);
					
                   // creates a new 3D vector position that stores the x, y, and z values of the particle's initial position. The y value is set to 0, and the x and z values are based on the particle's initial state.
					auto position = glm::vec3(newColorData.x, 0, newColorData.y);
					positionData.push_back(position);//adds the particle's initial position to a vector called positionData
				}
			}
		}
		numVertices = colorData.size();
	} else {
		// if there was a problem loading the image, then just assign default data
		colorData.assign( numVertices, glm::vec4(0,0,0,0.0) );
		positionData.assign( numVertices, glm::vec3(0,0,0) );
	}
	
	//The for loop is executed twice, creating two VBOs.
	for (int i = 0; i < 2; i++) {
		auto vbo = make_shared<ofVbo>();
        //The setVertexData function is called on the VBO object, which sets the vertex position data to 'positionData' using 'GL_DYNAMIC_COPY' as the usage hint, indicating that the data may change frequently.
		vbo->setVertexData( positionData.data(), numVertices, GL_DYNAMIC_COPY );
		//set the color data for the VBO
		vbo->setAttributeData( ofShader::COLOR_ATTRIBUTE, &colorData[0].x, 4, (int)colorData.size(), GL_DYNAMIC_COPY, sizeof(glm::vec4) );
		vbos.push_back( vbo );
	}
	
	// create 2 sets of transform feedback buffers to capture the ping pong
	for (int i = 0; i < 2; i++) {
		transformFeedbackVertexBindings.push_back(ofShader::TransformFeedbackBaseBinding(vbos[i]->getVertexBuffer()));
		transformFeedbackVertexBindings[i].index = ofShader::POSITION_ATTRIBUTE;
		
		transformFeedbackNormalBindings.push_back(ofShader::TransformFeedbackBaseBinding(vbos[i]->getColorBuffer()));
		transformFeedbackNormalBindings[i].index = ofShader::COLOR_ATTRIBUTE;
	}
	
	// load a shader to work with point sprites
	// shaderRender will read in the attributes from the vbo and use them for rendering
	shaderRender.load("pointSprite.vert", "pointSprite.frag");
	
	// load image to use for shaderRender
	// we call disableArbTex to be used with the point sprite texcoords that are mapped from 0 -> 1
	ofDisableArbTex();
	ofLoadImage(dotTexture, "butterfly.webp");
	ofEnableArbTex();
	
	gui.setup();
	// GUI-----------------------------------------------------------------
	paramGroupRender.setName("Design your butterfly");
	paramGroupRender.add(paramPointSize.set("uPointSize", 8, 2, 30)); //change the point size(butterfly)
	gui.add( paramGroupRender );
	gui.add( paramRenderStartColor1.set("uStartColor1", ofFloatColor(162/255.,25/255.,255/255.,1.0)));
	gui.add( paramRenderStartColor2.set("uStartColor2", ofFloatColor(255/255.,102/255.,204/255.,1.0)));
	gui.add( paramRenderEndColor.set("uEndColor", ofFloatColor(57/255.,255/255.,20/255.,1.0)));
	
	gui.loadFromFile("settings.xml");
	
	cam.setPosition(100, 3000, 1200);
	cam.lookAt( glm::vec3(0,0,0) );
    
    
 //particles
    hue=0; //particle hue
    
    //shaking boxes background-------------------------------------
    int maxBoxes;
    maxBoxes = 150;
    for(int i = 0; i< maxBoxes; i++){
        box newBox;
        boxes.push_back(newBox);
        
    }

}

//--------------------------------------------------------------
void ofApp::update(){
    
    // update the sound playing system:
    ofSoundUpdate();
    
   
    //butterfly
	float deltaTime = ofClamp( ofGetLastFrameTime(), 1.f/5000.f, 1.f/5.f );
	// create a vector of the active transform feedback buffers using 1-bufferIndex
	// we use 1-bufferIndex here so that it can output to the opposing vbos[bufferIndex]
	// these will be passed to shaderTransformFeedback
	vector<ofShader::TransformFeedbackBaseBinding> currentTransformFeedbackBindings = {
		transformFeedbackVertexBindings[1-bufferIndex],
		transformFeedbackNormalBindings[1-bufferIndex]
	};

	shaderTransformFeedback.beginTransformFeedback(GL_POINTS, currentTransformFeedbackBindings );
	shaderTransformFeedback.setUniform1i("uTotalParticles", numVertices );
	shaderTransformFeedback.setUniform1f("uElapsedTime", ofGetElapsedTimef() );
	shaderTransformFeedback.setUniform1f("uDeltaTime", deltaTime );
	shaderTransformFeedback.setUniforms(paramGroupTransformFeedback);
	vbos[bufferIndex]->draw(GL_POINTS, 0, numVertices);
	shaderTransformFeedback.endTransformFeedback(currentTransformFeedbackBindings);
    
    
    //boxes--------------------------------------------------------------
    for(int i=0; i< boxes.size(); i++){
        boxes[i].wobble();
      
    }
    //particles with mouse--------------------------------------------------------------
    for(int i=0; i<particles.size(); i++){
        particles[i].update();
    }
    
	
}

//--------------------------------------------------------------
void ofApp::draw(){
    
   //set background color of the screen to a gradient between two colors
	ofBackgroundGradient(ofColor(10), ofColor(50));
	
    //used to begin the camera view. The camera is set up so that the point of view is positioned at a fixed point in 3D space and looks towards the origin.
	cam.begin();
    
	ofSetColor( 150 );
    //rotate the coordinate system to make it easier to view the particles.
	ofPushMatrix();
	ofRotateYDeg(90);
	ofRotateZDeg(90);
	ofPopMatrix();
    
	ofEnablePointSprites();//enables point sprites, which are particles that are rendered using a single texture image instead of a complex 3D model.
	shaderRender.begin(); // starts the shaderRender shader program, which is responsible for rendering the point sprites.
	shaderRender.setUniformTexture("tex", dotTexture, 0);// binds the dotTexture image to the shader program.
	shaderRender.setUniform1f("uElapsedTime", ofGetElapsedTimef() );//sets a uniform variable in the shader program to the elapsed time since the program started running.
	shaderRender.setUniforms(paramGroupRender);//sets a group of uniform variables in the shader program that were defined elsewhere.
	//set three additional uniform variables in the shader program for the start and end colors of the particle system.
	shaderRender.setUniform4f( paramRenderStartColor1.getName(), paramRenderStartColor1.get() );
	shaderRender.setUniform4f( paramRenderStartColor2.getName(), paramRenderStartColor2.get() );
	shaderRender.setUniform4f( paramRenderEndColor.getName(), paramRenderEndColor.get() );
	
    //draws the particles on the screen using the vbos vertex buffer object.
	vbos[bufferIndex]->draw(GL_POINTS, 0, numVertices);
	shaderRender.end();
	ofDisablePointSprites();//disables point sprites
	
	cam.end(); //used to begin the camera view.
	bufferIndex = 1 - bufferIndex; //swaps the vertex buffer object indices to alternate between the two.
	
	ofSetColor(255);
	gui.draw();//draw the interface on the screen
    

    float widthDiv = ofGetWidth() / 3.0f;
    //---------------------------------- sound1:
    if (sound1.isPlaying()) ofSetHexColor(0x39ff14);
    else ofSetHexColor(0x7700c8);
    but.draw(60,630,70,70);//purple butterfly
    
    ofSetHexColor(0x7700c8);
    string tempStr = "click";
    ofDrawBitmapString(tempStr, 75,ofGetHeight()-50);


    //---------------------------------- sound2:
    if (sound2.isPlaying()) ofSetHexColor(0xff66cc);
    else ofSetHexColor(0x39ff14);
    but2.draw(widthDiv+150,630,70,70);//green butterfly

    ofSetHexColor(0x39ff14);
    tempStr = "Drag";
    ofDrawBitmapString(tempStr, widthDiv+168,ofGetHeight()-50);
    
    
    //---------------------------------- sound3:
    if (sound3.isPlaying()) ofSetHexColor(0x7700c8);
    else ofSetHexColor(0xff66cc);
    but3.draw(widthDiv*2+250,630,70,70);//pink butterfly
  

    ofSetHexColor(0xff66cc);
    tempStr = "click";
    ofDrawBitmapString(tempStr, widthDiv*2+268,ofGetHeight()-50);
   
    
    //particle
    //boxes--------------------------------------------------------------
    for(int i=0; i<boxes.size(); i++){
        boxes[i].draw();
    }
    
    //particle--------------------------------------------------------------
    for(int i=0; i<particles.size(); i++){
        particles[i].draw();
    }

    //arduino interaction flower-------------------------------------------------------
   //checking if there are any bytes available to read from the serial port, and if so, it reads the byte data into the byteData variable.
    if(serial.available()<0){
        cout<< "ERROR ARDUINO" << endl;
    } else{
        while (serial.available() > 0) {
            //byte data is being writen into byteData as int.
            byteData = serial.readByte();
        }
    }
   // cout << sensorValue << endl; // output the sensorValue
    cout << "sensorvaluevalue: " + ofToString(sensorValue);
    
    //drawing a shape based on calculations using the byteData variable. It sets the color, fills turned off, and translates the origin to the xCenter and yCenter variables. It then creates a closed shape with vertices and draws lines between the vertices
    ofSetColor(235,ofRandom(225),255,55);
    //ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255));
    ofNoFill();
    ofTranslate(xCenter,yCenter,0);
    ofBeginShape();
    for(float i=0.0; i<=TWO_PI + 0.001; i+= 0.01){
        auto theta = i + (sin( b*i)/c);
        auto r = amp * (2.0 + byteData * sin(a *i));
        auto x = r * cos(theta);
        auto y = r * sin(theta);
        ofVertex(x,y);// adds a new point to the shape
        ofDrawLine(x * 0.2, y * 0.2,x,y);//draws a line from the previous point to the current point.
    }
    ofEndShape();
    //-----------------------------------------------
    
    
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
    
    // "sound2" region:
    float widthStep = ofGetWidth() / 3.0f;
    //the particle effect appears when dragging the mouse:  adding a new particle to a vector of particles every time the user drags the mouse with a button pressed.

     particle newParticle(mouseX, mouseY,hue);
      particles.push_back(newParticle);

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    float widthStep = ofGetWidth() / 3.0f;//divide into 3 part
   // To check which part of the window the mouse was clicked in to interact with the butterfly. If it’s the purple butterfly, it plays "sound1" and sets its speed and panning based on the y and x coordinates of the mouse, respectively. The speed is set to a value between 0.1 and 1.0, with higher values for lower y values. The panning is set to a value between -1 and 1, with -1 being completely left and 1 being completely right, based on the mouse's x position within the left third of the window.
    if (x < widthStep){
        sound1.play();
        sound1.setSpeed( 0.1f + ((float)(ofGetHeight() - y) / (float)ofGetHeight())*10);
        sound1.setPan(ofMap(x, 0, widthStep, -1, 1, true));
    } else if (x >= widthStep && x < widthStep*2){
        sound2.play();
    } else {
        sound3.play();
        sound3.setSpeed( 0.1f + ((float)(ofGetHeight() - y) / (float)ofGetHeight())*3);
        //map x within the last third of window to -1 to 1 ( for left / right panning )
        sound3.setPan( ofMap(x, widthStep*2, widthStep*3, -1, 1, true) );
    }
    
    //particle--------------------------------------------------------------
    hue = ofRandom(255);

}



//--------------------------------------------------------------
box::box(){
    width = 6;
    height = 6;
    x = ofRandom(ofGetWidth());
    y = ofRandom(ofGetWidth());
    color.set(ofRandom(255), ofRandom(255), ofRandom(255));
  //  texture.load("butterfly.png");
}

box::~box(){
    
}

void box::draw(){
    ofSetColor(color);
    ofDrawRectangle(x, y, width, height);
   // texture.draw(50,50,100,100);
}

// randomly adjust the width and height of the box and change its color to a random RGB value.
void box::wobble(){
    width = ofRandom(5,15);
    height = ofRandom(5,15);
    color.set(ofRandom(255), ofRandom(255), ofRandom(255));
}



particle:: particle(int startX, int startY, int hue){
    position = glm::vec2(startX,startY);
    force = glm::vec2(0.01);
    direction=glm::vec2(ofRandom(-2.0,2.0), ofRandom(-2.0, 2.0));
    size= 6;
    color.set(ofRandom(219), ofRandom(205), ofRandom(255));
    
}

particle::~particle(){
    
}

void particle::update(){
    position += direction;
    direction += force;
    
    if(size>1){
        size-=0.07;
    }
    float brightness = color.getBrightness();
    float myHue = color.getHue();
    
    if(brightness > 20){
        color.setBrightness(brightness -= 0.5);
        color.setHue(myHue -= 0.5);
    }
}

void particle::draw(){
    ofSetColor(color);
    ofDrawCircle(position,size);
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
