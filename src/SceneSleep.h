#ifndef SCENE_SLEEP
#define SCENE_SLEEP

#include "SceneBase.h"
#include "UIMovie.h"

class SceneSleep:public SceneBase{

public:
	
	SceneSleep(ofApp *set_ptr):SceneBase(set_ptr){
		arr_button=new PButton[1];
		arr_button[0]=PButton(ofRectangle(0,0,ofGetWidth(),ofGetHeight()),0);
		mbutton=1;

		movie_start=UIMovie("standby_1.mov",0,105,UIMovie::MOV_HAP);
		//movie_start=UIMovie("standby.mov",0,105,UIMovie::MOV_GST);
		
		play_ani=FrameAnimation(800);
		show_ani[0]=FrameAnimation(30,60,-540,0);
		show_ani[1]=FrameAnimation(30,240,1080,540);
		show_ani[2]=FrameAnimation(30,480,-540,0);
		
		ani_fade=FrameAnimation(80);
		ani_fade.setElastic(false);

		loadPrevImage();
		//Init();
		ofAddListener(movie_start.event_finish,this,&SceneSleep::onBackMovieFinish);
	}
	void DrawContent(){

		//ofBackground(ofColor::black);
		
		v_prev_image[ishow_img[0]].draw(0,0,540,960);
		v_prev_image[ishow_img[1]].draw(540,0,540,960);
		v_prev_image[ishow_img[2]].draw(0,960,540,960);

		
		v_prev_image[inext_img[0]].draw(show_ani[0].GetPos(),0,540,960);
		v_prev_image[inext_img[1]].draw(show_ani[1].GetPos(),0,540,960);
		v_prev_image[inext_img[2]].draw(show_ani[2].GetPos(),960,540,960);

		ptr_app->drawCamView();

		ofPushStyle();
		ofSetColor(255,255*(1-ani_fade.GetPortion()));
			movie_start.DrawOnGraph(0,810);
		ofPopStyle();

	}
	void Update(){
		for(int i=0;i<3;++i) show_ani[i].Update();

		movie_start.update();
		
		ani_fade.Update();
		if(ani_fade.GetPortion()==1){
			ptr_app->changeScene(ofApp::SceneMode::CLAIM);
		}

		play_ani.Update();
		//if(show_ani[0].GetPortion()==1 && show_ani[1].GetPortion()==1 && show_ani[2].GetPortion()==1){
		if(show_ani[0].isFinished() && show_ani[1].isFinished() && show_ani[2].isFinished()){
			updateShowIndex();
			/*play_ani.setDelay(ofRandom(600,800));
			play_ani.Restart();*/
		}



	}
	void onBackMovieFinish(int & param){
		//ptr_app->changeScene(ofApp::SceneMode::CLAIM);
		//movie_start.Reset();
	}
	void Init(){
		ofLog()<<"sleep scene init!";

		movie_start.Init();
		arr_button[0].setEnable(true);
		
		play_ani.Restart();
		for(int i=0;i<3;++i) show_ani[i].Restart();

		//loadPrevImage();
		for(int i=0;i<3;++i) ishow_img[i]=(mshow_img-3+i)%mshow_img;
		for(int i=0;i<3;++i){
			inext_img[i]=(i)%mshow_img;
			show_ani[i].setDelay(ofRandom(i*120+60,i*360+120));
		}

		ani_fade.Reset();

	}
	void End(){
		movie_start.stop();
		//v_prev_image.clear();
	}

	void ButtonEvent(int index){
		
		ofLog()<<index<<" button clicked!";
		switch(index){
			case 0:
				// go to next scene
				arr_button[0].setEnable(false);
				movie_start.setPaused(true);
				ani_fade.Restart();
				//for(int i=0;i<3;++i) show_ani[i].Reset();
				break;
		}
	}

	void addNewImage(ofImage new_image){
		v_prev_image.push_front(new_image);
		while(v_prev_image.size()>MAX_MIMG){
			v_prev_image.pop_back();
		}
	}

	

private:
	UIMovie movie_start;
	
	deque<ofImage> v_prev_image;
	int ishow_img[3];
	int inext_img[3];
	int mshow_img;
	static const int MAX_MIMG=15;
	FrameAnimation play_ani;
	FrameAnimation show_ani[3];

	FrameAnimation ani_fade;

	void loadPrevImage(){
		v_prev_image.clear();

		vector<ofImage> tmp_vec;
		string dir_path="created";
		ofDirectory img_dir(dir_path);
		img_dir.allowExt("png");
		img_dir.listDir();
		
		//g_dir.sort();
		int mfile=img_dir.numFiles();
		mshow_img=0;
		while(mshow_img<MAX_MIMG && mshow_img<mfile){
			ofImage img;
			
			img.loadImage(img_dir.getFile(mshow_img));
			v_prev_image.push_front(img);
			
			mshow_img++;
		}

		mshow_img=v_prev_image.size();
		
		img_dir.close();
	}
	void updateShowIndex(){
		int iprev=ishow_img[2];
		for(int i=0;i<3;++i)
			ishow_img[i]=(iprev+(i+1))%mshow_img;

		for(int i=0;i<3;++i)
			inext_img[i]=(iprev+(i+4))%mshow_img;

		
		float delay[3]={ofRandom(40,120),ofRandom(360,720),ofRandom(720,1000)};
		vector<int> idelay;
		for(int i=0;i<3;++i) idelay.push_back(i);
		ofRandomize(idelay);

		for(int i=0;i<3;++i){
			show_ani[i].setDelay(delay[idelay[i]]);
			show_ani[i].Restart();
		}
	}

};

#endif