#ifndef SCENE_CLAIM
#define SCENE_CLAIM

#include "SceneBase.h"
#include "UIMovie.h"

class SceneClaim:public SceneBase{

public:
	
	SceneClaim(ofApp *set_ptr):SceneBase(set_ptr){
		
		mbutton=2;
		arr_button=new PButton[mbutton];
		arr_button[0]=PButton(ofRectangle(204,1074,260,260),0);
		arr_button[1]=PButton(ofRectangle(620,1074,260,260),1);
		

		movie_back=UIMovie("notice_1.mov",36,UIMovie::MOV_HAP);
		//movie_back=UIMovie("notice.mov",36,UIMovie::MOV_GST);
		
		ofAddListener(movie_back.event_finish,this,&SceneClaim::onBackMovieFinish);
		ofAddListener(movie_back.event_start_wait,this,&SceneClaim::onMovieStartWait);

	}
	void DrawContent(){

		//ofBackground(ofColor::black);
		movie_back.DrawOnGraph(0,290);

	}
	void Update(){
		movie_back.update();
		

		if(ptr_app->CRAZY_MODE){
			if(ofRandom(0,900)<1){
				if(ofRandom(0,2)<1) ButtonEvent(0);
				else ButtonEvent(1);
			}
		}
		
	}
	void Init(){
		
		movie_back.Init();
		arr_button[0].setEnable(false);
		arr_button[1].setEnable(false);
	}
	void End(){
		
		movie_back.stop();

	}

	void ButtonEvent(int index){
		
		if(ptr_app->CRAZY_MODE){
			ofLog()<<"Auto Event: "<<index;
			if(!arr_button[index].isEnable()) return;
		}

		arr_button[0].setEnable(false);
		arr_button[1].setEnable(false);
		movie_back.Continue();

		switch(index){
			case 0: //agree
				next_scene=ofApp::SceneMode::SELECT;
				break;
			case 1: //disagree
				next_scene=ofApp::SceneMode::SLEEP;
				break;
		}
	}

	void onBackMovieFinish(int & param){
		ptr_app->changeScene(next_scene);
		//movie_back.Reset();
	}
	void onMovieStartWait(int & p){
		for(int i=0;i<mbutton;++i) arr_button[i].setEnable(true);
	}
private:
	UIMovie movie_back;
	ofApp::SceneMode next_scene;

};

#endif