#pragma once

#include "ofMain.h"
#include "HttpFormManager.h"
<<<<<<< HEAD
#include "ofxHttpUtils.h"
=======
>>>>>>> 353a6421e22b79be6ef1fa7a578bcb21c0df7d5c

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
		bool DEBUG_MODE;
		bool CRAZY_MODE;

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
		
		void setScoreEnable(bool set_enable);

		void setSmileFrame(int set_frame);
		void setSmileStage(int set_stage);
		void stopAllSmileFrame();
		int getFrameIndex(int iframe,int istage);

		void restartTimeout();

		bool uploadSuccess();
		void preparePhoto(bool create_new_id);
		void deleteCurPhoto();
		
<<<<<<< HEAD
		void newResponse(ofxHttpResponse &response);
=======
		void newResponse(HttpFormResponse &response);
>>>>>>> 353a6421e22b79be6ef1fa7a578bcb21c0df7d5c
	
		static int MAPP_FRAME;
		static int MTOTAL_FRAME;
		static int FRAME_PER_PAGE;
<<<<<<< HEAD
		
		void playSound(int type);

		bool webcam_fail;
		bool kinect_fail;

		float getDeltaMillis();
		void onSmileFrameLoopStart(int &param);

		void startBgm();

		void exit();
		
		void writeLog(string log_text,boolean send_slack);

		void uploadResponse(HttpFormResponse &response);
		HttpFormManager upload_manager;

=======
	
>>>>>>> 353a6421e22b79be6ef1fa7a578bcb21c0df7d5c
	private:
		
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
		
		FrameAnimation ani_sound_fade;
		bool sound_fade_in;

		void loadScene();
		void loadSmileFrame();
		void initCamera();

		void initScene(SceneMode set_scene);

		bool upload_success;

		K2Face* k2face;

<<<<<<< HEAD
		//HttpFormManager http_form_manager;
		ofxHttpUtils httpUtils;
=======
		HttpFormManager http_form_manager;

>>>>>>> 353a6421e22b79be6ef1fa7a578bcb21c0df7d5c
	
		//PhotoThread *photo_thread;
		ofImage arr_smile_img[3];
		void createPhoto(string id_str);
		void createQRCode(string url_str);
		void uploadPhoto(string id);
		string getCurFolder();


		string cur_id_str;
		void deletePhoto(string id);
		
		bool camera_paused;

		ofImage* created_img;

<<<<<<< HEAD
		ofSoundPlayer  sound_bgm,sound_button,sound_camera,sound_frame;
		void loadSound();

		float last_frame_millis,delta_frame_millis;
		
		ofTrueTypeFont font_debug;

		void sendSlackLog(string content);
=======
>>>>>>> 353a6421e22b79be6ef1fa7a578bcb21c0df7d5c
};


