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
		

		movie_back=UIMovie("notice.mov",36,UIMovie::MOV_GST);
		
	}
	void DrawContent(){

		//ofBackground(ofColor::black);
		movie_back.DrawOnGraph(0,290);

	}
	void Update(){
		movie_back.update();
		
		if(movie_back.flag_finished){
			ptr_app->changeScene(next_scene);
			movie_back.Reset();
		}
		if(!arr_button[0].isEnable() && movie_back.flag_loop){
			arr_button[0].setEnable(true);
			arr_button[1].setEnable(true);
			//movie_back.flag_loop=false;
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
		
		ofLog()<<index<<" button clicked!";
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
private:
	UIMovie movie_back;
	ofApp::SceneMode next_scene;

};

#endif