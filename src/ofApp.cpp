#include "ofApp.h"
#include "SceneSleep.h"
#include "SceneClaim.h"
#include "SceneSelect.h"
#include "SceneConfirm.h"
#include "SceneDetect.h"
#include "ScenePreview.h"
#include "QRcodeImage.h"


//--------------------------------------------------------------
void ofApp::setup(){

	/*initSmileFrame();
	*/
	global_param=new GlobalParam();

	DEBUG_MODE=false;


	initCamera();
	camera_paused=false;

	k2face=new K2Face();
	k2face->setGlobalParam(global_param);
	k2face->Init();
	
	initScene();
	initSmileFrame();
	

	initScene(SceneMode::SLEEP);


	timer_change_scene=FrameAnimation(5);
}

//--------------------------------------------------------------
void ofApp::update(){
	
	ofSetWindowTitle(ofToString(ofGetFrameRate()));

	arr_scene[icur_scene]->Update();

	if(!camera_paused) 
		video_cam.update();
	//if(cam_size_animation.GetPortion()<1) cam_size_animation.Update();
	if(!cam_size_animation.isFinished()) cam_size_animation.Update();
	
	k2face->Update();

	// use timer to delay change btw scenes
	if(icur_scene!=idest_scene){
		arr_scene[idest_scene]->Update();
		timer_change_scene.Update();
		

		//if(timer_change_scene.GetPortion()==1){			
		if(timer_change_scene.isFinished()){			
			arr_scene[icur_scene]->End();
			icur_scene=idest_scene;			
		}
	}
	

	if(icur_scene==SceneMode::CONFIRM || icur_scene==SceneMode::DETECT){
		if(ismile_frame>-1){
			for(int i=0;i<=ismile_stage;++i){
				arr_smile_frame[ismile_frame*3+i].update();
			}
		}
		
	}
	if(icur_scene==SceneMode::DETECT){
		((SceneDetect*)arr_scene[SceneMode::DETECT])->updateSmileScore(k2face->getTotalSmileScore());
	}


}

//--------------------------------------------------------------
void ofApp::draw(){
	
	//ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofEnableAlphaBlending();

	//if(cam_size_animation.GetPortion()<1){
	if(!cam_size_animation.isFinished()){
			arr_scene[SceneMode::SLEEP]->DrawOnGraph(DEBUG_MODE);
	}else{
		if(icur_scene!=SceneMode::SLEEP) 
			drawCamView();
	
	}
	
	if(icur_scene==SceneMode::CONFIRM || icur_scene==SceneMode::DETECT){
		if(ismile_frame>-1){
			for(int i=0;i<=ismile_stage;++i){
			//for(int i=0;i<=2;++i){
				int iframe=ismile_frame*3+i;
				ofPushStyle();
				if(iframe==8||iframe==11) ofEnableBlendMode(OF_BLENDMODE_SCREEN);
				arr_smile_frame[iframe].DrawOnGraph(0,0);
				//if(iframe==8||iframe==11) ofDisableBlendMode();
				ofPopStyle();
			
			}
		}

	}
	
	if(icur_scene==SceneMode::DETECT){
		vector<TrackedFace> vface=k2face->getTrackedFace();
		((SceneDetect*)arr_scene[SceneMode::DETECT])->drawTrackedFace(arr_smile_img,vface,DEBUG_MODE);

		if(DEBUG_MODE){
			ofPushStyle();
			ofSetColor(0,255,0);
			ofNoFill();
				ofRect(global_param->Kinect_Position_x,global_param->Kinect_Position_y,
					   1920*global_param->Kinect_Scale_x,1080*global_param->Kinect_Scale_y);
			ofPopStyle();
		}
	}

	arr_scene[icur_scene]->DrawOnGraph(DEBUG_MODE);
	
}

void ofApp::drawCamView(){
	ofPushMatrix();
	ofTranslate(1080,0);
	ofRotate(90);
	ofTranslate(1920,0);
	ofScale(-1,1);
	float cur_cam_size=cam_size_animation.GetPos();
		video_cam.draw(0,0,cur_cam_size*1920.0,cur_cam_size*1080.0);
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
		if(key=='a'){
			DEBUG_MODE=!DEBUG_MODE;
		}else if(key=='s'){
			global_param->saveParameterFile();
		}else if(key=='r'){
			global_param->readParameterFile();
		}else{
			if(!DEBUG_MODE) return;
			switch(key){
				case OF_KEY_LEFT:
					global_param->Kinect_Position_x-=1;
					break;
				case OF_KEY_RIGHT:
					global_param->Kinect_Position_x+=1;
					break;
				case OF_KEY_UP:
					global_param->Kinect_Position_y-=1;
					break;
				case OF_KEY_DOWN:
					global_param->Kinect_Position_y+=1;
					break;
				case 'q':
					global_param->Kinect_Scale_x-=.1;
					break;
				case 'w':
					global_param->Kinect_Scale_x+=.1;
					break;
				case 'z':
					global_param->Kinect_Scale_y-=.1;
					break;
				case 'x':
					global_param->Kinect_Scale_y+=.1;
					break;
			}
		
		}
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	arr_scene[icur_scene]-> HandleMousePressed(x,y);
	
}




