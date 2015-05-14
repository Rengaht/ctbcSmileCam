
#include "ofApp.h"
#include "ofMain.h"

//========================================================================

int ofApp::MAPP_FRAME=4;
int UIMovie::MOV_HAP=1;
int UIMovie::MOV_GST=0;


int main( ){
	ofSetupOpenGL(1080,1920,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
