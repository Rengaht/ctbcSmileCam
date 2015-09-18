#ifndef SCENE_DETECT
#define SCENE_DETECT

#include "ofApp.h"
#include "SceneBase.h"
#include "UIMovie.h"
#include "SmileBar.h"

//static int SMILE_WEIGHT=1;

class SceneDetect:public SceneBase{

public:
	
	SceneDetect(ofApp *set_ptr):SceneBase(set_ptr){
		
		mbutton=0;
		
<<<<<<< HEAD
		movie_back=UIMovie("count_10_2.mov",UIMovie::MOV_HAP);
		//movie_look=UIMovie("look_short.mov",UIMovie::MOV_HAP);
=======
		movie_back=UIMovie("count_10_1.mov",UIMovie::MOV_HAP);
		movie_look=UIMovie("look_1.mov",UIMovie::MOV_HAP);
>>>>>>> 353a6421e22b79be6ef1fa7a578bcb21c0df7d5c
		movie_hint=UIMovie("ready_1.mov",UIMovie::MOV_HAP);

		/*movie_back=UIMovie("count_10.mov",UIMovie::MOV_GST);
		movie_look=UIMovie("look.mov",UIMovie::MOV_GST);*/

		arr_img_hint=new ofImage[4];
		for(int i=0;i<4;++i){
			arr_img_hint[i].loadImage("collect/collect_"+ofToString(i)+".png");
		}

		flash_animation=FrameAnimation(10);
		setFirstShoot(true);

		ofAddListener(movie_back.event_finish,this,&SceneDetect::onBackMovieFinish);
<<<<<<< HEAD
		//ofAddListener(movie_look.event_finish,this,&SceneDetect::onLookMovieFinish);
=======
		ofAddListener(movie_look.event_finish,this,&SceneDetect::onLookMovieFinish);
>>>>>>> 353a6421e22b79be6ef1fa7a578bcb21c0df7d5c
		ofAddListener(movie_hint.event_finish,this,&SceneDetect::onHintMovieFinish);
	}
	~SceneDetect(){
		delete[] arr_img_hint;
	}

