
#include "ofApp.h"

#include "K2Face.h"
#include "iostream"


using namespace std;

//static float SMILE_DETECT_THRES[3]={0,.3,.6};



// define the face frame features required to be computed by this application
static const DWORD c_FaceFrameFeatures =
FaceFrameFeatures::FaceFrameFeatures_BoundingBoxInColorSpace
| FaceFrameFeatures::FaceFrameFeatures_PointsInColorSpace
| FaceFrameFeatures::FaceFrameFeatures_RotationOrientation
| FaceFrameFeatures::FaceFrameFeatures_Happy
| FaceFrameFeatures::FaceFrameFeatures_RightEyeClosed
| FaceFrameFeatures::FaceFrameFeatures_LeftEyeClosed
| FaceFrameFeatures::FaceFrameFeatures_MouthOpen
| FaceFrameFeatures::FaceFrameFeatures_MouthMoved
| FaceFrameFeatures::FaceFrameFeatures_LookingAway
| FaceFrameFeatures::FaceFrameFeatures_Glasses
| FaceFrameFeatures::FaceFrameFeatures_FaceEngagement;


K2Face::K2Face() :m_pBodyFrameReader(nullptr),
								m_pCoordinateMapper(NULL)
{
	
	/*LARGE_INTEGER qpf={0};
	if(QueryPerformanceFrequency(&qpf)){
		cout<<"PerFreq= "<<double(qpf.QuadPart)<<endl;
	}*/

	for(int i=0; i<BODY_COUNT; i++){
		m_pFaceFrameSources[i]=nullptr;
		m_pFaceFrameReaders[i]=nullptr;
	}
	
	
}

K2Face::~K2Face(){
	
	// done with face sources and readers
	for(int i=0; i<BODY_COUNT; i++){
		SafeRelease(m_pFaceFrameSources[i]);
		SafeRelease(m_pFaceFrameReaders[i]);
	}

	// done with body frame reader
	SafeRelease(m_pBodyFrameReader);
	SafeRelease(m_pCoordinateMapper);

	// close the Kinect Sensor
	if(pKinectSensor){
		pKinectSensor->Close();
	}

	SafeRelease(pKinectSensor);


}

void K2Face::Init(){

	iselected_frame=-1;
	HRESULT hr = InitK2Sensor();
	
}

void K2Face::Update(){
	
	if(!m_pBodyFrameReader){
		return;
	}
	
	IBodyFrame* pBodyFrame = nullptr;
	HRESULT	hr = m_pBodyFrameReader->AcquireLatestFrame(&pBodyFrame);

	IBody* ppBodies[BODY_COUNT] = { 0 };

	if(SUCCEEDED(hr)){
		hr = pBodyFrame->GetAndRefreshBodyData(BODY_COUNT, ppBodies);
		if (SUCCEEDED(hr)){
			ProcessFace(BODY_COUNT,ppBodies);
		}
		
		for(int i = 0; i<_countof(ppBodies); ++i){
				SafeRelease(ppBodies[i]);
		}
		
	}
	SafeRelease(pBodyFrame);

	

	auto it=map_tracked_face.begin();
	while(it!=map_tracked_face.end()){
		if(it->second.hasLostTrack()) it=map_tracked_face.erase(it);
		else{
			it->second.update();
			it++;
		}
	}


}

HRESULT K2Face::InitK2Sensor(){

	cout << "Init Kinect..." << endl;

	HRESULT hr;

	IBodyFrameSource* pBodyFrameSource = nullptr;

	hr=GetDefaultKinectSensor(&pKinectSensor);
	if(FAILED(hr)) return hr;
	
	cout << "Kinect OK!" << endl;

	hr=pKinectSensor->Open();
	if(SUCCEEDED(hr)){
		hr = pKinectSensor->get_CoordinateMapper(&m_pCoordinateMapper);
	}
	if(SUCCEEDED(hr)){
		hr=pKinectSensor->get_BodyFrameSource(&pBodyFrameSource);
	}
	if(SUCCEEDED(hr)){
		hr=pBodyFrameSource->OpenReader(&m_pBodyFrameReader);
	}
	cout << "Kinect Sensor OK!" << endl;

	if(SUCCEEDED(hr)){
		// create a face frame source + reader to track each body in the fov
		for(int i=0;i<BODY_COUNT;i++){
			if(SUCCEEDED(hr)){
				// create the face frame source by specifying the required face frame features
				hr = CreateFaceFrameSource(pKinectSensor, 0, c_FaceFrameFeatures, &m_pFaceFrameSources[i]);
			}
			if(SUCCEEDED(hr)){
				// open the corresponding reader
				hr = m_pFaceFrameSources[i]->OpenReader(&m_pFaceFrameReaders[i]);
			}
		}
	}

	SafeRelease(pBodyFrameSource);

	if(!pKinectSensor || FAILED(hr)){
		cout<<"No ready Kinect found!"<<endl;
		return E_FAIL;
	}

	return hr;
}
HRESULT K2Face::UpdateBodyData(IBody** ppBodies){
	HRESULT hr=E_FAIL;

	if(m_pBodyFrameReader!=nullptr){
		IBodyFrame* pBodyFrame=nullptr;
		hr=m_pBodyFrameReader->AcquireLatestFrame(&pBodyFrame);
		if(SUCCEEDED(hr)){
			hr = pBodyFrame->GetAndRefreshBodyData(BODY_COUNT, ppBodies);
		}
		SafeRelease(pBodyFrame);
	}

	return hr;
}

