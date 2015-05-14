#ifndef PHOTO_THREAD
#define PHOTO_THREAD

#include "ofMain.h"
#include "ofThread.h"
#include "QRcodeImage.h"

class PhotoThread:public ofThread{
public:
	PhotoThread(string set_id,int set_frame,ofImage set_photo,bool* set_flag){
		ismile_frame=set_frame;
		photo_img=set_photo;
		photo_id=set_id;
		flag_success=set_flag;
	}
	void threadedFunction(){

		string url_str="http://"+photo_id;

		createPhoto(photo_id);
		createQRCode(url_str);

		uploadPhoto(photo_id);

		//lock();
		*flag_success=true;

	}
	ofImage getPhoto(){
		return *result_img;
	}
	QRcodeImage getQRcode(){
		return qr_img;
	}

private:
	bool* flag_success;
	int ismile_frame;
	ofImage photo_img;
	string photo_id;
	ofImage* result_img;
	QRcodeImage qr_img;

	void createPhoto(string id_str){
		
	
		ofFbo imgfbo;
		imgfbo.allocate(ofGetWidth(),ofGetHeight(),GL_RGBA);
		imgfbo.begin();
			ofPushMatrix();
			ofTranslate(1080,0);
			ofRotate(90);
			ofTranslate(1920,0);
			ofScale(-1,1);
				float cur_cam_size=1;
				photo_img.draw(0,0,cur_cam_size*1920.0,cur_cam_size*1080.0);
			ofPopMatrix();
		
			ofImage frameImage;
			frameImage.loadImage("frame_"+ofToString(ismile_frame+1)+".png");
			frameImage.draw(0,0);
		imgfbo.end();

		//int img_size=imgfbo.getWidth()*imgfbo.getHeight();
		ofPixels pixels;
		imgfbo.readToPixels(pixels);

		result_img=new ofImage();
		result_img->setFromPixels(pixels.getPixels(),pixels.getWidth(),pixels.getHeight(),OF_IMAGE_COLOR_ALPHA);
	

		//((ScenePreview*)arr_scene[SceneMode::PREVIEW])->setPhotoImage(pimg);
		result_img->saveImage("created/smilecam_"+id_str+".png");
	
	}


	void createQRCode(string url_str){

		//qr_img=new QRcodeImage();
		qr_img.fetch(url_str,335,0);

		//((ScenePreview*)arr_scene[SceneMode::PREVIEW])->setQRcodeImage(qr_image);

	}

	void uploadPhoto(string id){


	}

};


#endif