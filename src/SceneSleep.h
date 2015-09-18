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
		
		//play_ani=FrameAnimation(800);
		show_ani[0]=FrameAnimation(60,60,1080,540);
		show_ani[1]=FrameAnimation(60,240,1080,540);
		show_ani[2]=FrameAnimation(60,480,-540,0);
		show_ani[3]=FrameAnimation(60,600,-540,0);
		
		ani_fade=FrameAnimation(80);
		ani_fade.setElastic(false);

		loadPrevImage();
		//Init();
		ofAddListener(movie_start.event_finish,this,&SceneSleep::onBackMovieFinish);
<<<<<<< HEAD

		img_live.loadImage("live.png");

		mpic=3;
		game_available=true;


=======
>>>>>>> 353a6421e22b79be6ef1fa7a578bcb21c0df7d5c
	}


	void DrawContent(){

		//ofBackground(ofColor::black);
		
		v_prev_image[ishow_img[0]].draw(540,960,540,960);
		v_prev_image[ishow_img[1]].draw(540,0,540,960);
		v_prev_image[ishow_img[2]].draw(0,960,540,960);
		if(mpic==4) v_prev_image[ishow_img[3]].draw(0,0,540,960);
		
		v_prev_image[inext_img[0]].draw(show_ani[0].GetPos(),960,540,960);
		v_prev_image[inext_img[1]].draw(show_ani[1].GetPos(),0,540,960);
		v_prev_image[inext_img[2]].draw(show_ani[2].GetPos(),960,540,960);
		if(mpic==4) v_prev_image[inext_img[3]].draw(show_ani[3].GetPos(),0,540,960);
		
		
			
			if(mpic==3){
				ptr_app->drawCamView();
				
				ofPushStyle();
				ofSetColor(255,255*(1-ani_fade.GetPortion()));
					
					img_live.draw(20,40);					
					if(game_available) movie_start.DrawOnGraph(0,810);

<<<<<<< HEAD
				ofPopStyle();
			}
		
=======
		ptr_app->drawCamView();

		ofPushStyle();
		ofSetColor(255,255*(1-ani_fade.GetPortion()));
			movie_start.DrawOnGraph(0,810);
		ofPopStyle();
>>>>>>> 353a6421e22b79be6ef1fa7a578bcb21c0df7d5c

	}
	void Update(){
		float delta_t=ptr_app->getDeltaMillis();

		for(int i=0;i<mpic;++i) show_ani[i].Update(delta_t);

		movie_start.update();
		
<<<<<<< HEAD
		ani_fade.Update(delta_t);
=======
		ani_fade.Update();
>>>>>>> 353a6421e22b79be6ef1fa7a578bcb21c0df7d5c
		if(ani_fade.GetPortion()==1){
			ptr_app->changeScene(ofApp::SceneMode::CLAIM);
		}

		play_ani.Update(delta_t);
		bool all_finish=true;
		for(int i=0;i<mpic;++i){
			if(!show_ani[i].isFinished()){
				all_finish=false;
				break;
			}
		}

		if(all_finish){
			updateShowIndex();
			/*play_ani.setDelay(ofRandom(600,800));
			play_ani.Restart();*/
<<<<<<< HEAD
		}


		if(ptr_app->CRAZY_MODE){
			if(ofRandom(0,900)<1){
				ofLog()<<"Auto Enter...";
				ButtonEvent(0);
			}
		}

=======
		}



