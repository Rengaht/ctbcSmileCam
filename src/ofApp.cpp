#include "ofApp.h"
#include "SceneSleep.h"
#include "SceneClaim.h"
#include "SceneSelect.h"
#include "SceneConfirm.h"
#include "SceneDetect.h"
#include "ScenePreview.h"
#include "QRcodeImage.h"

#include "FindKinect.h"

//--------------------------------------------------------------
void ofApp::setup(){

	/*initSmileFrame();
	*/
	string timestamp=ofGetTimestampString("%y%m%e%H");
	ofLogToFile("log/SLog_"+timestamp+".txt", true);

	writeLog("----- Start Program -----",true);


	last_frame_millis=ofGetElapsedTimeMillis();
	
	global_param=new GlobalParam();

	DEBUG_MODE=false;
	CRAZY_MODE=false;

	ofSetVerticalSync(true);
	
	//ofSetFrameRate(60);
	

	initCamera();
	camera_paused=false;

	/* for complete mode */
	FULL_SCREEN=true;
	ofSetFullscreen(FULL_SCREEN);
	ofHideCursor();


	kinect_fail=!(FindKinect());
	if(kinect_fail){
		writeLog("[Device Check] Kinect Fail!",true);		
	}else{
		writeLog("[Device Check] Kinect OK!",true);
	}
	k2face=new K2Face();
	k2face->setGlobalParam(global_param);
	k2face->Init();
	
	
	created_img=new ofImage();

	loadScene();
	loadSmileFrame();
	
	loadSound();
	
	initScene(SceneMode::SLEEP);

	sound_fade_in=false;
	ani_sound_fade.Restart();

	timer_change_scene=FrameAnimation(5);
	timer_scene_timeout=FrameAnimation(global_param->Scene_Timeout);

	// setup upload request
	//ofRegisterURLNotification(this);
	ofAddListener(httpUtils.newResponseEvent, this, &ofApp::newResponse);
	httpUtils.start();

	ofAddListener(upload_manager.formResponseEvent, this, &ofApp::uploadResponse);

	font_debug.loadFont("consola.ttf",32);
	font_debug.setLineHeight(32);
	

	//uploadPhoto("test");
	//createQRCode("hahaha");

}

//--------------------------------------------------------------
void ofApp::update(){
	
	

	/* update delta time for animation*/
	delta_frame_millis=ofGetElapsedTimeMillis()-last_frame_millis;
	last_frame_millis+=delta_frame_millis;


	ofSetWindowTitle(ofToString(ofGetFrameRate()));

	arr_scene[icur_scene]->Update();

	// handle timeout
	timer_scene_timeout.Update(getDeltaMillis());
	if(timer_scene_timeout.isFinished()){
		changeScene(SceneMode::SLEEP);
	}

	if(!camera_paused) 	video_cam.update();
	if(!cam_size_animation.isFinished()) cam_size_animation.Update(delta_frame_millis);
	

	k2face->Update(delta_frame_millis);

	// use timer to delay change btw scenes
	if(icur_scene!=idest_scene){
		arr_scene[idest_scene]->Update();
		timer_change_scene.Update(getDeltaMillis());
		

		//if(timer_change_scene.GetPortion()==1){			
		if(timer_change_scene.isFinished()){			
			arr_scene[icur_scene]->End();
			icur_scene=idest_scene;		
			if(icur_scene==SceneMode::SLEEP){
				sound_fade_in=false;
				ani_sound_fade.Restart();
			}
		}
	}
	

	if(icur_scene==SceneMode::CONFIRM || icur_scene==SceneMode::DETECT){
		if(ismile_frame>-1){
			for(int i=0;i<=ismile_stage;++i){
				int ifr=getFrameIndex(ismile_frame,i);
				if(ifr<0) continue;
				arr_smile_frame[ifr].update();
			}
		}
		
	}
	if(icur_scene==SceneMode::DETECT){
		((SceneDetect*)arr_scene[SceneMode::DETECT])->updateSmileScore(k2face->getTotalSmileScore());
	}

	ofSoundUpdate();
	ani_sound_fade.Update(delta_frame_millis);
	if(ani_sound_fade.isPlaying()){
		if(!sound_fade_in) sound_bgm.setVolume((1-ani_sound_fade.GetPortion())*.5);
		else sound_bgm.setVolume(ani_sound_fade.GetPortion()*.5);
	}

}

