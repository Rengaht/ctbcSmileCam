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
	int last_happy_stage;
	int maccu;
	FrameAnimation ani_change;

	int hint_frame;
	FrameAnimation ani_hint;

	void setup(ofPoint position_,ofPoint size_,float happy_,GlobalParam* gparam_,float camera_dist){
		
		gparam=gparam_;

		camera_distance=camera_dist;

		position=position_;
		sizee=size_;
		//cur_happy=happy_;
		cur_happy_stage=happy_;
		happy_score=0;
		mlost_track=0;

		dest_happy_stage=0;
		last_happy_stage=-1;

		maccu=0;
		/*accu_happy=new float[SMOOTH_FRAME];
		for(int i=0;i<SMOOTH_FRAME;++i){
			accu_happy[i]=0;
		}*/

<<<<<<< HEAD
		ani_change=FrameAnimation(50);
=======
		ani_change=FrameAnimation(20);
>>>>>>> 353a6421e22b79be6ef1fa7a578bcb21c0df7d5c
		ani_change.setElastic(false);
		ani_change.Restart();

		hint_frame=0;
		ani_hint=FrameAnimation(30);
		ani_hint.setElastic(false);
		ani_hint.Restart();
		
	}

public:

	float camera_distance;

	TrackedFace(){
		setup(ofPoint(0,0),ofPoint(0,0),0,nullptr,0);
	}
	TrackedFace(ofPoint position_,ofPoint size_,int happy_,GlobalParam* gparam_,float camera_dist){
		
		setup(position_,size_,happy_,gparam_,camera_dist);
	}
	
	~TrackedFace(){
		//delete smile_image;
	}
<<<<<<< HEAD
	void update(float delta_t){
		ani_change.Update(delta_t);

		ani_hint.Update(delta_t);
=======
	void update(){
		ani_change.Update();

		ani_hint.Update();
>>>>>>> 353a6421e22b79be6ef1fa7a578bcb21c0df7d5c
		if(ani_hint.GetPortion()==1){
			ani_hint.Restart();
			hint_frame=(hint_frame+1)%4;
		}
	}
	void draw(ofImage* smile_img,ofImage* last_smile_img,ofImage* hint_img,bool debug_mode){
		
		if(mlost_track>0) return;
		

		ofPushMatrix();
		ofTranslate(position.x,position.y);
		
			float _alpha=ani_change.GetPortion();
			
			if(last_smile_img!=NULL){
				ofPushStyle();	
				ofSetColor(255,255*(1-_alpha));
					last_smile_img->draw(0,0);
				ofPopStyle();
			}
			ofPushStyle();
			ofSetColor(255,255*_alpha);
				smile_img->draw(0,0);
			ofPopStyle();

			if(debug_mode){
				ofPushStyle();
				ofSetColor(255,0,0);
				ofNoFill();
					ofRect(0,0,sizee.x,sizee.y);
				ofPopStyle();
			}
		
		hint_img->draw(-19,145);

		ofPopMatrix();


	

	}

	void updateHappyStage(int tmp_stage){

		// don't update while changing
		if(ani_change.GetPortion()>0 && ani_change.GetPortion()<1) return;

		if(tmp_stage<cur_happy_stage){
			dest_happy_stage=max(tmp_stage,dest_happy_stage);
			maccu++;
			if(maccu>gparam->Smooth_Frame){
				last_happy_stage=cur_happy_stage;
				cur_happy_stage=dest_happy_stage;
				dest_happy_stage=0;

				ani_change.Restart();
			}
		}else if(tmp_stage>cur_happy_stage){
			last_happy_stage=cur_happy_stage;
			cur_happy_stage=tmp_stage;
			maccu=0;
			ani_change.Restart();
		}
		
		// happy_score+=score;
	}

	void updateGeometry(ofPoint pos_,ofPoint sizee_,float camera_dist){
		if(pos_.x==0 && pos_.y==0){
			mlost_track++;
			// return;
		}else{
			mlost_track=0;
		}
		
		position=pos_;
		sizee=sizee_;
		camera_distance=camera_dist;
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
	int getLastStage(){
		return last_happy_stage;
	}
	int getHintFrame(){
		return hint_frame;
	}

	/*float getCameraDist(){
		return camera_distance;
	}*/
};


#endif