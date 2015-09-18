#include "SceneBase.h"
#include "ofApp.h"

SceneBase::SceneBase(){}

SceneBase::SceneBase(ofApp *set_ptr){
	ptr_app=set_ptr;
}

SceneBase::~SceneBase(){
	if(mbutton>0) delete [] arr_button;
}

void SceneBase::HandleMousePressed(float mouse_x,float mouse_y){
	for(int i=0;i<mbutton;++i) 
		if(arr_button[i].isEnable()){
			if(arr_button[i].checkClicked(mouse_x,mouse_y)){
				ButtonEvent(arr_button[i].getIndex());
				ptr_app->playSound(arr_button[i].getSoundType());
			}
		}
}


void SceneBase::DrawOnGraph(bool debug_mode){
			//ofBackground(0,0);
	DrawContent();
	if(debug_mode){
		for(int i=0;i<mbutton;++i) 
			if(arr_button[i].isEnable()) arr_button[i].draw();
	}
}
