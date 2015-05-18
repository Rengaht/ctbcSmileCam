#ifndef GLOBAL_PARAM
#define GLOBAL_PARAM

#include "ofMain.h"
#include "ofxXmlSettings.h"

const static string PARAMETER_FILE_PATH="smilecam_param_file.xml";

class GlobalParam{

public:

	int Scene_Timeout;
	
	int* Kinect_Face_Limit;

	float Kinect_Position_x;
	float Kinect_Position_y;
	float Kinect_Scale_x;
	float Kinect_Scale_y;
	float Kinect_Left_x;
	float Kinect_Right_x;
	float Smile_Detect_Thres[3];//={0,.3,.6};

	int mLost_Frame;//=3;
	int Smooth_Frame;//=4;
	float Smile_Stage_Score[3];//={0,.5,1};

	string Created_File_Path;//="created/";
	string Created_File_Title;//="smilecam_";


	GlobalParam(){
		readParameterFile();
		Kinect_Face_Limit=new int[4];
		Kinect_Face_Limit[0]=2;
		Kinect_Face_Limit[1]=1;
		Kinect_Face_Limit[2]=3;
		Kinect_Face_Limit[3]=1;

	}

	void readParameterFile(){

		ofxXmlSettings _param;
		bool file_exist=true;
		if(_param.loadFile(PARAMETER_FILE_PATH) ){
			ofLog()<<"mySettings.xml loaded!";
		}else{
			ofLog()<<"unable to load xml check data/ folder";
			file_exist=false;
		}

		Kinect_Position_x=_param.getValue("KINECT_POS_X",-480);
		Kinect_Position_y=_param.getValue("KINECT_POS_Y",0);

		Kinect_Scale_x=_param.getValue("KINECT_SCALE_X",1.0);
		Kinect_Scale_y=_param.getValue("KINECT_SCALE_Y",1.0);


		ofLog()<<"KINECT PARAM: "<<Kinect_Position_x<<" , "<<Kinect_Position_y<<" , "
				<<Kinect_Scale_x<<" , "<<Kinect_Scale_y;

		Kinect_Left_x=_param.getValue("KINECT_LEFT_X",0.0);
		Kinect_Right_x=_param.getValue("KINECT_RIGHT_X",1.0);
		


		Smile_Detect_Thres[0]=_param.getValue("DETECT_THRES_0",0);
		Smile_Detect_Thres[1]=_param.getValue("DETECT_THRES_1",.3);
		Smile_Detect_Thres[2]=_param.getValue("DETECT_THRES_2",.6);

		mLost_Frame=_param.getValue("MLOST_FRAME",3);//=3;
		Smooth_Frame=_param.getValue("SMOOTH_FRAME",4);//=4;
		
		Smile_Stage_Score[0]=_param.getValue("SMILE_SCORE_0",0);//={0,.5,1};
		Smile_Stage_Score[1]=_param.getValue("SMILE_SCORE_1",.5);
		Smile_Stage_Score[2]=_param.getValue("SMILE_SCORE_2",1);

		Created_File_Path=_param.getValue("CREATED_FILE_PATH","created/");
		Created_File_Title=_param.getValue("CREATED_FILE_TITLE","smilecam_");
		
		Scene_Timeout=_param.getValue("SCENE_TIMEOUT",3600);

		if(!file_exist) saveParameterFile();
	}
	
	void saveParameterFile(){
		

		ofxXmlSettings _param;
		
		_param.setValue("KINECT_POS_X",Kinect_Position_x);
		_param.setValue("KINECT_POS_Y",Kinect_Position_y);

		_param.setValue("KINECT_SCALE_X",Kinect_Scale_x);
		_param.setValue("KINECT_SCALE_Y",Kinect_Scale_y);

		_param.setValue("KINECT_LEFT_X",Kinect_Left_x);
		_param.setValue("KINECT_RIGHT_X",Kinect_Right_x);

		_param.setValue("DETECT_THRES_0",Smile_Detect_Thres[0]);
		_param.setValue("DETECT_THRES_1",Smile_Detect_Thres[1]);
		_param.setValue("DETECT_THRES_2",Smile_Detect_Thres[2]);

		_param.setValue("MLOST_FRAME",mLost_Frame);
		_param.setValue("SMOOTH_FRAME",Smooth_Frame);
		
		_param.setValue("SMILE_SCORE_0",Smile_Stage_Score[0]);
		_param.setValue("SMILE_SCORE_1",Smile_Stage_Score[1]);
		_param.setValue("SMILE_SCORE_2",Smile_Stage_Score[2]);
		
		_param.setValue("CREATED_FILE_PATH",Created_File_Path);
		_param.setValue("CREATED_FILE_TITLE",Created_File_Title);

		_param.setValue("SCENE_TIMEOUT",Scene_Timeout);

		_param.save(PARAMETER_FILE_PATH);

	}



};

#endif