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
	FULL_SCREEN=false;

	k2face=new K2Face();
	k2face->setGlobalParam(global_param);
	k2face->Init();
	
	created_img=new ofImage();

	loadScene();
	loadSmileFrame();
	
	

	initScene(SceneMode::SLEEP);


	timer_change_scene=FrameAnimation(5);
	timer_scene_timeout=FrameAnimation(global_param->Scene_Timeout);

	// setup upload request
	//ofRegisterURLNotification(this);
	ofAddListener(http_form_manager.formResponseEvent, this, &ofApp::newResponse);


	
}

//--------------------------------------------------------------
void ofApp::update(){
	
	ofSetWindowTitle(ofToString(ofGetFrameRate()));

	arr_scene[icur_scene]->Update();

	// handle timeout
	timer_scene_timeout.Update();
	if(timer_scene_timeout.isFinished()){
		changeScene(SceneMode::SLEEP);
	}

	if(!camera_paused) 	video_cam.update();
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
				int ifr=getFrameIndex(ismile_frame,ismile_stage);
				if(ifr<0) continue;
				arr_smile_frame[ifr].update();
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
			if(ismile_frame==1){
				if(ismile_stage>=2) arr_smile_frame[5].DrawOnGraph(0,0);
				if(ismile_stage>=1) arr_smile_frame[4].DrawOnGraph(0,0);
				if(ismile_stage>=0) arr_smile_frame[3].DrawOnGraph(0,0);
				
			}else{
				for(int i=0;i<=ismile_stage;++i){
					
					int iframe=getFrameIndex(ismile_frame,ismile_stage);
					if(iframe<0) continue;

					ofPushStyle();
						if(iframe==8) ofEnableBlendMode(OF_BLENDMODE_SCREEN);
						arr_smile_frame[iframe].DrawOnGraph(0,0);
					ofPopStyle();			
				}
			}
			
		
		}

	}
	
	arr_scene[icur_scene]->DrawOnGraph(DEBUG_MODE);

	if(icur_scene==SceneMode::DETECT){
		vector<TrackedFace> vface=k2face->getTrackedFace();
		((SceneDetect*)arr_scene[SceneMode::DETECT])->drawTrackedFace(arr_smile_img,vface,DEBUG_MODE);

		if(DEBUG_MODE){
			ofPushStyle();
			ofSetColor(0,255,0);
			ofNoFill();
			ofRect(global_param->Kinect_Position_x+1920*global_param->Kinect_Left_x,
				   global_param->Kinect_Position_y,
				   1920*global_param->Kinect_Scale_x*(global_param->Kinect_Right_x-global_param->Kinect_Left_x),
				   1080*global_param->Kinect_Scale_y);
			ofPopStyle();
		}
	}

	
	
}

void ofApp::drawCamView(){
	float cur_cam_size=cam_size_animation.GetPos();
	float cur_wid=cur_cam_size*1080;
	float cur_hei=cur_cam_size*1920;

	ofPushStyle();
	ofSetRectMode(ofRectMode::OF_RECTMODE_CENTER);
	ofPushMatrix();
	ofTranslate(1080-cur_wid/2,1920-cur_hei/2);
	ofScale(-1,1);
	ofRotate(90);
		video_cam.draw(0,0,cur_hei,cur_wid);
	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
		
		if(key=='a'){
			DEBUG_MODE=!DEBUG_MODE;
		}else if(key=='f'){
			FULL_SCREEN=!FULL_SCREEN;
			ofSetFullscreen(FULL_SCREEN);

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
					ofClamp(global_param->Kinect_Scale_x,0,1);
					break;
				case 'w':
					global_param->Kinect_Scale_x+=.1;
					ofClamp(global_param->Kinect_Scale_x,0,1);
					break;
				case 'z':
					global_param->Kinect_Scale_y-=.1;
					ofClamp(global_param->Kinect_Scale_y,0,1);
					break;
				case 'x':
					global_param->Kinect_Scale_y+=.1;
					ofClamp(global_param->Kinect_Scale_y,0,1);
					break;
				case 'u':
					global_param->Kinect_Left_x-=.05;
					ofClamp(global_param->Kinect_Left_x,0,1);
					break;
				case 'i':
					global_param->Kinect_Left_x+=.05;
					ofClamp(global_param->Kinect_Left_x,0,1);
					break;
				case 'o':
					global_param->Kinect_Right_x-=.05;
					ofClamp(global_param->Kinect_Right_x,0,1);
					break;
				case 'p':
					global_param->Kinect_Right_x+=.05;
					ofClamp(global_param->Kinect_Right_x,0,1);
					break;

			}
		
		}
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	arr_scene[icur_scene]-> HandleMousePressed(x,y);
	
}




