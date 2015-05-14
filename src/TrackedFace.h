#pragma once

#ifndef TRACKED_FACE
#define TRACKED_FACE

#include "ofMain.h"
#include "GlobalParam.h"

////static ofPoint Kinect_Position;
////static ofPoint Kinect_Scale;
//static float Kinect_Position_x;
//static float Kinect_Position_y;
//static float Kinect_Scale_x;
//static float Kinect_Scale_y;

//static int MLOST_FRAME=3;
//static int SMOOTH_FRAME=4;
//static float SMILE_STAGE_SCORE[3]={0,.5,1};

class TrackedFace{
private:
	
	GlobalParam* gparam;
	ofImage smile_image[3];

	ofPoint position,sizee;
	//float cur_happy;
	float happy_score;
	int mlost_track;
	//float* accu_happy;
	int dest_happy_stage;
	int cur_happy_stage;
	int maccu;

	void setup(ofPoint position_,ofPoint size_,float happy_,GlobalParam* gparam_){
		
		gparam=gparam_;

		position=position_;
		sizee=size_;
		//cur_happy=happy_;
		cur_happy_stage=happy_;
		happy_score=0;
		mlost_track=0;

		dest_happy_stage=0;
		maccu=0;
		/*accu_happy=new float[SMOOTH_FRAME];
		for(int i=0;i<SMOOTH_FRAME;++i){
			accu_happy[i]=0;
		}*/
	}

public:
	TrackedFace(){
		setup(ofPoint(0,0),ofPoint(0,0),0,nullptr);
	}
	TrackedFace(ofPoint position_,ofPoint size_,int happy_,GlobalParam* gparam_){
		
		setup(position_,size_,happy_,gparam_);
	}
	
	~TrackedFace(){
		//delete smile_image;
	}
	void draw(ofImage* smile_img, bool debug_mode){
		
		if(mlost_track>0) return;

		ofPushMatrix();
		ofTranslate(position.x,position.y);
			/*if(cur_happy>0.4) smile_image[2].draw(0,0);
			else if(cur_happy>0.2) smile_image[1].draw(0,0);
			else smile_image[0].draw(0,0);*/
			smile_img->draw(0,0);

			// text(happy_score,0,0);
			if(debug_mode){
				ofPushStyle();
				ofSetColor(255,0,0);
				ofNoFill();
					ofRect(0,0,sizee.x,sizee.y);
				ofPopStyle();
			}
		ofPopMatrix();
	}

	void updateHappyStage(int tmp_stage){
		if(tmp_stage<cur_happy_stage){
			dest_happy_stage=max(tmp_stage,dest_happy_stage);
			maccu++;
			if(maccu>gparam->Smooth_Frame){
				cur_happy_stage=dest_happy_stage;
				dest_happy_stage=0;
			}
		}else{
			cur_happy_stage=tmp_stage;
			maccu=0;
		}
		
		// happy_score+=score;
	}

	void updateGeometry(ofPoint pos_,ofPoint sizee_){
		if(pos_.x==0 && pos_.y==0){
			mlost_track++;
			// return;
		}else{
			mlost_track=0;
		}
		
		position=pos_;
		sizee=sizee_;
		
	}
	
	bool hasLostTrack(){
		return mlost_track>gparam->mLost_Frame;
	}
	float getCurScore(){
		switch(cur_happy_stage){
			case 0:
				return 0.05;
			case 1:
				return .5;
			case 2:
				return 1;
		}
	}
	int getCurStage(){
		return cur_happy_stage;
	}

};


#endif