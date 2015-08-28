#ifndef SCENE_SELECT
#define SCENE_SELECT


#include "SceneBase.h"
#include "UIMovie.h"

class SceneSelect:public SceneBase{

public:
	
	SceneSelect(ofApp *set_ptr):SceneBase(set_ptr){
		
		mbutton=7;
		arr_button=new PButton[mbutton];
		arr_button[0]=PButton(ofRectangle(66,1080,230,230),0);
		arr_button[1]=PButton(ofRectangle(316,1140,230,230),1);
		arr_button[2]=PButton(ofRectangle(566,1101,230,230),2);
		arr_button[3]=PButton(ofRectangle(814,1101,230,230),3);
		arr_button[4]=PButton(ofRectangle(702,1393,186,186),4);

		/* left-right arrow */
		arr_button[5]=PButton(ofRectangle(72,1338,80,70),5);
		arr_button[6]=PButton(ofRectangle(929,1338,80,70),6);
		
		//movie_list_people=UIMovie("list_export down people.mov",60,UIMovie::MOV_GST);
		movie_list=UIMovie("list_export down_1.mov",55,UIMovie::MOV_HAP);
		movie_top_in=UIMovie("list_in_up_1.mov",UIMovie::MOV_HAP);
		/*movie_list=UIMovie("list_export down.mov",55,UIMovie::MOV_GST);
		movie_top_in=UIMovie("list_in_up.mov",UIMovie::MOV_GST);*/
		
		movie_top_leave=new UIMovie[ofApp::MAPP_FRAME];
		for(int i=0;i<ofApp::MAPP_FRAME;++i)
			movie_top_leave[i]=UIMovie("list_out/list_out_up_"+ofToString(i+1)+"_1.mov",2,UIMovie::MOV_HAP);
		
		img_yellow_frame.loadImage("list_yellow.png");

		frame_position=new ofPoint[ofApp::FRAME_PER_PAGE];
		frame_position[0]=ofPoint(66,1112);
		frame_position[1]=ofPoint(316,1140);
		frame_position[2]=ofPoint(566,1101);
		frame_position[3]=ofPoint(814,1135);

		arr_ani_sel=new FrameAnimation[ofApp::FRAME_PER_PAGE];
		for(int i=0;i<ofApp::FRAME_PER_PAGE;++i){
			arr_ani_sel[i]=FrameAnimation(25);
			arr_ani_sel[i].setElastic(false);
		}
		ani_top=FrameAnimation(25);
		ani_top.setElastic(false);

		/* for multi-page */
		img_left_arrow.loadImage("list_select_1.png");
		img_right_arrow.loadImage("list_select_2.png");

		arr_img_sel=new ofImage[ofApp::MAPP_FRAME];
		for(int i=0;i<ofApp::MAPP_FRAME;++i){
			arr_img_sel[i].loadImage("list_pic/list_"+ofToString(i+1)+".png");
		}
		icur_page=0;
		TOTAL_PAGE=ceil((float)ofApp::MAPP_FRAME/ofApp::FRAME_PER_PAGE);
		
		ani_page_in=FrameAnimation(40);
		ani_page_in.setElastic(false);

		ani_page_out=FrameAnimation(40);
		ani_page_out.setElastic(false);

		ani_arrow_in=FrameAnimation(40);
		ani_arrow_in.setElastic(false);

		ani_arrow_out=FrameAnimation(40);
		ani_arrow_out.setElastic(false);


		arr_img_people=new ofImage[3];
		for(int i=0;i<3;++i){
			arr_img_people[i].loadImage("people_"+ofToString(i+1)+".png");
		}

		ofAddListener(movie_list.event_finish,this,&SceneSelect::onBackMovieFinish);
		ofAddListener(movie_top_in.event_finish,this,&SceneSelect::onTopMovieFinish);

	}
	void DrawContent(){

		movie_list.DrawOnGraph(0,1040);
		// draw selection rectangle
		
		if(draw_selection){ // while choosing

			float _palpha=ani_page_in.GetPortion();

			for(int i=0;i<ofApp::FRAME_PER_PAGE;++i){

				float _alpha=arr_ani_sel[i].GetPortion();
				

				/* draw pic */
				if(_palpha<1 && ilast_page>-1){
					int lfr=getSelFrame(ilast_page,i);
					if(lfr>-1){
						ofPushStyle();
						ofSetColor(255,255*(1-_palpha));						
								arr_img_sel[lfr].draw(frame_position[i].x,frame_position[i].y);
								arr_img_people[getFramePeople(lfr)].draw(frame_position[i].x,frame_position[i].y);					
						ofPopStyle();
					}
				}
				int ifr=getSelFrame(i);
				if(ifr>-1){
					ofPushStyle();
					ofSetColor(255,255*_palpha);
						arr_img_sel[ifr].draw(frame_position[i].x,frame_position[i].y);
						arr_img_people[getFramePeople(ifr)].draw(frame_position[i].x,frame_position[i].y);
					ofPopStyle();
				}

				/* draw selection */
				if(i==iselected_pos){
					ofPushStyle();
					ofSetColor(255,255*_alpha);
						img_yellow_frame.draw(frame_position[iselected_pos].x-12,frame_position[iselected_pos].y-15);
					ofPopStyle();
					if(last_selected_pos>-1){
						ofPushStyle();
						ofSetColor(0,180*(1-_alpha));
							ofRect(frame_position[i].x,frame_position[i].y,189,203);
						ofPopStyle();
					}
				}else{
					ofPushStyle();
					ofSetColor(0,180*_alpha);
					ofRect(frame_position[i].x,frame_position[i].y,189,203);
					ofPopStyle();
				}
			}
			
		}else{ // while going into frame

			float _palpha=1-ani_page_out.GetPortion();

			for(int i=0;i<ofApp::FRAME_PER_PAGE;++i){
				float _alpha=arr_ani_sel[i].GetPortion();
				

				/* draw pic */
				int ifr=getSelFrame(i);
				if(ifr>-1){
					ofPushStyle();
					ofSetColor(255,255*_palpha);
						arr_img_sel[ifr].draw(frame_position[i].x,frame_position[i].y);
						arr_img_people[getFramePeople(ifr)].draw(frame_position[i].x,frame_position[i].y);
					ofPopStyle();
				}
				
				/* draw selection */
				if(i==iselected_pos){
					ofPushStyle();
					ofSetColor(255,255*(1-_alpha));
						img_yellow_frame.draw(frame_position[iselected_pos].x-12,frame_position[iselected_pos].y-15);
					ofPopStyle();
				}else{
					ofPushStyle();
					ofSetColor(0,180*(1-_alpha));
						ofRect(frame_position[i].x,frame_position[i].y,189,203);
					ofPopStyle();
				}
			}
		
		}
		

		//movie_list_people.DrawOnGraph(0,1040);
		
		if(iselected_frame>-1){
			float _alpha=ani_top.GetPortion();
			ofPushStyle();
			ofSetColor(255,255*_alpha);
				movie_top_leave[iselected_frame].DrawOnGraph(0,400);
			ofPopStyle();
			if(_alpha<1){
				ofPushStyle();
				if(last_selected_frame>-1){
					ofSetColor(255,255*(1-_alpha));
					movie_top_leave[last_selected_frame].DrawOnGraph(0,400);
				}else movie_top_in.DrawOnGraph(0,550);
				ofPopStyle();
			}

		}else movie_top_in.DrawOnGraph(0,550);


		/* for multi-page */
		float _aalpha=draw_selection?ani_arrow_in.GetPortion():(1-ani_arrow_out.GetPortion());
		ofPushStyle();
		ofSetColor(255,255*_aalpha);
			img_left_arrow.draw(72,1338,80,70);
			img_right_arrow.draw(929,1338,80,70);
		ofPopStyle();
	
		
	}
	void Update(){
		//movie_list_people.update();
		movie_list.update();
		movie_top_in.update();

		for(int i=0;i<ofApp::FRAME_PER_PAGE;++i) arr_ani_sel[i].Update();
		ani_top.Update();
		if(ani_top.GetPortion()==1 && draw_selection){
			for(int i=0;i<mbutton;++i) arr_button[i].setEnable(true);
		}

		ani_page_in.Update();
		/*if(ani_page_in.GetPortion()==1 && draw_selection){
			for(int i=0;i<mbutton;++i) arr_button[i].setEnable(true);
		}*/

		ani_page_out.Update();

		ani_arrow_in.Update();
		ani_arrow_out.Update();

		/*if(movie_top_in.flag_finished){
			itest++;
			for(int i=0;i<ofApp::FRAME_PER_PAGE;++i) arr_ani_sel[i].Restart();
			ani_page_in.Restart();
			ani_arrow_in.Restart();

			chooseFrame(0,0);
			movie_top_in.flag_finished=false;						
		}*/
		
		
		//if(!arr_button[0].isEnable() && movie_list.flag_loop){
		//	for(int i=0;i<mbutton;++i) arr_button[i].setEnable(true);
		//	//movie_list.flag_loop=false;
		//
		//}

		if(iselected_frame>-1){
			movie_top_leave[iselected_frame].update();
			//if(movie_top_leave[iselected_frame].flag_finished && movie_list.flag_finished){ 
			//	//ptr_app->setSmileFrame(iselected_frame);
			//	ptr_app->setSmileFrame(iselected_frame);
			//	ptr_app->changeScene(ofApp::SceneMode::CONFIRM);
			//	movie_top_leave[iselected_frame].Reset();
			//}
		}

	}
	void onBackMovieFinish(int & param){
		ptr_app->setSmileFrame(iselected_frame);
		ptr_app->changeScene(ofApp::SceneMode::CONFIRM);
		//movie_top_leave[iselected_frame].Reset();
	}
	void onTopMovieFinish(int &p){
		itest++;
		for(int i=0;i<ofApp::FRAME_PER_PAGE;++i) arr_ani_sel[i].Restart();
		ani_page_in.Restart();
		ani_arrow_in.Restart();

		chooseFrame(0,0);
	}
	void onMovieStartWait(int &p){
		
	}

