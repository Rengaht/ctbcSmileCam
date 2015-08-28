#pragma once


#include "ofMain.h"
#include "HttpFormManager.h"

#include "UIMovie.h"
#include "SceneBase.h"
#include "FrameAnimation.h"
#include "K2Face.h"
#include "GlobalParam.h"
//#include "PhotoThread.h"

//static string CREATED_FILE_PATH="created/";
//static string CREATED_FILE_TITLE="smilecam_";

class ofApp : public ofBaseApp{

	public:
		
		GlobalParam* global_param;

		enum SceneMode {SLEEP,CLAIM,SELECT,CONFIRM,DETECT,PREVIEW};

		void setup();
		void update();
		void draw();
		void drawCamView();

		void keyPressed(int key);
		void mousePressed(int x, int y, int button);
		

		void pauseCamera();

		void changeScene(SceneMode new_scene);
		

		void setSmileFrame(int set_frame);
		void setSmileStage(int set_stage);
		void stopAllSmileFrame();
		int getFrameIndex(int iframe,int istage);

		void restartTimeout();

		bool uploadSuccess();
		void preparePhoto(bool create_new_id);
		void deleteCurPhoto();
		
		void newResponse(HttpFormResponse &response);
	
		static int MAPP_FRAME;
		static int MTOTAL_FRAME;
		static int FRAME_PER_PAGE;
	
	private:
		bool DEBUG_MODE;
		bool FULL_SCREEN;

		int ismile_frame;
		int ismile_stage;

		UIMovie* arr_smile_frame;
		//ofPoint* arr_smile_frame_position;

		ofVideoGrabber video_cam;
		FrameAnimation cam_size_animation;

		SceneMode icur_scene;
		SceneMode idest_scene;
		SceneBase* arr_scene[6];
		FrameAnimation timer_change_scene;
		FrameAnimation timer_scene_timeout;

		void loadScene();
		void loadSmileFrame();
		void initCamera();

		void initScene(SceneMode set_scene);

		bool upload_success;

		K2Face* k2face;

		HttpFormManager http_form_manager;

	
		//PhotoThread *photo_thread;
		ofImage arr_smile_img[3];
		void createPhoto(string id_str);
		void createQRCode(string url_str);
		void uploadPhoto(string id);
		
		string cur_id_str;
		void deletePhoto(string id);
		
		bool camera_paused;

		ofImage* created_img;

};


