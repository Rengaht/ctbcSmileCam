#ifndef PBUTTON
#define PBUTTON

#include "ofRectangle.h"
#include "ofMain.h"

class PButton{
	public:
		PButton(){
		}
		PButton(ofRectangle set_area,int set_index){
			button_area=set_area;
			button_index=set_index;
		}
		bool isEnable(){
			return enabled;
		}
		void setEnable(bool set_enable){
			enabled=set_enable;
		}
		void draw(){
			ofPushStyle();
				ofSetColor(255,0,0,80);
				ofRect(button_area);
			ofPopStyle();
		}
		bool checkClicked(float mouse_x,float mouse_y){
			return (button_area.inside(mouse_x,mouse_y));
		}
		int getIndex(){
			return button_index;
		}
		

	private:
		bool enabled;
		int button_index;
		ofRectangle button_area;
};


#endif