void ofApp::loadSmileFrame(){
	
	arr_smile_frame=new UIMovie[MTOTAL_FRAME];

		arr_smile_frame[0]=UIMovie("frame/FIRE/F1.mov",85,133,UIMovie::MOV_HAP);
		arr_smile_frame[1]=UIMovie("frame/FIRE/F2.mov",85,133,UIMovie::MOV_HAP);
		arr_smile_frame[2]=UIMovie("frame/FIRE/F3.mov",85,133,UIMovie::MOV_HAP);

		arr_smile_frame[3]=UIMovie("frame/BUILD/B1.mov",73,104,UIMovie::MOV_HAP);
		arr_smile_frame[4]=UIMovie("frame/BUILD/B2.mov",73,104,UIMovie::MOV_HAP);
		arr_smile_frame[5]=UIMovie("frame/BUILD/B3.mov",73,104,UIMovie::MOV_HAP);

		arr_smile_frame[6]=UIMovie("frame/MOTO/M1.mov",51,91,UIMovie::MOV_HAP);
		arr_smile_frame[7]=UIMovie("frame/MOTO/M2.mov",51,91,UIMovie::MOV_HAP);
		arr_smile_frame[8]=UIMovie("frame/MOTO/M3.mov",25,57,UIMovie::MOV_HAP);

		arr_smile_frame[9]=UIMovie("frame/WOMAN/W1.mov",33,64,UIMovie::MOV_HAP);
		arr_smile_frame[10]=UIMovie("frame/WOMAN/W2.mov",33,64,UIMovie::MOV_HAP);
		arr_smile_frame[11]=UIMovie("frame/WOMAN/W3.mov",33,64,UIMovie::MOV_HAP);

		arr_smile_frame[12]=UIMovie("frame/GOLF/G1_Lin.mov",31,61,UIMovie::MOV_HAP);
		arr_smile_frame[13]=UIMovie("frame/GOLF/G2_Hsu.mov",31,61,UIMovie::MOV_HAP);
		arr_smile_frame[14]=UIMovie("frame/GOLF/G3_Ya.mov",31,61,UIMovie::MOV_HAP);
		arr_smile_frame[15]=UIMovie("frame/GOLF/G4_Hsieh.mov",31,61,UIMovie::MOV_HAP);
		
		arr_smile_frame[16]=UIMovie("frame/BASEBALL/B1_Chou.mov",31,61,UIMovie::MOV_HAP);
		arr_smile_frame[17]=UIMovie("frame/BASEBALL/B2_Lin.mov",31,61,UIMovie::MOV_HAP);
		arr_smile_frame[18]=UIMovie("frame/BASEBALL/B3_Pon.mov",31,61,UIMovie::MOV_HAP);
		

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
void ofApp::loadScene(){
	
	icur_scene=SceneMode::SLEEP;

	arr_scene[0]=new SceneSleep(this);
	arr_scene[1]=new SceneClaim(this);
	arr_scene[2]=new SceneSelect(this);
	arr_scene[3]=new SceneConfirm(this);
	arr_scene[4]=new SceneDetect(this);
	arr_scene[5]=new ScenePreview(this);
	
	
}

void ofApp::changeScene(SceneMode new_scene){

	timer_scene_timeout.Reset();

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
	
	if(set_scene<0 || set_scene>5) return;

	//if(icur_scene!=SceneMode::SLEEP) 
		arr_scene[set_scene]->Init();
	if(set_scene==SceneMode::DETECT){
		k2face->Reset();
		if(icur_scene==SceneMode::PREVIEW) ((SceneDetect*)arr_scene[SceneMode::DETECT])->setFirstShoot(false);
		else ((SceneDetect*)arr_scene[SceneMode::DETECT])->setFirstShoot(true);
	}
	float dest_pos=1;
	if(set_scene==SceneMode::SLEEP){
		if(created_img->isAllocated())
			((SceneSleep*)arr_scene[SceneMode::SLEEP])->addNewImage(*created_img);
		dest_pos=.5;
	}
	if(dest_pos!=cam_size_animation.getEndPos()){
		cam_size_animation.setEndPos(dest_pos);
		cam_size_animation.Restart();
	}

	idest_scene=set_scene;
	timer_change_scene.Restart();
	//icur_scene=set_scene;
	if(idest_scene!=SceneMode::SLEEP && idest_scene!=SceneMode::PREVIEW && idest_scene!=SceneMode::DETECT)
		restartTimeout();
}
void ofApp::restartTimeout(){
		timer_scene_timeout.Restart();
}


#pragma region Smile Frame

void ofApp::setSmileFrame(int set_frame){
	if(set_frame<0 || set_frame>=MAPP_FRAME) return;

	ismile_frame=set_frame;
	ismile_stage=0;
	int ifr=getFrameIndex(ismile_frame,ismile_stage);
	if(ifr>-1) arr_smile_frame[ifr].Init();


	k2face->setSelectedFrame(set_frame);

	//for(int i=0;i<3;++i) arr_smile_frame[ismile_frame*3+i].Init();
}
void ofApp::setSmileStage(int set_stage){
	if(set_stage<0 || set_stage>=3) return;
	//if(ismile_stage==set_stage) return;

	ismile_stage=set_stage;
	int ifr=getFrameIndex(ismile_frame,ismile_stage);
	if(ifr<0) return;

	if(!arr_smile_frame[ifr].isPlaying()) 
		arr_smile_frame[ifr].Init();
}
void ofApp::stopAllSmileFrame(){
	
	for(int i=0;i<MTOTAL_FRAME;++i)
		arr_smile_frame[i].stop();

}

int ofApp::getFrameIndex(int iframe,int istage){
	
	if(iframe<4) return iframe*3+istage;
	else{
		if(istage==0) return iframe+8;
		else return -1;
	}
}

#pragma endregion



#pragma region Handle Photo

bool ofApp::uploadSuccess(){
	
	return upload_success;
}

void ofApp::preparePhoto(bool create_new_id){
	
	upload_success=false;
	if(create_new_id) cur_id_str=ofGetTimestampString("%Y_%m%d_%H%M_%S");
	//string url_str="http://"+cur_id_str;
	
	createPhoto(cur_id_str);
	//createQRCode(url_str);

	uploadPhoto(cur_id_str);
	/*photo_thread=new PhotoThread(id_str,ismile_frame,pimg,&upload_success);
	photo_thread->startThread();*/
	//upload_success=true;
	camera_paused=false;

	//cur_id_str=id_str;
}

void ofApp::createPhoto(string id_str){
		
	
		ofFbo imgfbo;
		imgfbo.allocate(ofGetWidth(),ofGetHeight(),GL_RGBA);
		imgfbo.begin();
			drawCamView();
		
			/*ofImage frameImage;
			frameImage.loadImage("frame_"+ofToString(ismile_frame+1)+".png");
			frameImage.draw(0,0);*/
			if(ismile_frame==1){
				if(ismile_stage>=2) arr_smile_frame[5].DrawOnGraph(0,0);
				if(ismile_stage>=1) arr_smile_frame[4].DrawOnGraph(0,0);
				if(ismile_stage>=0) arr_smile_frame[3].DrawOnGraph(0,0);
				
			}else{
				for(int i=0;i<=ismile_stage;++i){
					int iframe=getFrameIndex(ismile_frame,ismile_stage);
					if(iframe<0) continue;

					ofPushStyle();
					if(iframe==8||iframe==11) ofEnableBlendMode(OF_BLENDMODE_SCREEN);
					arr_smile_frame[iframe].draw(0,0);
					ofPopStyle();
				}
			}

			
			arr_smile_img[ismile_stage].draw(880,220);

		imgfbo.end();

		//int img_size=imgfbo.getWidth()*imgfbo.getHeight();
		ofPixels pixels;
		imgfbo.readToPixels(pixels);

		created_img->setFromPixels(pixels.getPixels(),pixels.getWidth(),pixels.getHeight(),OF_IMAGE_COLOR_ALPHA);
		
		((ScenePreview*)arr_scene[SceneMode::PREVIEW])->setPhotoImage(created_img);
		created_img->saveImage(global_param->Created_File_Path+global_param->Created_File_Title+id_str+".png");
		
		//((SceneSleep*)arr_scene[SceneMode::SLEEP])->addNewImage(*created_img);
		
	}


	void ofApp::createQRCode(string url_str){

		QRcodeImage *qr_img=new QRcodeImage();
		qr_img->fetch(url_str,335,0);

		((ScenePreview*)arr_scene[SceneMode::PREVIEW])->setQRcodeImage(qr_img);

	}

	void ofApp::uploadPhoto(string id_str){
		 string file_path=global_param->Created_File_Path+global_param->Created_File_Title+id_str+".png";;
		 

		 ofLog()<<"Upload Photo....";
		
		HttpForm f = HttpForm( "http://192.168.9.142/P314/ud_room4ARSharePhoto.php" );
		//form field name, file name, mime type
		f.addFormField("cardId",id_str);
		f.addFormField("mode","normal");
		f.addFile("file", file_path, "image/png");

		//upload_success=true;
		http_form_manager.submitForm( f, false );
	}

	void ofApp::newResponse(HttpFormResponse &response){
		
		if(response.status==HTTPResponse::HTTP_REQUEST_TIMEOUT){
			ofLog()<<"Upload time out...";
			upload_success=true;
			createQRCode("hahaha");
			return;
		}
		ofLog()<<"returned"<<response.responseBody;
		
		string response_str=response.responseBody;
		int str_p1=response_str.find_last_of("=");
		int str_p2=response_str.find_last_of('"');

		string res_url=response_str.substr(str_p1+2,str_p2-str_p1-2);
		ofLog()<<res_url;
		
		upload_success=true;

		createQRCode(res_url);
	}


	void ofApp::deleteCurPhoto(){
		//this->deletePhoto(cur_id_str);
	}
	void ofApp::deletePhoto(string id_str){
		string photo_path=global_param->Created_File_Path+global_param->Created_File_Title+id_str+".png";
		
		/*ofFile file;
		file.open(photo_path,ofFile::ReadWrite);	
		file.remove();*/
		
		//TODO: delete on server

	}

#pragma endregion

