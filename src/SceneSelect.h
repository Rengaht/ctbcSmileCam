#ifndef SCENE_SELECT
#define SCENE_SELECT


#include "SceneBase.h"
#include "UIMovie.h"

class SceneSelect:public SceneBase{

public:
	
	SceneSelect(ofApp *set_ptr):SceneBase(set_ptr){
		
		mbutton=5;
		arr_button=new PButton[mbutton];
		arr_button[0]=PButton(ofRectangle(66,1112,230,230),0);
		arr_button[1]=PButton(ofRectangle(316,1140,230,230),1);
		arr_button[2]=PButton(ofRectangle(566,1101,230,230),2);
		arr_button[3]=PButton(ofRectangle(814,1135,230,230),3);
		arr_button[4]=PButton(ofRectangle(702,1393,186,186),4);
		
		movie_list_people=UIMovie("list_export down people.mov",60,UIMovie::MOV_GST);
		movie_list=UIMovie("list_export down.mov",55,UIMovie::MOV_GST);
		movie_top_in=UIMovie("list_in_up.mov",UIMovie::MOV_GST);
		
		movie_top_leave=new UIMovie[ofApp::MAPP_FRAME];
		for(int i=0;i<4;++i)
			movie_top_leave[i]=UIMovie("list_out_up_"+ofToString(i+1)+".mov",2,UIMovie::MOV_GST);
		
		yellow_frame.loadImage("list_yellow.png");
		frame_position=new ofPoint[ofApp::MAPP_FRAME];
		frame_position[0]=ofPoint(66,1112);
		frame_position[1]=ofPoint(316,1140);
		frame_position[2]=ofPoint(566,1101);
		frame_position[3]=ofPoint(814,1135);


	}
	void DrawContent(){

		movie_list.DrawOnGraph(0,1040);
		// draw selection rectangle
		if(draw_selection){
			ofPushStyle();
			ofSetColor(0,180);
			for(int i=0;i<ofApp::MAPP_FRAME;++i){
				if(i==iselected_frame){
					ofPushStyle();
					ofSetColor(255);
					yellow_frame.draw(frame_position[iselected_frame].x-12,frame_position[iselected_frame].y-15);
					ofPopStyle();
				}else ofRect(frame_position[i].x,frame_position[i].y,189,203);
			}
			ofPopStyle();
		}
		

		movie_list_people.DrawOnGraph(0,1040);
		
		if(iselected_frame>-1) movie_top_leave[iselected_frame].DrawOnGraph(0,400);
		else movie_top_in.DrawOnGraph(0,550);
		
	}
	void Update(){
		movie_list_people.update();
		movie_list.update();
		movie_top_in.update();
		

		if(movie_top_in.flag_finished){
			chooseFrame(0);
			movie_top_in.Reset();			
		}
		if(!arr_button[0].isEnable() && movie_list.flag_loop){
			for(int i=0;i<ofApp::MAPP_FRAME+1;++i) arr_button[i].setEnable(true);
			//movie_list.flag_loop=false;
		
		}

		if(iselected_frame>-1){
			movie_top_leave[iselected_frame].update();
			if(movie_top_leave[iselected_frame].flag_finished && movie_list.flag_finished && movie_list_people.flag_finished){ 
				//ptr_app->setSmileFrame(iselected_frame);
				ptr_app->setSmileFrame(iselected_frame);
				ptr_app->changeScene(ofApp::SceneMode::CONFIRM);
				movie_top_leave[iselected_frame].Reset();
			}
		}

	}
	void Init(){
		ofLog()<<"sleep scene init!";

		movie_list.Init();
		movie_list_people.Init();
		movie_top_in.Init();
		for(int i=0;i<ofApp::MAPP_FRAME;++i) movie_top_leave[i].Reset();

		for(int i=0;i<ofApp::MAPP_FRAME+1;++i) arr_button[i].setEnable(false);

		iselected_frame=-1;
		draw_selection=false;
	}
	void End(){
		movie_list_people.stop();
		movie_list.stop();
		movie_top_in.stop();

		if(iselected_frame>-1) movie_top_leave[iselected_frame].stop();

	}

	void ButtonEvent(int index){
		
		

		switch(index){
			case 0: 
			case 1: 
			case 2:
			case 3:
				chooseFrame(index);		
				break;
			case 4:
				goIntoFrame();
				break;
		}
	}

	void chooseFrame(int select_frame){
		if(select_frame==iselected_frame){
			//goIntoFrame();
			return;
		}
		ofLog()<<"Frame #"<<select_frame;

		if(iselected_frame>-1) movie_top_leave[iselected_frame].stop();

		iselected_frame=select_frame;
		if(iselected_frame>-1)  movie_top_leave[iselected_frame].Init();
		
		draw_selection=true;

	}
	void goIntoFrame(){
		if(iselected_frame>-1){
			for(int i=0;i<ofApp::MAPP_FRAME+1;++i) arr_button[i].setEnable(false);

			movie_top_leave[iselected_frame].Continue();
			movie_list_people.Continue();
			movie_list.Continue();

			draw_selection=false;

			
			//ptr_app->setSmileFrame(iselected_frame);
		}
	}

private:
	UIMovie movie_list_people;
	UIMovie movie_list;
	UIMovie movie_top_in;
	UIMovie* movie_top_leave;
	int iselected_frame;

	ofImage yellow_frame;
	ofPoint* frame_position;
	bool draw_selection;

};

#endif