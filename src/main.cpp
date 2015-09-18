
#include "ofApp.h"
#include "ofMain.h"


//========================================================================

int ofApp::MAPP_FRAME=12;
int ofApp::MTOTAL_FRAME=19;
int ofApp::FRAME_PER_PAGE=4;

int UIMovie::MOV_HAP=1;
int UIMovie::MOV_GST=0;

float FrameAnimation::millis_per_frame=16.6667;


int main( ){
//	ofSetupOpenGL(1080,1920,OF_GAME_MODE);			// <-------- setup the GL context
	ofSetupOpenGL(1080,1920,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
