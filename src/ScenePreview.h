#ifndef SCENE_PREVIEW
#define SCENE_PREVIEW


#include "SceneBase.h"
#include "UIMovie.h"

class ScenePreview:public SceneBase{

public:
	
	ScenePreview(ofApp *set_ptr):SceneBase(set_ptr){
		
		mbutton=1;
		arr_button=new PButton[mbutton];
		arr_button[0]=PButton(ofRectangle(660,1580,152,152),0);

		movie_back=UIMovie("count_20_1.mov",UIMovie::MOV_HAP);
		movie_photo=UIMovie("end_1.mov",UIMovie::MOV_HAP);
<<<<<<< HEAD
		movie_upload=UIMovie("wait_1.mov",40,63,UIMovie::MOV_HAP);
=======
		movie_upload=UIMovie("wait_1.mov",35,UIMovie::MOV_HAP);
>>>>>>> 353a6421e22b79be6ef1fa7a578bcb21c0df7d5c
		/*movie_back=UIMovie("count_20.mov",UIMovie::MOV_GST);
		movie_photo=UIMovie("end.mov",UIMovie::MOV_GST);
		movie_upload=UIMovie("wait.mov",35,UIMovie::MOV_GST);*/


		photo_ani=FrameAnimation(25,0);
		qrcode_ani=FrameAnimation(25,36);

		back_photo=new ofImage();
		back_photo->loadImage("photo_frame.png");
		back_qrcode=new ofImage();
		back_qrcode->loadImage("qrcode_frame.png");

		image_qrcode=new ofImage();
		image_qrcode->loadImage("qrcode_frame.png");

		ofAddListener(movie_back.event_finish,this,&ScenePreview::onBackMovieFinish);
		ofAddListener(movie_upload.event_finish,this,&ScenePreview::onUploadMovieFinish);
		ofAddListener(movie_photo.event_finish,this,&ScenePreview::onPhotoMovieFinish);

<<<<<<< HEAD
	}
	~ScenePreview(){
		delete back_photo;
		delete back_qrcode;
		delete image_qrcode;
		//if(image_photo) delete image_photo;

=======
>>>>>>> 353a6421e22b79be6ef1fa7a578bcb21c0df7d5c
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
			if(image_photo->isAllocated()) image_photo->draw(0,0,cur_photo_size_x,cur_photo_size_y);

		ofPopMatrix();

		ofPushMatrix();
			float cur_qrcode_pos=ofLerp(1920,1355,qrcode_ani.GetPortion());
			ofTranslate(130,cur_qrcode_pos);
			ofRotate(-3);
			back_qrcode->draw(0,0);
			if(image_qrcode->isAllocated()) 
				image_qrcode->draw(27,27);
		ofPopMatrix();

	}
	void Update(){

		float delta_t=ptr_app->getDeltaMillis();

		movie_upload.update();

		movie_back.update();
		movie_photo.update();

		photo_ani.Update(delta_t);
		qrcode_ani.Update(delta_t);
		

		if(!upload_finish && ptr_app->uploadSuccess()){
			movie_upload.Continue();
			upload_finish=true;
				
		}
<<<<<<< HEAD
		
		
		/*if(ptr_app->CRAZY_MODE){
			if(qrcode_ani.GetPortion()==1){
				if(ofRandom(0,1500)<1){
					ButtonEvent(0);
				}
			}
		}*/
	}
	void onBackMovieFinish(int & param){
		ptr_app->changeScene(ofApp::SceneMode::SLEEP);
		//movie_back.Reset();
	}
	void onUploadMovieFinish(int & param){
		movie_back.Init();
		movie_photo.Init();
=======
		/*if(movie_upload.flag_finished){
			movie_back.Init();
			movie_photo.Init();
>>>>>>> 353a6421e22b79be6ef1fa7a578bcb21c0df7d5c

		photo_ani.Restart();
		qrcode_ani.Restart();

<<<<<<< HEAD
=======
			movie_upload.Reset();
		}
		if(movie_photo.flag_finished){
			arr_button[0].setEnable(true);
			movie_photo.flag_finished=false;
		}
		if(movie_back.flag_finished){
			ptr_app->changeScene(ofApp::SceneMode::SLEEP);
			movie_back.Reset();
		}*/
	}
	void onBackMovieFinish(int & param){
		ptr_app->changeScene(ofApp::SceneMode::SLEEP);
		//movie_back.Reset();
	}
	void onUploadMovieFinish(int & param){
		movie_back.Init();
		movie_photo.Init();

		photo_ani.Restart();
		qrcode_ani.Restart();

>>>>>>> 353a6421e22b79be6ef1fa7a578bcb21c0df7d5c
		//movie_upload.Reset();
	}
	void onPhotoMovieFinish(int & param){
		arr_button[0].setEnable(true);
	}
	void Init(){
		
		movie_upload.Init();
		upload_finish=false;

		movie_back.Reset();
		movie_photo.Reset();

		arr_button[0].setEnable(false);
		
		qrcode_ani.Reset();
		photo_ani.Reset();
	}
	void End(){
		movie_upload.stop();
		movie_back.stop();
		movie_photo.stop();
	}

	void ButtonEvent(int index){

		if(ptr_app->CRAZY_MODE) ofLog()<<"Auto Event: "<<index;


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