float ofApp::getDeltaMillis(){
	return delta_frame_millis;
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
			if(ismile_frame==0){
				// layer order for BUILDING
				if(ismile_stage>=2) arr_smile_frame[2].DrawOnGraph(0,0);
				if(ismile_stage>=1) arr_smile_frame[1].DrawOnGraph(0,0);
				if(ismile_stage>=0) arr_smile_frame[0].DrawOnGraph(0,0);
				
			}else{
				for(int i=0;i<=ismile_stage;++i){
					
					int iframe=getFrameIndex(ismile_frame,i);
					if(iframe<0) continue;

					ofPushStyle();
						// blending for MOTO
						if(iframe==5) ofEnableBlendMode(OF_BLENDMODE_SCREEN);
						if(iframe==8) ofEnableBlendMode(OF_BLENDMODE_ADD);
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
			ofRect(global_param->Kinect_Position_x,
				   global_param->Kinect_Position_y,
				   1920*global_param->Kinect_Scale_x,
				   1080*global_param->Kinect_Scale_y);

			ofSetColor(255,0,0);
			ofLine(global_param->Kinect_Position_x+1920*global_param->Kinect_Left_x,global_param->Kinect_Position_y,
				   global_param->Kinect_Position_x+1920*global_param->Kinect_Left_x,global_param->Kinect_Position_y+1080*global_param->Kinect_Scale_y);
			ofLine(global_param->Kinect_Position_x+1920*global_param->Kinect_Right_x,global_param->Kinect_Position_y,
				   global_param->Kinect_Position_x+1920*global_param->Kinect_Right_x,global_param->Kinect_Position_y+1080*global_param->Kinect_Scale_y);

			ofSetColor(0,0,255);
			ofPushMatrix();
			ofTranslate(50,960);
				font_debug.drawString("Kinect Pos: "+ofToString(global_param->Kinect_Position_x)+" , "+ofToString(global_param->Kinect_Position_y),0,0);
				font_debug.drawString("Kinect Size: "+ofToString(global_param->Kinect_Scale_x)+" , "+ofToString(global_param->Kinect_Scale_y),0,40);
				font_debug.drawString("Kinect Left-x: "+ofToString(global_param->Kinect_Left_x)+" , Right-x: "+ofToString(global_param->Kinect_Right_x),0,80);
			ofPopMatrix();
			ofPopStyle();
		}
	}

	if(CRAZY_MODE){
		ofPushStyle();
		ofSetColor(255,0,0);
		ofNoFill();
			ofRect(1,1,1079,1919);
		ofPopStyle();
	}

	
	
}

