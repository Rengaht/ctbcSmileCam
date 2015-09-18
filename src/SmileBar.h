#include "ofMain.h"

class SmileBar{
private:
	ofImage bar_top;
	ofImage bar_bottom;

	ofTrueTypeFont score_font;

	int smile_score;
	float dest_smile_score;
	FrameAnimation ani_show;

	float bonus_score;

public:
	SmileBar(){
		bar_top.loadImage("bar_2.png");
		bar_bottom.loadImage("bar_1.png");

		score_font.loadFont("MyriadPro-Semibold.otf",60);
		score_font.setLetterSpacing(0.85);

		ani_show=FrameAnimation(40);
	}
	void update(float delta_t){
		ani_show.Update(delta_t);
	}

	void drawOnGraph(float dx,float dy){

		ofPushMatrix();
		ofTranslate(dx,dy);

		ofPushStyle();
		ofSetColor(255,255*ani_show.GetPortion());
		
		bar_bottom.draw(0,0);
		float draw_length=180+smile_score*5;
		bar_top.drawSubsection(0,0,draw_length,180,0,0,draw_length,180);
		
		ofPushStyle();
		ofPushMatrix();
			ofTranslate(110,120);
			ofScale(.7,.7);
			ofPushMatrix();
			ofScale(1,1.5);
				score_font.drawString("%",0,0);
			ofPopMatrix();
				
				float char_pos=score_font.stringWidth(ofToString(smile_score))+6;
				ofTranslate(-char_pos,0);
				ofScale(1,1.5);
					score_font.drawString(ofToString(smile_score),0,0);
					/*string num_str=ofToString(smile_score);
					int num_len=num_str.length();

					
					for(int i=num_len-1;i>=0;--i){
						char num_char=num_str.charAt(i);
						pg.text(num_char,-char_pos,0);
						char_pos+=pg.textWidth(num_char);
					}*/
			ofPopMatrix();
		ofPopStyle();

		ofPopStyle();

		ofPopMatrix();

		updateSmileScore();
	
	}
	void addSmileScore(){
		addSmileScore(1);
	}
	void addSmileScore(float add_score){
		bonus_score+=add_score;
	}
	void setSmileScore(float set_score){
		dest_smile_score=set_score;
	}

	void updateSmileScore(){
		if(smile_score<dest_smile_score+bonus_score) smile_score+=1;	
		smile_score=ofClamp(smile_score,0,100);
	}

	void Init(){
		smile_score=0;
		dest_smile_score=0;
		ani_show.Restart();

		bonus_score=0;

	}
	int getSmileStage(){
		if(smile_score>=60) return 2;
		else if(smile_score>=20) return 1;
		return 0;
	}
};