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
		

		movie_back=UIMovie("next.mov",15,UIMovie::MOV_GST);
		
	}
	void DrawContent(){

		//ofBackground(ofColor::black);
		movie_back.DrawOnGraph(0,760);

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
		
//		ofLog()<<index<<" button clicked!";
		arr_button[0].setEnable(false);
		arr_button[1].setEnable(false);
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
private:
	UIMovie movie_back;
	ofApp::SceneMode next_scene;

};

#endif