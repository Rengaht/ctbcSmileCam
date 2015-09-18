#ifndef UI_MOVIE
#define UI_MOVIE

#include "ofMain.h"
//#include "ofGstVideoPlayer.h"
#include "ofxHapPlayer.h"

class UIMovie:public ofVideoPlayer{
	public:
		
		static int MOV_HAP;
		static int MOV_GST;
		bool flag_wait;
		bool flag_finish;
		bool playing; 

		int cur_frame;
		
		ofEvent<int> event_finish,event_start_wait;
		
		string _name;

		UIMovie(){
		}

		// no loop
		UIMovie(string file_path,int movie_type){
			setupMovie(file_path,movie_type);
			loop_start_frame=loop_end_frame=total_frame;
			has_loop_span=false;
		}

		// pause at frame
		UIMovie(string file_path,int pause_fr,int movie_type){
			setupMovie(file_path,movie_type);
			
			loop_start_frame=loop_end_frame=pause_fr;
			has_loop_span=false;
			
		}

		// loop btw frames
		UIMovie(string file_path,int loop_start,int loop_end,int movie_type){
			setupMovie(file_path,movie_type);

			loop_start_frame=loop_start;
			loop_end_frame=loop_end;
			has_loop_span=true;


		}
		void DrawOnGraph(float px,float py){
			/*if(cur_frame<0) return;*/
			//if(!isPlaying()) return;

			this->draw(px,py);

			/*if(playing && cur_frame==total_frame){
				flag_finished=true;
				playing=false;
			}*/
		}
		void DrawOnGraph(){
			DrawOnGraph(0,0);
		}
		void update(){

			//if(!playing) return;

			ofVideoPlayer::update();
			
			cur_frame=this->getCurrentFrame();
			//ofLog()<<cur_frame;
			/*if(cur_frame==total_frame) {
				flag_finished=true;
				playing=false;
			}*/

			if(!flag_finish && this->getIsMovieDone()){
				flag_finish=true;
				playing=false;
				int p=1;
				ofNotifyEvent(event_finish,p,this);
			}

			if(!pause_at_loop) return;

			if(!flag_wait && has_loop_span){
				if(cur_frame>=loop_start_frame){
					if(!flag_wait){
						flag_wait=true;
						int p=1;
						ofNotifyEvent(event_start_wait,p,this);
					}
				}
			}
			
			if(cur_frame>=loop_end_frame){

				if(!flag_wait && !has_loop_span){
					this->setPaused(true);					
					if(!flag_wait){
						flag_wait=true;
						int p=1;
						ofNotifyEvent(event_start_wait,p,this);
					}
				}else{ 
					this->setFrame(loop_start_frame);						
				}
				
			}

		}
		void stop(){
			
			ofVideoPlayer::setFrame(0);	
			ofVideoPlayer::update();
			ofVideoPlayer::stop();
		}
		void Reset(){
			
			/*ofVideoPlayer::play();
			ofVideoPlayer::setFrame(0);	
			ofVideoPlayer::update();*/
			//ofVideoPlayer::stop();

			//this->stop();
			//this->setPaused(true);
			//cur_frame=this->getCurrentFrame();
			//this->setPaused(true);
			pause_at_loop=true;
			flag_wait=false;
			flag_finish=false;
			playing=false;
		}
		void Init(){
			Reset();
			//this->setPaused(false);
			this->play();
			this->setPosition(0);
			ofVideoPlayer::update();

			cur_frame=this->getCurrentFrame();
			if(cur_frame!=0){
				/*this->setFrame(0);
				ofVideoPlayer::update();*/
				ofLog()<<_name<<" Not Start From 0 -> "<<cur_frame;
			}
			playing=true;
		}
		void Continue(){
			if(pause_at_loop){
				//flag_wait=false;
				pause_at_loop=false;
				if(!has_loop_span) this->setPaused(false);
			}
		}

		//static int MFRAME;


	private:
		bool has_loop_span;
		bool pause_at_loop;

		int loop_start_frame;
		int loop_end_frame;
		int total_frame;
		

		void setupMovie(string file_path,int movie_type){
			if(movie_type==MOV_HAP) setPlayer(ofPtr<ofxHapPlayer>(new ofxHapPlayer));
			//else if(movie_type==MOV_GST) setPlayer(ofPtr<ofGstVideoPlayer>(new ofGstVideoPlayer));

			this->setPixelFormat(ofPixelFormat::OF_PIXELS_RGBA);
			this->loadMovie(file_path);
			this->setLoopState(ofLoopType::OF_LOOP_NONE);

			total_frame=this->getTotalNumFrames();
			_name=file_path;

			Reset();

			/*this->Init();
			this->stop();*/
		}
};


#endif