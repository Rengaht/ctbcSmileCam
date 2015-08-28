
#ifndef K2_FACE
#define K2_FACE

//#include <windows.h>
// Kinect Header files

#include "TrackedFace.h"
#include "GlobalParam.h"

#include <Kinect.h>
#include <Kinect.Face.h>

#define ADDRESS "127.0.0.1"
#define PORT "7000"
#define OUTPUT_BUFFER_SIZE 1024


class K2Face{


	public:
		
		K2Face();
		~K2Face();

		
		void Update();
		void Init();
		
		void Reset();
		vector<TrackedFace> getTrackedFace();
		float getTotalSmileScore();

		void setGlobalParam(GlobalParam* set_param);
		void setSelectedFrame(int set_select);

	private:

		GlobalParam* global_param;

		float total_smile_score;

		//void Update();
		HRESULT InitK2Sensor();
		
		void ProcessFace(int nBodyCount, IBody** ppBodies);
		HRESULT UpdateBodyData(IBody** ppBodies);

		
		void updateFaceData(const UINT64 tracking_id, float head_pos_x,float head_pos_y,const RectI* pFaceBox, float smile_score,float camera_dist);

		float CalculateSmileScore(const UINT64 tracking_id, const RectI* pFaceBox, const PointF* pFacePoints, const DetectionResult* pFaceProperties);

		// Current Kinect
		IKinectSensor* pKinectSensor;

		// Body reader
		IBodyFrameReader*      m_pBodyFrameReader;

		// Face sources
		IFaceFrameSource*	   m_pFaceFrameSources[BODY_COUNT];

		// Face readers
		IFaceFrameReader*	   m_pFaceFrameReaders[BODY_COUNT];


		ICoordinateMapper*      m_pCoordinateMapper;

		
		float* BodyToScreen(const CameraSpacePoint& bodyPoint);

		map<Poco::UInt64,TrackedFace> map_tracked_face;
		int iselected_frame;

};

// Safe release for interfaces
template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
	if (pInterfaceToRelease != nullptr)
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = nullptr;
	}
}


#endif