void ofApp::initSmileFrame(){
	
	arr_smile_frame=new UIMovie[MAPP_FRAME*3];

		arr_smile_frame[0]=UIMovie("frame/FIRE/F1.mov",85,133,UIMovie::MOV_HAP);
		arr_smile_frame[1]=UIMovie("frame/FIRE/F2.mov",85,133,UIMovie::MOV_HAP);
		arr_smile_frame[2]=UIMovie("frame/FIRE/F3.mov",85,133,UIMovie::MOV_HAP);

		arr_smile_frame[3]=UIMovie("frame/FIRE/F1.mov",85,133,UIMovie::MOV_HAP);
		arr_smile_frame[4]=UIMovie("frame/FIRE/F2.mov",85,133,UIMovie::MOV_HAP);
		arr_smile_frame[5]=UIMovie("frame/FIRE/F3.mov",85,133,UIMovie::MOV_HAP);

		arr_smile_frame[6]=UIMovie("frame/MOTO/M1.mov",51,91,UIMovie::MOV_HAP);
		arr_smile_frame[7]=UIMovie("frame/MOTO/M2.mov",51,91,UIMovie::MOV_HAP);
		arr_smile_frame[8]=UIMovie("frame/MOTO/M3.mov",25,57,UIMovie::MOV_HAP);

		arr_smile_frame[9]=UIMovie("frame/MOTO/M1.mov",51,91,UIMovie::MOV_HAP);
		arr_smile_frame[10]=UIMovie("frame/MOTO/M2.mov",51,91,UIMovie::MOV_HAP);
		arr_smile_frame[11]=UIMovie("frame/MOTO/M3.mov",25,57,UIMovie::MOV_HAP);

		

		/*arr_smile_img=new ofImage[MAPP_FRAME*3];
		arr_smile_img[0].loadImage(""*/

	ismile_frame=-1;
	ismile_stage=-1;

	arr_smile_img[0].loadImage("smile1.png");
	arr_smile_img[1].loadImage("smile2.png");
	arr_smile_img[2].loadImage("smile3.png");
}

void ofApp::initCamera(){
	
	vector<ofVideoDevice> devices = video_cam.listDevices();
	
    for(int i = 0; i < devices.size(); i++){
		cout << devices[i].id << ": " << devices[i].deviceName; 
        if( devices[i].bAvailable ){
            cout << endl;
        }else{
            cout << " - unavailable " << endl; 
        }
	}
    
	video_cam.setDeviceID(0);
	video_cam.setDesiredFrameRate(60);
	video_cam.initGrabber(1920,1080);

	cam_size_animation=FrameAnimation(20);
	cam_size_animation.setPos(.5,.5);
}

void ofApp::pauseCamera(){
	camera_paused=false;
}
void ofApp::initScene(){
	
	icur_scene=SceneMode::SLEEP;

	arr_scene[0]=new SceneSleep(this);
	arr_scene[1]=new SceneClaim(this);
	arr_scene[2]=new SceneSelect(this);
	arr_scene[3]=new SceneConfirm(this);
	arr_scene[4]=new SceneDetect(this);
	arr_scene[5]=new ScenePreview(this);
	
	
}

void ofApp::changeScene(SceneMode new_scene){

	if(new_scene==idest_scene || new_scene==icur_scene)  return;

	ofLog()<<ofToString(icur_scene)<<" -> "<<ofToString(new_scene);

	//arr_scene[icur_scene]->End();
	initScene(new_scene);
	/*if(icur_scene==SceneMode::SLEEP){
		cam_size_animation.setEndPos(1);
		cam_size_animation.Restart();
	}*/
}

