#ifndef SCENE_CONFIRM
#define SCENE_CONFIRM


#include "SceneBase.h"
#include "UIMovie.h"

class SceneConfirm:public SceneBase{

public:
	
	SceneConfirm(ofApp *set_ptr):SceneBase(set_ptr){
		
		mbutton=2;
		arr_button=new PButton[mbutton];
		arr_button[0]=PButton(ofRectangle(205,840,250,250),0);
		arr_button[1]=PButton(ofRectangle(625,840,250,250),1);
		

		//movie_back=UIMovie("next_hap.mov",15,UIMovie::MOV_HAP);
		movie_back=UIMovie("next_1.mov",15,UIMovie::MOV_HAP);
		
		ofAddListener(movie_back.event_finish,this,&SceneConfirm::onBackMovieFinish);
		ofAddListener(movie_back.event_start_wait,this,&SceneConfirm::onMovieStartWait);

	}
	void DrawContent(){

		//ofBackground(ofColor::black);
		movie_back.DrawOnGraph(0,760);

	}
	void Update(){
		movie_back.update();

		
<<<<<<< HEAD
		if(ptr_app->CRAZY_MODE){
			if(ofRandom(0,900)<1){
				if(ofRandom(0,2)<1) ButtonEvent(0);
				else ButtonEvent(1);
			}
		}
	}
	void onBackMovieFinish(int &param){
		ptr_app->changeScene(next_scene);
		//movie_back.Reset();
	}
	void onMovieStartWait(int &p){
		//for(int i=0;i<mbutton;++i) arr_button[i].setEnable(true);
=======

		//if(movie_back.flag_finished){
		//	ptr_app->changeScene(next_scene);
		//	movie_back.Reset();
		//}
		//if(!arr_button[0].isEnable() && movie_back.flag_loop){
		//	for(int i=0;i<mbutton;++i) arr_button[i].setEnable(true);
		//	//movie_back.flag_loop=false;
		//}
		
>>>>>>> 353a6421e22b79be6ef1fa7a578bcb21c0df7d5c
	}
	void onBackMovieFinish(int &param){
		ptr_app->changeScene(next_scene);
		//movie_back.Reset();
	}
	void onMovieStartWait(int &p){
		for(int i=0;i<mbutton;++i) arr_button[i].setEnable(true);
	}
	void Init(){
		
		movie_back.Init();
		for(int i=0;i<mbutton;++i) arr_button[i].setEnable(false);

	}
	void End(){
		movie_back.stop();

	}

	void ButtonEvent(int index){
		
<<<<<<< HEAD
		if(ptr_app->CRAZY_MODE){
			ofLog()<<"Auto Event: "<<index;
			if(!arr_button[index].isEnable()) return;
		}

=======
//		ofLog()<<index<<" button clicked!";
>>>>>>> 353a6421e22b79be6ef1fa7a578bcb21c0df7d5c
		for(int i=0;i<mbutton;++i) arr_button[i].setEnable(false);
		movie_back.Continue();

		switch(index){
			case 0: //disagree
				next_scene=ofApp::SceneMode::SELECT;
				break;
			case 1: //agree
				next_scene=ofApp::SceneMode::DETECT;
				break;
		}
	}

	void setButtonEnable(){
		for(int i=0;i<mbutton;++i) arr_button[i].setEnable(true);
	}

private:
	UIMovie movie_back;
	ofApp::SceneMode next_scene;

};

#endif