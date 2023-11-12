# Coding2_FInal_project

## Butterfly
### Video Link: https://ual.cloud.panopto.eu/Panopto/Pages/Viewer.aspx?id=1e8e7253-0e17-4ed2-b152-afc7017a6591
### code notes: https://git.arts.ac.uk/22018506/Coding2_FInal_project/blob/main/src/ofApp.cpp

Creative motivations & inspirations:
The creative motivation for this project could be to explore the intersection between visual art, music, and technology. By using OpenFrameworks, the project can bring together a range of creative disciplines to create a multi-sensory experience for the user.

The inspiration for the butterfly design used the OpenGL graphics library to simulate particles and render them as point sprites. The particles are laid out in the shape of a butterfly and their position and colour data are stored in separate vertex buffer objects. Players can design their own butterfly by changing colour and point size with GUI, when players use the mouse to interact with the butterfly, there is a particle effect moving with the mouse.

Moreover, the use of mouse interaction with sound adds an additional layer of interactivity to the project, allowing the player to control and manipulate the sound in real time, players can interact with 3 butterflies(click or drag) to create sound. This can create a more immersive and engaging experience for the user, as they become an active participant in the creation of the music.

In addition, the integration of Arduino into the project can provide an additional dimension of creativity, as the user can interact with the flowers using physical input, like changing the shape and size of the flower. This can create a more tactile and immersive experience for the user, allowing them to engage with the project in a more physical way.

Overall, the project can be seen as an exploration of the creative possibilities that emerge when different disciplines are brought together in a single project. By combining visual art, music, and technology, the project can create a unique and engaging experience for the player that is both immersive and interactive.

Several challenge examples encountered during the design and implementation:
I desire to draw flowers using a potentiometer and button(Arduino), but achieving a flawless geometric shape has proven difficult for me. subsequent framework/algorithmic strategies applied to solve it:  It generates a shape using a loop that iterates over a range of values for a variable 'i' starting from 0.0 to TWO_PI + 0.001, in increments of 0.01.

```
ofBeginShape();
    for(float i=0.0; i<=TWO_PI + 0.001; i+= 0.01){
        auto theta = i + (sin( b*i)/c);
        auto r = amp * (2.0 + byteData * sin(a *i));
        auto x = r * cos(theta);
        auto y = r * sin(theta);
        ofVertex(x,y);// adds a new point to the shape
        ofDrawLine(x * 0.2, y * 0.2,x,y);
    }
    ofEndShape();
```

To check which part of the window the mouse was clicked in to interact with the butterfly. If it’s the purple butterfly, it plays "sound1" and sets its speed and panning based on the y and x coordinates of the mouse, respectively. The speed is set to a value between 0.1 and 1.0, with higher values for lower y values. The panning is set to a value between -1 and 1, with -1 being completely left and 1 being completely right, based on the mouse's x position within the left third of the window.

```
    if (x < widthStep){
        sound1.play();
        sound1.setSpeed( 0.1f + ((float)(ofGetHeight() - y) / (float)ofGetHeight())*10);
        sound1.setPan(ofMap(x, 0, widthStep, -1, 1, true));
    } else if (x >= widthStep && x < widthStep*2){
        sound2.play();
    } else {
        sound3.play();
        sound3.setSpeed( 0.1f + ((float)(ofGetHeight() - y) / (float)ofGetHeight())*3);
        sound3.setPan( ofMap(x, widthStep*2, widthStep*3, -1, 1, true) );
    }
```

The particle effect appears when dragging the mouse:  adding a new particle to a vector of particles every time the user drags the mouse with a button pressed.

```
void ofApp::mouseDragged(int x, int y, int button){
    
     //particles draw
     particle newParticle(mouseX, mouseY,hue);
      particles.push_back(newParticle);//The particles vector is being used to store this new particle object by calling the push_back() function, which adds the new particle to the end of the vector.

}
```

