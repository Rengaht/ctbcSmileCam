#ifndef SCENE_DETECT
#define SCENE_DETECT

#include "ofApp.h"
#include "SceneBase.h"
#include "UIMovie.h"
#include "SmileBar.h"

static int SMILE_WEIGHT=1.2;

class SceneDetect:public SceneBase{

public:
	
	SceneDetect(ofApp *set_ptr):SceneBase(set_ptr){
		
		mbutton=0;
		
		movie_back=UIMovie("count_10.mov",UIMovie::MOV_GST);
		
		flash_animation=FrameAnimation(10);

	}
	void DrawContent(){

		//ofBackground(ofColor::black);
		if(set_to_shoot){
			ofPushStyle();
			ofSetColor(255);
				ofRect(0,0,ofGetWidth(),ofGetHeight());
			ofPopStyle();
			return;
		}
		
		movie_back.DrawOnGraph(0,0);
		smile_bar.drawOnGraph(351,36);
		
	}
	void Update(){
		movie_back.update();
		if(movie_back.flag_finished){
			set_to_shoot=true;
			
			ptr_app->pauseCamera();
			movie_back.Reset();
			flash_animation.Restart();
			ptr_app->preparePhoto();
		}
		//if(flash_animation.GetPortion()==1){
		if(flash_animation.isFinished()){
			ptr_app->changeScene(ofApp::SceneMode::PREVIEW);
			flash_animation.Reset();
		}else{
			flash_animation.Update();
		}

		smile_bar.updateSmileScore();
		if(smile_bar.getSmileStage()==1) ptr_app->setSmileStage(1);
		else if(smile_bar.getSmileStage()==2) ptr_app->setSmileStage(2);

		if(ofRandom(0,4)<1) smile_bar.addSmileScore(1);
	}

	void Init(){
		
		movie_back.Init();
		smile_bar.Init();
		ptr_app->setSmileStage(0);

		set_to_shoot=false;
		flash_animation.Reset();
	}
	void End(){
		movie_back.stop();
		ptr_app->stopAllSmileFrame();
	}

	void ButtonEvent(int index){

	}
	void updateSmileScore(float score){
		smile_bar.setSmileScore(score*SMILE_WEIGHT);
	}
	void drawTrackedFace(ofImage arr_smile_img[],vector<TrackedFace> trface,bool debug_mode){
		
		if(set_to_shoot) return;

		auto it_face=trface.begin();
		for(;it_face!=trface.end();++it_face){
			int score=it_face->getCurStage();
			it_face->draw(&arr_smile_img[score],debug_mode);
			
		}
		
	}
	
private:

	bool set_to_shoot;

	UIMovie movie_back;
	ofApp::SceneMode next_scene;
	SmileBar smile_bar;
	
	FrameAnimation flash_animation;
	
};

#endif