void ofApp::initScene(SceneMode set_scene){

	//if(icur_scene!=SceneMode::SLEEP) 
		arr_scene[set_scene]->Init();
	if(set_scene==SceneMode::DETECT) k2face->Reset();

	float dest_pos=1;
	if(set_scene==SceneMode::SLEEP) dest_pos=.5;
	
	if(dest_pos!=cam_size_animation.getEndPos()){
		cam_size_animation.setEndPos(dest_pos);
		cam_size_animation.Restart();
	}

	idest_scene=set_scene;
	timer_change_scene.Restart();
	//icur_scene=set_scene;

	
}

#pragma region Smile Frame

void ofApp::setSmileFrame(int set_frame){
	if(set_frame<0 || set_frame>=MAPP_FRAME) return;

	ismile_frame=set_frame;
	ismile_stage=0;
	arr_smile_frame[ismile_frame*3+ismile_stage].Init();
	//for(int i=0;i<3;++i) arr_smile_frame[ismile_frame*3+i].Init();
}
void ofApp::setSmileStage(int set_stage){
	if(set_stage<0 || set_stage>=3) return;
	//if(ismile_stage==set_stage) return;

	ismile_stage=set_stage;
	if(!arr_smile_frame[ismile_frame*3+ismile_stage].isPlaying()) 
		arr_smile_frame[ismile_frame*3+ismile_stage].Init();
}
void ofApp::stopAllSmileFrame(){
	for(int i=0;i<MAPP_FRAME*3;++i)
		arr_smile_frame[i].stop();

}
#pragma endregion



#pragma region Handle Photo

bool ofApp::uploadSuccess(){
	
	return upload_success;
}

void ofApp::preparePhoto(){
	
	upload_success=false;
	string id_str=ofGetTimestampString("%Y_%m%d_%H%M_%S");
	string url_str="http://"+id_str;
	
	createPhoto(id_str);
	createQRCode(url_str);

	uploadPhoto(id_str);
	/*photo_thread=new PhotoThread(id_str,ismile_frame,pimg,&upload_success);
	photo_thread->startThread();*/
	upload_success=true;
	camera_paused=false;

	cur_id_str=id_str;
}

void ofApp::createPhoto(string id_str){
		
	
		ofFbo imgfbo;
		imgfbo.allocate(ofGetWidth(),ofGetHeight(),GL_RGBA);
		imgfbo.begin();
			ofPushMatrix();
			ofTranslate(1080,0);
			ofRotate(90);
			ofTranslate(1920,0);
			ofScale(-1,1);
				float cur_cam_size=1;
				video_cam.draw(0,0,cur_cam_size*1920.0,cur_cam_size*1080.0);
			ofPopMatrix();
		
			/*ofImage frameImage;
			frameImage.loadImage("frame_"+ofToString(ismile_frame+1)+".png");
			frameImage.draw(0,0);*/
			for(int i=0;i<=ismile_stage;++i){
				int iframe=ismile_frame*3+i;
				ofPushStyle();
				if(iframe==8||iframe==11) ofEnableBlendMode(OF_BLENDMODE_SCREEN);
				arr_smile_frame[iframe].draw(0,0);
				ofPopStyle();
			}

			
			arr_smile_img[ismile_stage].draw(880,220);

		imgfbo.end();

		//int img_size=imgfbo.getWidth()*imgfbo.getHeight();
		ofPixels pixels;
		imgfbo.readToPixels(pixels);

		ofImage* pimg=new ofImage();
		pimg->setFromPixels(pixels.getPixels(),pixels.getWidth(),pixels.getHeight(),OF_IMAGE_COLOR_ALPHA);
		//pimg->grabScreen(0,0,ofGetWidth(),ofGetHeight());

		((ScenePreview*)arr_scene[SceneMode::PREVIEW])->setPhotoImage(pimg);
		pimg->saveImage(global_param->Created_File_Path+global_param->Created_File_Title+id_str+".png");
		
		((SceneSleep*)arr_scene[SceneMode::SLEEP])->addNewImage(*pimg);
		
	}


	void ofApp::createQRCode(string url_str){

		QRcodeImage *qr_img=new QRcodeImage();
		qr_img->fetch(url_str,335,0);

		((ScenePreview*)arr_scene[SceneMode::PREVIEW])->setQRcodeImage(qr_img);

	}

	void ofApp::uploadPhoto(string id){


	}
	void ofApp::deleteCurPhoto(){
		this->deletePhoto(cur_id_str);
	}
	void ofApp::deletePhoto(string id_str){
		string photo_path=global_param->Created_File_Path+global_param->Created_File_Title+id_str+".png";
		ofFile file;
		/*file.open(photo_path,ofFile::ReadWrite);
		file.remove();
		file.close();*/
		//TODO: delete on server

	}

#pragma endregion