void ofApp::drawCamView(){
	float cur_cam_size=cam_size_animation.GetPos();
	float cur_wid=cur_cam_size*1080;
	float cur_hei=cur_cam_size*1920;

	ofPushStyle();
	ofSetRectMode(ofRectMode::OF_RECTMODE_CENTER);
	ofPushMatrix();
	ofTranslate(cur_wid/2,cur_hei/2);
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

		}else if(key=='c'){
			CRAZY_MODE=!CRAZY_MODE;
		}else if(key=='s'){
			global_param->saveParameterFile();
		}else if(key=='r'){
			global_param->readParameterFile();
		}else if(key=='q'){
			//sendSlackLog("send @ "+ofToString(ofGetFrameNum()));
			
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
	
		arr_smile_frame[0]=UIMovie("frame/BUILD/B1.mov",20,104,UIMovie::MOV_HAP);
		arr_smile_frame[1]=UIMovie("frame/BUILD/B2.mov",73,104,UIMovie::MOV_HAP);
		arr_smile_frame[2]=UIMovie("frame/BUILD/B3.mov",73,104,UIMovie::MOV_HAP);
		
		arr_smile_frame[3]=UIMovie("frame/MOTO/M1.mov",51,91,UIMovie::MOV_HAP);
		arr_smile_frame[4]=UIMovie("frame/MOTO/M2.mov",51,91,UIMovie::MOV_HAP);
		arr_smile_frame[5]=UIMovie("frame/MOTO/M3.mov",25,57,UIMovie::MOV_HAP);

		arr_smile_frame[6]=UIMovie("frame/WOMAN/W1.mov",41,80,UIMovie::MOV_HAP);
		arr_smile_frame[7]=UIMovie("frame/WOMAN/W2.mov",33,64,UIMovie::MOV_HAP);
		arr_smile_frame[8]=UIMovie("frame/WOMAN/W3.mov",41,80,UIMovie::MOV_HAP);


		arr_smile_frame[9]=UIMovie("frame/FIRE/F1.mov",106,166,UIMovie::MOV_HAP);
		arr_smile_frame[10]=UIMovie("frame/FIRE/F2.mov",85,133,UIMovie::MOV_HAP);
		arr_smile_frame[11]=UIMovie("frame/FIRE/F3.mov",85,133,UIMovie::MOV_HAP);

		arr_smile_frame[12]=UIMovie("frame/BASEBALL/B3_Pon.mov",31,61,UIMovie::MOV_HAP);
		arr_smile_frame[13]=UIMovie("frame/BASEBALL/B1_Chou.mov",31,61,UIMovie::MOV_HAP);
		arr_smile_frame[14]=UIMovie("frame/BASEBALL/B2_Lin.mov",31,61,UIMovie::MOV_HAP);


		arr_smile_frame[15]=UIMovie("frame/GOLF/G3_Ya.mov",31,61,UIMovie::MOV_HAP);
		arr_smile_frame[16]=UIMovie("frame/GOLF/G2_Hsu.mov",31,61,UIMovie::MOV_HAP);
		arr_smile_frame[17]=UIMovie("frame/GOLF/G4_Hsieh.mov",31,61,UIMovie::MOV_HAP);
		arr_smile_frame[18]=UIMovie("frame/GOLF/G1_Lin.mov",31,61,UIMovie::MOV_HAP);
		
		

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
	webcam_fail=!video_cam.initGrabber(1920,1080);
	if(webcam_fail){
		writeLog("[Device Check] webcam fail!",true);
	}


	cam_size_animation=FrameAnimation(20);
	cam_size_animation.setPos(.5,.5);
	cam_size_animation.setElastic(false);

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

	writeLog(ofToString(icur_scene)+" -> "+ofToString(new_scene),false);

	//arr_scene[icur_scene]->End();
	initScene(new_scene);
	/*if(icur_scene==SceneMode::SLEEP){
		cam_size_animation.setEndPos(1);
		cam_size_animation.Restart();
	}*/
}