void K2Face::ProcessFace(int nBodyCount, IBody** ppBodies){

	HRESULT hr;
	

	// iterate through each face reader
	for(int iFace=0; iFace<BODY_COUNT; ++iFace){

		// retrieve the latest face frame from this reader
		IFaceFrame* pFaceFrame=nullptr;
		hr=m_pFaceFrameReaders[iFace]->AcquireLatestFrame(&pFaceFrame);

		BOOLEAN bFaceTracked=false;
		if(SUCCEEDED(hr)&& nullptr!=pFaceFrame){
			// check if a valid face is tracked in this face frame
			hr=pFaceFrame->get_IsTrackingIdValid(&bFaceTracked);
		}
		
	

		if(SUCCEEDED(hr)){

			// 1. get head pos
			Joint joints[JointType_Count];
			float screenPointX=0;
			float screenPointY=0;
			float camera_distance=0;
			IBody* pBody = ppBodies[iFace];
			
			if(pBody){


				BOOLEAN bTracked = false;
				hr = pBody->get_IsTracked(&bTracked);
				
				if (SUCCEEDED(hr) && bTracked){
				
					hr = pBody->GetJoints(_countof(joints), joints);
					if (SUCCEEDED(hr)){
					
						CameraSpacePoint head_point = joints[JointType::JointType_Head].Position;
						float* screen_point = BodyToScreen(head_point);
						screenPointX = screen_point[0];
						screenPointY = screen_point[1];
						camera_distance=pow(head_point.X,2)+pow(head_point.Y,2)+pow(head_point.Z,2);
					}
				}
			}

			// 2. get face 
			if (bFaceTracked){
				IFaceFrameResult* pFaceFrameResult = nullptr;
				RectI faceBox = { 0 };
				PointF facePoints[FacePointType::FacePointType_Count];
				Vector4 faceRotation;
				DetectionResult faceProperties[FaceProperty::FaceProperty_Count];
				//D2D1_POINT_2F faceTextLayout;
				UINT64 tracking_id;
				
				hr = pFaceFrame->get_FaceFrameResult(&pFaceFrameResult);

				// need to verify if pFaceFrameResult contains data before trying to access it
				if(SUCCEEDED(hr) && pFaceFrameResult != nullptr){
					hr=pFaceFrameResult->get_FaceBoundingBoxInColorSpace(&faceBox);

					if(SUCCEEDED(hr)){
						hr=pFaceFrameResult->GetFacePointsInColorSpace(FacePointType::FacePointType_Count, facePoints);
					}

					if(SUCCEEDED(hr)){
						hr=pFaceFrameResult->get_FaceRotationQuaternion(&faceRotation);
					}

					if(SUCCEEDED(hr)){
						hr=pFaceFrameResult->GetFaceProperties(FaceProperty::FaceProperty_Count, faceProperties);
					}
					if (SUCCEEDED(hr)){
						hr = pFaceFrame->get_TrackingId(&tracking_id);
					}
					
					if(SUCCEEDED(hr)){
						//cout << "Get Face Result: " << tracking_id << endl;
						float score=CalculateSmileScore(tracking_id,&faceBox, facePoints, faceProperties);
						
						updateFaceData(tracking_id,screenPointX,screenPointY, &faceBox, score,camera_distance);

					}
				}

				SafeRelease(pFaceFrameResult);

			}else{
				
				
				// face tracking is not valid - attempt to fix the issue
				// a valid body is required to perform this step
				//if(bHaveBodyData){
					
					// check if the corresponding body is tracked 
					// if this is true then update the face frame source to track this body
				
					if(pBody!=nullptr){
						BOOLEAN bTracked=false;
						hr=pBody->get_IsTracked(&bTracked);

						UINT64 bodyTId;
						if(SUCCEEDED(hr) && bTracked){
							// get the tracking ID of this body
							hr=pBody->get_TrackingId(&bodyTId);
							if(SUCCEEDED(hr)){
								// update the face frame source with the tracking ID
								m_pFaceFrameSources[iFace]->put_TrackingId(bodyTId);
								
							}
						}
					}
				//}
			}
		}

		SafeRelease(pFaceFrame);
	}

	
}


