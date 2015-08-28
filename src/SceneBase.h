#ifndef SCENE_BASE
#define SCENE_BASE


#include "ofRectangle.h"
#include "ofMain.h"
#include "PButton.h"

class ofApp;

class SceneBase{
	
	public:
		ofApp* ptr_app;
		PButton* arr_button;
		int mbutton;
		
		SceneBase(){
		}
		SceneBase(ofApp *set_ptr){
			ptr_app=set_ptr;
		}
		
		void DrawOnGraph(bool debug_mode){
			//ofBackground(0,0);
			DrawContent();
			if(debug_mode){
				for(int i=0;i<mbutton;++i) 
					if(arr_button[i].isEnable()) arr_button[i].draw();
			}
		}

		void HandleMousePressed(float mouse_x,float mouse_y){
			for(int i=0;i<mbutton;++i) 
				if(arr_button[i].isEnable()){
					if(arr_button[i].checkClicked(mouse_x,mouse_y)){
						ButtonEvent(arr_button[i].getIndex());
					}
				}
		}

		
		virtual void DrawContent(){}
		virtual void Init(){}
		virtual void Update(){}
		virtual void End(){}

		virtual void ButtonEvent(int index){}

		void onBackMovieFinish(int & p){}
		void onMovieStartWait(int & p){}
};

#endif