	void DrawContent(){

		//ofBackground(ofColor::black);
		if(detect_stage==2){
			ofPushStyle();
			ofSetColor(255);
				ofRect(0,0,ofGetWidth(),ofGetHeight());
			ofPopStyle();
			return;
		}
		
		movie_back.DrawOnGraph(0,0);
<<<<<<< HEAD
		//movie_look.DrawOnGraph(0,270);
=======
		movie_look.DrawOnGraph(0,270);
>>>>>>> 353a6421e22b79be6ef1fa7a578bcb21c0df7d5c
		movie_hint.DrawOnGraph(0,720);
		
		if(detect_stage==1)
			smile_bar.drawOnGraph(351,36);
		
	}
	void Update(){
<<<<<<< HEAD
		//movie_look.update();
		movie_hint.update();
	
=======
		movie_look.update();
		movie_hint.update();
		/*if(movie_look.flag_finished){
			detect_stage=1;
			movie_back.Init();
			smile_bar.Init();
			movie_look.Reset();
		}*/
		
>>>>>>> 353a6421e22b79be6ef1fa7a578bcb21c0df7d5c
		movie_back.update();
		float t=movie_back.getPosition();
		//if(t>=.5 && !movie_look.isPlaying()) movie_look.Init();

<<<<<<< HEAD
		float delta_t=ptr_app->getDeltaMillis();
		smile_bar.update(delta_t);
		
=======
		/*if(movie_back.flag_finished){
			detect_stage=2;
			
			ptr_app->pauseCamera();
			movie_back.Reset();
			flash_animation.Restart();
			ptr_app->preparePhoto(first_shoot);
		}*/
		//if(flash_animation.GetPortion()==1){
>>>>>>> 353a6421e22b79be6ef1fa7a578bcb21c0df7d5c
		if(flash_animation.isFinished()){
			ptr_app->changeScene(ofApp::SceneMode::PREVIEW);
			flash_animation.Reset();
		}else{
			flash_animation.Update(delta_t);
		}

		smile_bar.updateSmileScore();
		int istage=smile_bar.getSmileStage();
		if(istage==1) ptr_app->setSmileStage(1);
		else if(istage==2) ptr_app->setSmileStage(2);

		if(detect_stage==1){
			if(istage<1){
				if(ofRandom(0,8)<1) smile_bar.addSmileScore(1);
			}else{
				if(ofRandom(0,30)<1) smile_bar.addSmileScore(1);
			}
		}
	}
	void onBackMovieFinish(int & param){
		detect_stage=2;
			
		ptr_app->pauseCamera();
		
		flash_animation.Restart();
		ptr_app->preparePhoto(first_shoot);

	}
<<<<<<< HEAD
	void onLookMovieFinish(int &p){
		
		/*movie_hint.Init();
		if(ptr_app->DEBUG_MODE){
			detect_stage=1;
			smile_bar.Init();		
			ptr_app->setScoreEnable(true);
		}*/
	}
	void onHintMovieFinish(int &p){
		
		

		movie_back.Init();

		detect_stage=1;
		smile_bar.Init();		
		ptr_app->setScoreEnable(true);
=======
	void onBackMovieFinish(int & param){
		detect_stage=2;
			
		ptr_app->pauseCamera();
		//movie_back.Reset();
		flash_animation.Restart();
		ptr_app->preparePhoto(first_shoot);
	}
	void onLookMovieFinish(int &p){
		detect_stage=1;
		smile_bar.Init();

		movie_hint.Init();
	}
	void onHintMovieFinish(int &p){
		
		movie_back.Init();
>>>>>>> 353a6421e22b79be6ef1fa7a578bcb21c0df7d5c
		
		//movie_look.Reset();
	}
	void Init(){
		
		//movie_back.Init();
		smile_bar.Init();
<<<<<<< HEAD
		movie_hint.Init();
=======
		movie_look.Init();
>>>>>>> 353a6421e22b79be6ef1fa7a578bcb21c0df7d5c
		ptr_app->setSmileStage(0);

		detect_stage=0;
		flash_animation.Reset();

		

	}
	void End(){
		movie_back.stop();
		ptr_app->stopAllSmileFrame();
		
<<<<<<< HEAD
		//movie_look.stop();
=======
		movie_look.stop();
>>>>>>> 353a6421e22b79be6ef1fa7a578bcb21c0df7d5c
		movie_hint.stop();
	}

	void ButtonEvent(int index){

	}
	void updateSmileScore(float score){
<<<<<<< HEAD
		if(detect_stage!=1) return;

=======
>>>>>>> 353a6421e22b79be6ef1fa7a578bcb21c0df7d5c
		smile_bar.setSmileScore(score);
	}
	void drawTrackedFace(ofImage arr_smile_img[],vector<TrackedFace> trface,bool debug_mode){
		
		if(detect_stage!=1) return;

		auto it_face=trface.begin();
		for(;it_face!=trface.end();++it_face){
			int score=it_face->getCurStage();
			int last=it_face->getLastStage();
			int hfr=it_face->getHintFrame();

			if(last<0) it_face->draw(&arr_smile_img[score],NULL,&arr_img_hint[hfr],debug_mode);
			else it_face->draw(&arr_smile_img[score],&arr_smile_img[last],&arr_img_hint[hfr],debug_mode);
		}
		
	}
	void setFirstShoot(bool set_first){
		first_shoot=set_first;
	}

	
private:
	bool first_shoot;
	int detect_stage;
	

	UIMovie movie_back,movie_look,movie_hint;
	ofApp::SceneMode next_scene;
	SmileBar smile_bar;
	
	FrameAnimation flash_animation;
	
	ofImage* arr_img_hint;
};

#endif