void ofApp::initScene(SceneMode set_scene){
	
	if(set_scene<0 || set_scene>5) return;

	if(!DEBUG_MODE && set_scene==SceneMode::SLEEP) 
		((SceneSleep*)arr_scene[SceneMode::SLEEP])->setFailMode(webcam_fail,kinect_fail);
		
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
void ofApp::setScoreEnable(bool set_enable){
	k2face->setEnableScore(set_enable);
}

#pragma region Smile Frame

void ofApp::setSmileFrame(int set_frame){
	if(set_frame<0 || set_frame>=MAPP_FRAME) return;

	ismile_frame=set_frame;
	ismile_stage=0;
	int ifr=getFrameIndex(ismile_frame,ismile_stage);
	if(ifr>-1){ 
		arr_smile_frame[ifr].Init();
		ofAddListener(arr_smile_frame[ifr].event_start_wait,this,&ofApp::onSmileFrameLoopStart);
	}

	k2face->setSelectedFrame(set_frame);

	
	

}
void ofApp::onSmileFrameLoopStart(int &param){
	if(icur_scene==SceneMode::CONFIRM){
		((SceneConfirm*)arr_scene[SceneMode::CONFIRM])->setButtonEnable();
	}
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
	if(create_new_id){
		cur_id_str=ofGetTimestampString("%Y_%m%d_%H%M_%S");
		writeLog("create ID: "+cur_id_str,true);
	}
	//string url_str="http://"+cur_id_str;
	playSound(2);
	createPhoto(cur_id_str);
	//createQRCode(url_str);

	uploadPhoto(cur_id_str);
	/*photo_thread=new PhotoThread(id_str,ismile_frame,pimg,&upload_success);
	photo_thread->startThread();*/
	//upload_success=true;
	camera_paused=false;

	//cur_id_str=id_str;
}
string ofApp::getCurFolder(){
	return ofGetTimestampString("_%Y_%m");
}
void ofApp::createPhoto(string id_str){
		
	
		ofFbo imgfbo;
		imgfbo.allocate(ofGetWidth(),ofGetHeight(),GL_RGBA);
		imgfbo.begin();
			drawCamView();
		
			/*ofImage frameImage;
			frameImage.loadImage("frame_"+ofToString(ismile_frame+1)+".png");
			frameImage.draw(0,0);*/
			if(ismile_frame==0){
				if(ismile_stage>=2) arr_smile_frame[2].DrawOnGraph(0,0);
				if(ismile_stage>=1) arr_smile_frame[1].DrawOnGraph(0,0);
				if(ismile_stage>=0) arr_smile_frame[0].DrawOnGraph(0,0);
				
			}else{
				for(int i=0;i<=ismile_stage;++i){
					int iframe=getFrameIndex(ismile_frame,i);
					if(iframe<0) continue;

					ofPushStyle();
					if(iframe==5) ofEnableBlendMode(OF_BLENDMODE_SCREEN);
					if(iframe==8) ofEnableBlendMode(OF_BLENDMODE_ADD);
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

		string mstr=global_param->Created_File_Path+"/"+getCurFolder()+"/";
		ofDirectory mdir(mstr);
			 
		string img_path=mdir.getAbsolutePath()+"/"+global_param->Created_File_Title+id_str+".png";
		created_img->saveImage(img_path);
		
		//((SceneSleep*)arr_scene[SceneMode::SLEEP])->addNewImage(*created_img);
		
	}


	void ofApp::createQRCode(string url_str){

		QRcodeImage *qr_img=new QRcodeImage();
		qr_img->fetch(url_str,335,0);
		/*if(!qr_img->loaded){
			ofRemoveURLRequest(request_id);
		}*/

		((ScenePreview*)arr_scene[SceneMode::PREVIEW])->setQRcodeImage(qr_img);

	}

	void ofApp::uploadPhoto(string id_str){
		
		string file_path=global_param->Created_File_Path+"/"+getCurFolder()+"/"+
						global_param->Created_File_Title+id_str+".png";;
		writeLog("[Compelete] Upload Photo: "+file_path,false);
		
		
		//upload_success=true;
		if(DEBUG_MODE){
			upload_success=true;
			createQRCode("hahaha");
		}else{
			//ofxHttpForm f;
			//f.action="http://192.168.9.142/P314/ud_room4ARSharePhoto.php";
			//f.method=OFX_HTTP_POST;
			////form field name, file name, mime type
			//f.addFormField("cardId",id_str);
			//f.addFormField("mode","normal");
			//f.addFile("file", file_path);
			//
			////httpUtils.addForm(f);
			//httpUtils.submitForm(f);
			//http_form_manager.submitForm( f, false );
			//HttpForm f = HttpForm( "http://192.168.9.142/P314/ud_room4ARSharePhoto.php" );

			//form field name, file name, mime type
			/*f.addFormField("cardId",id_str);
			f.addFormField("mode","normal");
			f.addFile("file", file_path, "image/png");*/
			
			HttpForm f = HttpForm( "http://www.ctbc-museum.com/upload/upload.php" );

			//form field name, file name, mime type
			f.addFormField("id",id_str);
			f.addFormField("photoframe",ofToString(ismile_frame));
			f.addFile("fileToUpload", file_path, "image/png");
			//f.addFile("fileToUpload", "created/_2015_09/test.png", "image/png");
			upload_manager.setTimeOut(20);
			upload_manager.submitForm(f,false);
		}
	}
	void ofApp::uploadResponse(HttpFormResponse &response){
		ofLog()<<ofToString(response.status) + ": " + (string)response.responseBody;

		if(response.status!=200 && response.status!=201){
			writeLog("[!Warn] Upload time out!",true);
			
			if(!upload_success && icur_scene==SceneMode::PREVIEW){
				upload_success=true;
				createQRCode(global_param->Default_Address);
				
			}
			return;
		}
		
		string response_str=response.responseBody;
		ofLog()<<"Http Response: "<<response.responseBody;
		

		//if(response_str.find("http")>-1){
			size_t pos1=response_str.find("success");
			if(pos1!=std::string::npos){

				int str_p1=response_str.find("url");
				int str_p2=response_str.find_last_of('"');
			
				string res_url=response_str.substr(str_p1+6,str_p2-str_p1-3);
				//if(res_url.find("http")>-1){
				ofLog()<<res_url;		
				
				writeLog("[!Compelete] Photo Uploaded! http:\/\/www.ctbc-museum.com\/upload\/results\/"+cur_id_str,true);
				upload_success=true;
				//createQRCode("http://www.ctbc-museum.com/P314/pages/roomAR2/index.php?"+res_url);
				createQRCode("http://www.ctbc-museum.com/upload/results/"+cur_id_str+".png");
			//}
			}else{
				writeLog("[!Warn] Upload Fail!",true);

				if(!upload_success && icur_scene==SceneMode::PREVIEW){
					upload_success=true;
					createQRCode(global_param->Default_Address);
				
				}
			
			}
		//}
	
	}
	void ofApp::newResponse(ofxHttpResponse &response){
		ofLog()<<ofToString(response.status) + ": " + (string)response.responseBody;

		
	}


	void ofApp::deleteCurPhoto(){
		this->deletePhoto(cur_id_str);
	}
	void ofApp::deletePhoto(string id_str){
		string photo_path=global_param->Created_File_Path+"/"+getCurFolder()+"/"
							+global_param->Created_File_Title+id_str+".png";
		
		//ofFile file;
		//file.open(photo_path,ofFile::ReadWrite);	

		//file.renameTo("_"+id_str);

		//file.remove();
		//ofLog()<<"Delete file: "+photo_path;
		//TODO: delete on server

	}

#pragma endregion


#pragma region Handle Sound

	
void ofApp::loadSound(){

	sound_bgm.loadSound("sound/bgm.wav");
	sound_button.loadSound("sound/button.wav");
	sound_camera.loadSound("sound/camera.wav");
	sound_frame.loadSound("sound/frame.wav");
	
	ani_sound_fade=FrameAnimation(80);
	ani_sound_fade.setElastic(false);


	sound_bgm.setLoop(true);
	sound_bgm.play();
	sound_bgm.setVolume(0.5);
}

void ofApp::playSound(int itype){

	switch(itype){
		case 0:
			sound_button.play();
			break;
		case 1:
			sound_frame.play();
			break;
		case 2:
			sound_camera.play();
			break;
		
	}


}
void ofApp::startBgm(){
	sound_fade_in=true;
	ani_sound_fade.Restart();
}

#pragma endregion

#pragma region LOG
void ofApp::writeLog(string log_text,boolean send_slack){
	if(send_slack) sendSlackLog(log_text);
	ofLog()<<log_text;
}
void ofApp::sendSlackLog(string content){
	
	cout<<"send to slack: "<<content<<endl;

	ofxHttpForm f;
	f.action="https://hooks.slack.com/services/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";
	f.method=OFX_HTTP_POST;
	string text_str="{\"text\":\""+content+"\"}";
	f.addFormField("payload",text_str);
	//cout<<text_str<<endl;
	httpUtils.submitForm(f);
	

}
#pragma endregion

void ofApp::exit(){

	writeLog("----- Program Exit! -----",true);
	arr_scene[icur_scene]->End();
	httpUtils.stop();
	upload_manager.stopThread();
	/*delete (SceneSleep*)arr_scene[0];
	delete (SceneClaim*)arr_scene[1];
	delete (SceneSelect*)arr_scene[2];
	delete (SceneConfirm*)arr_scene[3];
	delete (SceneDetect*)arr_scene[4];
	delete (ScenePreview*)arr_scene[5];*/


	delete k2face;
	//delete [] arr_smile_frame;


	//std::exit(1);

}