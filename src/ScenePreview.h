#ifndef SCENE_PREVIEW
#define SCENE_PREVIEW


#include "SceneBase.h"
#include "UIMovie.h"

class ScenePreview:public SceneBase{

public:
	
	ScenePreview(ofApp *set_ptr):SceneBase(set_ptr){
		
		mbutton=1;
		arr_button=new PButton[mbutton];
		arr_button[0]=PButton(ofRectangle(624,1420,205,205),0);

		movie_back=UIMovie("count_20.mov",UIMovie::MOV_GST);
		movie_photo=UIMovie("end.mov",UIMovie::MOV_GST);
		movie_upload=UIMovie("wait.mov",35,UIMovie::MOV_GST);


		photo_ani=FrameAnimation(25,12);
		qrcode_ani=FrameAnimation(15,36);

		back_photo=new ofImage();
		back_photo->loadImage("photo_frame.png");
		back_qrcode=new ofImage();
		back_qrcode->loadImage("qrcode_frame.png");

		image_qrcode=new ofImage();
		image_qrcode->loadImage("qrcode_frame.png");

	}
	void DrawContent(){

		
		ofPushStyle();
			ofSetColor(0,80);
				ofRect(0,0,1080,1920);
			ofPopStyle();
		//ofBackground(ofColor::black);
		if(movie_upload.playing){			
			image_photo->draw(0,0,1080,1920);
			movie_upload.DrawOnGraph(0,810);
			return;
		}
		movie_back.DrawOnGraph(0,0);
		movie_photo.DrawOnGraph(0,0);
		
		ofPushMatrix();
			float ani_pos=photo_ani.GetPortion();
			float cur_photo_pos_y=ofLerp(0,154.74,ani_pos);//ofLerp(1920,131.74,photo_ani.GetPortion());
			float cur_photo_pos_x=ofLerp(0,389.5,ani_pos);
			float cur_rotate=ofLerp(0,6,ani_pos);
			float cur_photo_size_x=ofLerp(1080,621,ani_pos);
			float cur_photo_size_y=ofLerp(1920,1104,ani_pos);
			
			/*ofTranslate(366.5,cur_photo_pos);
			ofRotate(6);*/
			ofTranslate(cur_photo_pos_x,cur_photo_pos_y);
			ofRotate(cur_rotate);
			back_photo->draw(-cur_photo_size_x*0.035,-cur_photo_size_y*0.02,cur_photo_size_x*1.09,cur_photo_size_y*1.05);
			//image_photo->draw(23,23,621,1104);
			image_photo->draw(0,0,cur_photo_size_x,cur_photo_size_y);

		ofPopMatrix();

		ofPushMatrix();
			float cur_qrcode_pos=ofLerp(1920,1366,qrcode_ani.GetPortion());
			ofTranslate(159.6,cur_qrcode_pos);
			ofRotate(-3);
			back_qrcode->draw(0,0);
			image_qrcode->draw(27,27);
		ofPopMatrix();

	}
	void Update(){
		movie_upload.update();

		movie_back.update();
		movie_photo.update();

		photo_ani.Update();
		qrcode_ani.Update();
		
		if(!upload_finish && ptr_app->uploadSuccess()){
			movie_upload.Continue();
			upload_finish=true;
		}
		if(movie_upload.flag_finished){
			movie_back.Init();
			movie_photo.Init();

			photo_ani.Restart();
			qrcode_ani.Restart();

			movie_upload.Reset();
		}
		if(movie_photo.flag_finished){
			arr_button[0].setEnable(true);
			movie_photo.flag_finished=false;
		}
		if(movie_back.flag_finished){
			ptr_app->changeScene(ofApp::SceneMode::SLEEP);
			movie_back.Reset();
		}
	}
	void Init(){
		
		movie_upload.Init();
		upload_finish=false;

		movie_back.Reset();
		movie_photo.Reset();

		arr_button[0].setEnable(false);
	}
	void End(){
		movie_upload.stop();
		movie_back.stop();
		movie_photo.stop();
	}

	void ButtonEvent(int index){
		switch(index){
			case 0:
				ptr_app->deleteCurPhoto();
				ptr_app->changeScene(ofApp::SceneMode::DETECT);
				break;
		}
	}
	void setPhotoImage(ofImage* set_photo){
		image_photo=set_photo;
	}
	void setQRcodeImage(ofImage* set_qrcode){
		image_qrcode=set_qrcode;
	}
private:
	UIMovie movie_back;
	UIMovie movie_photo;
	UIMovie movie_upload;

	ofApp::SceneMode next_scene;

	ofImage *image_photo,*image_qrcode;
	ofImage *back_photo,*back_qrcode;

	FrameAnimation photo_ani;
	FrameAnimation qrcode_ani;

	bool upload_finish;

};

#endif