>>>>>>> 353a6421e22b79be6ef1fa7a578bcb21c0df7d5c
	}
	void onBackMovieFinish(int & param){
		//ptr_app->changeScene(ofApp::SceneMode::CLAIM);
		//movie_start.Reset();
	}
	void Init(){
		ofLog()<<"sleep scene init!";

		movie_start.Init();
		arr_button[0].setEnable(true);
		
		//play_ani.Restart();
		for(int i=0;i<mpic;++i) show_ani[i].Restart();

		//loadPrevImage();
		for(int i=0;i<mpic;++i) ishow_img[i]=(mshow_img-mpic+i)%mshow_img;
		for(int i=0;i<mpic;++i){
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
		
		if(ptr_app->CRAZY_MODE){
			ofLog()<<"Auto Event: "<<index;
			if(!arr_button[index].isEnable()) return;
		}

		if(!game_available) return;

		ofLog()<<index<<" button clicked!";
		switch(index){
			case 0:
				// go to next scene
				arr_button[0].setEnable(false);
				movie_start.setPaused(true);
				ani_fade.Restart();
<<<<<<< HEAD
				ptr_app->startBgm();
=======
>>>>>>> 353a6421e22b79be6ef1fa7a578bcb21c0df7d5c
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

<<<<<<< HEAD
	void setFailMode(bool fail_camera,bool fail_kinect){
		if(fail_camera) mpic=4;
		else mpic=3;

		if(fail_camera||fail_kinect) game_available=false;
	}

	


=======
	

>>>>>>> 353a6421e22b79be6ef1fa7a578bcb21c0df7d5c
private:
	UIMovie movie_start;
	ofImage img_live;

	deque<ofImage> v_prev_image;
	int ishow_img[4];
	int inext_img[4];
	int mshow_img;
	static const int MAX_MIMG=25;
	FrameAnimation play_ani;
	FrameAnimation show_ani[4];

	FrameAnimation ani_fade;

	int mpic; //4 for failed camera
	bool game_available;

	FrameAnimation ani_fade;

	void loadPrevImage(){
		v_prev_image.clear();

		vector<ofImage> tmp_vec;
		string dir_path=ptr_app->global_param->Created_File_Path;
		ofDirectory img_dir(dir_path);
		
		img_dir.listDir();
		//img_dir.sort();

		int mfolder=img_dir.numFiles();
		int ifolder=0;

		mshow_img=0;

		for(int i=0;i<mfolder;++i){
			ofFile sub_file=img_dir.getFile(mfolder-1-i);
			if(!sub_file.isDirectory()){
				if(sub_file.getExtension()=="png"){
					ofImage img;
			
<<<<<<< HEAD
					img.loadImage(sub_file);
					v_prev_image.push_back(img);
					ofLog()<<"Load Image:¡@"<<sub_file.getFileName();

					mshow_img++;
					if(mshow_img>=MAX_MIMG) break;
					continue;
				}
				
			}
			ofDirectory sub_dir(sub_file.getAbsolutePath());
=======
			img.loadImage(img_dir.getFile(mshow_img));
			v_prev_image.push_front(img);
>>>>>>> 353a6421e22b79be6ef1fa7a578bcb21c0df7d5c
			
			sub_dir.allowExt("png");
			sub_dir.listDir();
			//sub_dir.sort();

			int mfile=sub_dir.numFiles();
			for(int j=0;j<mfile;++j){	
			//while(mshow_img<MAX_MIMG && mshow_img<mfile){
				ofImage img;
				ofFile sub_file=sub_dir.getFile(mfile-1-j);
				
				img.loadImage(sub_file);
				v_prev_image.push_back(img);
				ofLog()<<"Load Image:¡@"<<sub_file.getFileName();

				mshow_img++;

				if(mshow_img>=MAX_MIMG) break;
			}
			if(mshow_img>=MAX_MIMG) break;
		}
<<<<<<< HEAD
=======

>>>>>>> 353a6421e22b79be6ef1fa7a578bcb21c0df7d5c
		mshow_img=v_prev_image.size();
		
		img_dir.close();
	}
	void updateShowIndex(){
		int iprev=ishow_img[mpic-1];
		for(int i=0;i<mpic;++i)
			ishow_img[i]=(iprev+(i+1))%mshow_img;

		for(int i=0;i<mpic;++i)
			inext_img[i]=(iprev+(i+mpic+1))%mshow_img;

		
		float *delay;
		delay=new float[mpic];
		for(int i=0;i<mpic;++i){
			delay[i]=ofRandom(40+120*i,120+250*i);			
		}
		vector<int> idelay;
		for(int i=0;i<mpic;++i) idelay.push_back(i);
		ofRandomize(idelay);

		for(int i=0;i<mpic;++i){
			show_ani[i].setDelay(delay[idelay[i]]);
			show_ani[i].Restart();
		}
	}

};

#endif