	void Init(){
		ofLog()<<"select scene init!";

		movie_list.Init();
		//movie_list_people.Init();
		movie_top_in.Init();
		//for(int i=0;i<ofApp::MAPP_FRAME;++i) movie_top_leave[i].Reset();

		for(int i=0;i<mbutton;++i) arr_button[i].setEnable(false);

		iselected_frame=-1;
		last_selected_frame=-1;
		iselected_pos=0;
		last_selected_pos=-1;
		
		icur_page=0;
		ilast_page=-1;

		draw_selection=true;

		for(int i=0;i<ofApp::FRAME_PER_PAGE;++i) arr_ani_sel[i].Reset();
		ani_top.Reset();
		ani_page_in.Reset();
		ani_page_out.Reset();

		ani_arrow_in.Reset();
		ani_arrow_out.Reset();

		itest=0;

	}
	void End(){
		//movie_list_people.stop();
		movie_list.stop();
		movie_top_in.stop();

		for(int i=0;i<ofApp::MAPP_FRAME;++i) movie_top_leave[i].stop();

	}

	void ButtonEvent(int index){
		switch(index){
			case 0: 
			case 1: 
			case 2:
			case 3:
				chooseFrame(index,getSelFrame(index));
				ptr_app->restartTimeout();
				break;
			case 4:
				goIntoFrame();
				break;
			case 5:
				//go prev page
				goToPage((icur_page+TOTAL_PAGE-1)%TOTAL_PAGE);
				ptr_app->restartTimeout();
				break;
			case 6:
				//go next page
				goToPage((icur_page+1)%TOTAL_PAGE);
				ptr_app->restartTimeout();
				break;
		}
	}

