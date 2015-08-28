#ifndef FRAME_ANIMATION
#define FRAME_ANIMATION

#include <math.h>  
#include "ofMath.h"

class FrameAnimation{
	public:
		
		
		FrameAnimation(){
			setup(1,0,true);
		}
		FrameAnimation(float set_length){
			setup(set_length,0.0f,true);
		}
		/*FrameAnimation(float set_length,bool elastic){
			setup(set_length,0.0f,elastic);
		}*/
		FrameAnimation(float set_length,float set_delay){
			setup(set_length,set_delay,true);
		}
		FrameAnimation(float set_length,float set_delay,float set_start,float set_end){
			setup(set_length,set_delay,true);
			setPos(set_start,set_end);
		}

		void Start(){
			ani_start=true;
			//ani_t=0;
		}
		bool isFinished(){
			return (ani_t==1);
		}
		float GetPortion(){
			if(!ani_start) return 0;
			if(ani_t<0) return 0;
			if(ani_t==1) return 1;
			//return ofClamp(sinf(ani_t*(HALF_PI)),0,1);
			float k=0.5f;
			float r=is_elastic?(0.5f-0.51f*cosf(PI*ani_t+k*ani_t-k*0.5f)):(0.5-0.5*cosf(ani_t*(PI)));
			return r;
		}
		float GetPos(){
			if(ani_t<0) 
				return start_pos;
			return ofMap(GetPortion(),1,0,end_pos,start_pos);
		}
		void Update(){
			if(!ani_start) return;
			
			if(ani_t<1-ani_vel) ani_t+=ani_vel;
			else ani_t=1;

		}
		void Reset(){
			ani_start=false;
			ani_t=-delay_fr*ani_vel;
			// Start();
		}
		void Restart(){
			Reset();
			Start();
		}
		void setPos(float set_start,float set_end){
			start_pos=set_start;
			end_pos=set_end;
		}
		void setEndPos(float set_end){
			start_pos=end_pos;
			end_pos=set_end;
		}
		void setDelay(float set_delay){
			delay_fr=set_delay;
		}
		float getEndPos(){
			return end_pos;
		}
		void setElastic(bool set_elastic){
			is_elastic=set_elastic;
		}
	private:
		float ani_t,ani_vel;
		float delay_fr;

		float start_pos,end_pos;

		bool ani_start;
		bool is_elastic;

		void setup(float set_length,float set_delay,bool set_elastic){
			ani_vel=1/set_length;
			//ofLog()<<ani_vel;
			delay_fr=set_delay;
			
			is_elastic=set_elastic;

			Reset();
		}
};


#endif