void K2Face::updateFaceData(const UINT64 tracking_id, float head_pos_x,float head_pos_y,const RectI* pFaceBox, float smile_score,float camera_dist){
	
	float wid = pFaceBox->Right - pFaceBox->Left;
	float hei = pFaceBox->Bottom - pFaceBox->Top;

	/*cout << "head_pos =" << head_pos_x << " , " << head_pos_y 
		 << " face_pos = " << pFaceBox->Left << " , " << pFaceBox->Top << endl;*/
	
	// out of detection range
	if(head_pos_x>1920.0*global_param->Kinect_Right_x || head_pos_x<1920.0*global_param->Kinect_Left_x){ 
		head_pos_x=0;  head_pos_y=0;
	}


	ofPoint scaled_pos=ofPoint(head_pos_x,head_pos_y);
	scaled_pos.x*=global_param->Kinect_Scale_x;
	scaled_pos.y*=global_param->Kinect_Scale_y;
		
	scaled_pos.x+=global_param->Kinect_Position_x;
	scaled_pos.y+=global_param->Kinect_Position_y;

	ofPoint scaled_sizee=ofPoint(wid,hei);
	scaled_sizee.x*=global_param->Kinect_Scale_x;
	scaled_sizee.y*=global_param->Kinect_Scale_y;

	int happy_stage=(smile_score>=global_param->Smile_Detect_Thres[2])?2:((smile_score>=global_param->Smile_Detect_Thres[1])?1:0);

	if(map_tracked_face.find(tracking_id)==map_tracked_face.end()){
		map_tracked_face[tracking_id]=TrackedFace(scaled_pos,scaled_sizee,happy_stage,global_param,camera_dist);
		
	}else{
		map_tracked_face[tracking_id].updateGeometry(scaled_pos,scaled_sizee,camera_dist);
		map_tracked_face[tracking_id].updateHappyStage(happy_stage);
	}

	float tmp_score=(map_tracked_face[tracking_id].getCurScore())*global_param->Smile_Score_Weight;
	int mface=map_tracked_face.size();
	if(mface>1) tmp_score/=(mface/1.2);
	total_smile_score+=tmp_score;
	
}


float K2Face::CalculateSmileScore(const UINT64 tracking_id, const RectI* pFaceBox, const PointF* pFacePoints, const DetectionResult* pFaceProperties){

	float ishappy = ((int)pFaceProperties[FaceProperty::FaceProperty_Happy] == 3 ? 1.0f : 0.0f);
	float ismouth_move = ((int)pFaceProperties[FaceProperty::FaceProperty_MouthMoved] == 3 ? 1.0f : 0.0f);
	float ismouth_open = ((int)pFaceProperties[FaceProperty::FaceProperty_MouthOpen] == 3 ? 1.0f : 0.0f);
	

	return (ishappy*4.0f+ismouth_move+ismouth_open*2.5)/4.0f;

}

float* K2Face::BodyToScreen(const CameraSpacePoint& body_point){

	
	//cout << "head_point= " << body_point.X << ", " << body_point.Y << endl;

	DepthSpacePoint depthPoint = { 0 };
	m_pCoordinateMapper->MapCameraPointToDepthSpace(body_point, &depthPoint);
	
	//cout << "depth_point= " << depthPoint.X << ", " << depthPoint.Y << endl;


	float screenPointX = static_cast<float>(depthPoint.X * 1920) / 512;
	float screenPointY = static_cast<float>(depthPoint.Y * 1080) / 424;
	
	float location[]={screenPointX, screenPointY};
	return location;
}


vector<TrackedFace> K2Face::getTrackedFace(){
	
	
	vector<TrackedFace> cur_face;
	auto it=map_tracked_face.begin();
	for(;it!=map_tracked_face.end();it++){
		cur_face.push_back(it->second);
	}
	int max_face=map_tracked_face.size();
	if(iselected_frame>=0 && iselected_frame<4) max_face=global_param->Kinect_Face_Limit[iselected_frame];
	
	if(max_face<map_tracked_face.size()){
		sort(cur_face.begin(),cur_face.end(),
			[] (TrackedFace const& a, TrackedFace const& b) { return a.camera_distance < b.camera_distance;});	
		cur_face.resize(max_face);
	}

	return cur_face;
}

float K2Face::getTotalSmileScore(){
	return total_smile_score;
}

void K2Face::Reset(){

	total_smile_score=0;
	map_tracked_face.clear();

}

void K2Face::setGlobalParam(GlobalParam* set_param){
	global_param=set_param;
}

void K2Face::setSelectedFrame(int set_select){
	iselected_frame=set_select;

}