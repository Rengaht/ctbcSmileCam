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
		
		SceneBase();
		SceneBase(ofApp *set_ptr);
		virtual ~SceneBase();

		void DrawOnGraph(bool debug_mode);
		

		void HandleMousePressed(float mouse_x,float mouse_y);
		
		virtual void DrawContent(){}
		virtual void Init(){}
		virtual void Update(){}
		virtual void End(){}

		virtual void ButtonEvent(int index){}

		void onBackMovieFinish(int & p){}
		void onMovieStartWait(int & p){}
};

#endif