	void chooseFrame(int select_pos,int select_frame){

		/* check illegal & last one */
		if(select_frame<0 || select_frame>ofApp::MAPP_FRAME-2) return;

		if(select_frame==iselected_frame){
			//goIntoFrame();
			return;
		}

		for(int i=0;i<mbutton;++i) arr_button[i].setEnable(false);

		ofLog()<<"Frame #"<<select_frame;

		if(iselected_frame>-1) movie_top_leave[iselected_frame].stop();
		arr_ani_sel[iselected_frame].Restart();
		last_selected_frame=iselected_frame;
		last_selected_pos=iselected_pos;

		iselected_frame=select_frame;
		iselected_pos=select_pos;
		if(iselected_frame>-1)  movie_top_leave[iselected_frame].Init();
		
		draw_selection=true;
		arr_ani_sel[iselected_pos].Restart();

		ani_top.Restart();

	}
	int getSelFrame(int ipos){
		return getSelFrame(icur_page,ipos);
	}
	int getFramePeople(int iframe){
		switch(iframe){
			case 0:
				return 1;				
			case 2:
				return 2;
			default:
				return 0;
		}
	}
	int getSelFrame(int page,int ipos){
		int ifr=ipos+ofApp::FRAME_PER_PAGE*page;
		if(ifr<ofApp::MAPP_FRAME && ifr>=0) return ifr;
		else return -1;
	}
	void goIntoFrame(){
		if(iselected_frame>-1){
			for(int i=0;i<mbutton;++i) arr_button[i].setEnable(false);

			movie_top_leave[iselected_frame].Continue();
			//movie_list_people.Continue();
			movie_list.Continue();

			draw_selection=false;
			for(int i=0;i<ofApp::FRAME_PER_PAGE;++i) arr_ani_sel[i].Restart();
			ani_page_out.Restart();
			ani_arrow_out.Restart();

			//ptr_app->setSmileFrame(iselected_frame);
		}
	}
	void goToPage(int go_page){
		ilast_page=icur_page;
		icur_page=go_page;
		ani_page_in.Restart();

		for(int i=0;i<mbutton;++i) arr_button[i].setEnable(false);

		// select pos 0
		chooseFrame(0,getSelFrame(0));
	}
	
private:
	//UIMovie movie_list_people;
	UIMovie movie_list;
	UIMovie movie_top_in;
	UIMovie* movie_top_leave;
	int iselected_frame,iselected_pos;
	int last_selected_frame,last_selected_pos;

	ofImage img_yellow_frame;
	ofPoint* frame_position;
	bool draw_selection;

	FrameAnimation* arr_ani_sel;
	FrameAnimation ani_top;
	FrameAnimation ani_page_in,ani_page_out;
	FrameAnimation ani_arrow_in,ani_arrow_out;

	ofImage img_left_arrow,img_right_arrow;
	ofImage* arr_img_sel;
	ofImage* arr_img_people;

	int TOTAL_PAGE;
	int icur_page,ilast_page;
	int itest;
};

#endif