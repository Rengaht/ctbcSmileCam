#ifndef UI_MOVIE
#define UI_MOVIE

#include "ofMain.h"
#include "ofGstVideoPlayer.h"
#include "ofxHapPlayer.h"

class UIMovie:public ofVideoPlayer{
	public:
		bool flag_finished;
		bool playing; 

		UIMovie(){
		}

		// no loop
		UIMovie(string file_path,int movie_type){
			setupMovie(file_path,movie_type);
			loop_start_frame=loop_end_frame=total_frame;
			looped=false;
		}

		// pause at frame
		UIMovie(string file_path,int pause_fr,int movie_type){
			setupMovie(file_path,movie_type);
			
			loop_start_frame=loop_end_frame=pause_fr;
			looped=false;

			
		}

		// loop btw frames
		UIMovie(string file_path,int loop_start,int loop_end,int movie_type){
			setupMovie(file_path,movie_type);

			loop_start_frame=loop_start;
			loop_end_frame=loop_end;
			looped=true;


		}
		void DrawOnGraph(float px,float py){
			if(cur_frame<0) return;

			this->draw(px,py);

			if(cur_frame==total_frame) {
				flag_finished=true;
				playing=false;
			}
		}
		void DrawOnGraph(){
			DrawOnGraph(0,0);
		}
		void update(){
			ofVideoPlayer::update();
			
			cur_frame=this->getCurrentFrame();
			//ofLog()<<cur_frame;
			/*if(cur_frame==total_frame) {
				flag_finished=true;
				playing=false;
			}*/
			if(!cur_looped) return;

			if(!flag_loop && cur_frame>=loop_end_frame){
				flag_loop=true;
				if(!looped) this->setPaused(true);
				else{ 
					this->setFrame(loop_start_frame);
					flag_loop=false;
				}
			}

		}
		void Reset(){
			
			/*this->setFrame(0);	
			this->update();*/
			this->stop();
			cur_frame=this->getCurrentFrame();
			//this->setPaused(true);
			cur_looped=true;
			flag_finished=false;
			flag_loop=false;
		}
		void Init(){
			Reset();
			//this->setPaused(false);
			this->play();
			this->setFrame(0);
			cur_frame=this->getCurrentFrame();
			
			playing=true;
		}
		void Continue(){
			if(cur_looped){
				cur_looped=false;
				if(!looped) this->setPaused(false);
			}
		}

		//static int MFRAME;

		static int MOV_HAP;
		static int MOV_GST;
		bool flag_loop;
		
	private:
		bool looped;
		bool cur_looped;

		int loop_start_frame;
		int loop_end_frame;
		int total_frame;
		int cur_frame;

		void setupMovie(string file_path,int movie_type){
			if(movie_type==MOV_HAP) setPlayer(ofPtr<ofxHapPlayer>(new ofxHapPlayer));
			else if(movie_type==MOV_GST) setPlayer(ofPtr<ofGstVideoPlayer>(new ofGstVideoPlayer));

			this->setPixelFormat(ofPixelFormat::OF_PIXELS_RGBA);
			this->loadMovie(file_path);
			this->setLoopState(ofLoopType::OF_LOOP_NONE);

			total_frame=this->getTotalNumFrames();

			flag_finished=false;
			flag_loop=false;
			playing=false;
		}
};


#endif