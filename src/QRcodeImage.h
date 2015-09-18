
#ifndef QRCODE_IMAGE
#define QRCODE_IMAGE

#include "ofURLFileLoader.h"
#include "ofImage.h"
#include <string>
#include <sstream>

class QRcodeImage:public ofImage{
public:
	bool loaded;
	
	void fetch(const std::string& data, size_t size, size_t margin){
		std::stringstream googleChartsQRurl;
		googleChartsQRurl
		<< "http://chart.googleapis.com/chart?"   // Google Charts Endpoint
		<< "chs=" << size << "x" << size << "&"   // chart size
		<< "cht=qr&"                              // chart type: QR Code
		<< "chld=L|" << margin << "&"             // Error correction [L,M,Q,H], margin
		<< "choe=UTF-8&"                          // Encoding
		<< "chl=" << data;                        // QR code data
	
		loaded=false;
		ofRegisterURLNotification(this);
		int request_id=ofLoadURLAsync(googleChartsQRurl.str(), "qrcode");
		//return request_id;
		 
		//ofHttpResponse resp=ofLoadURL(googleChartsQRurl.str());
		//if(resp.status == 200){
		//	loadImage(resp.data);
		//	loaded=true;
		//	//return true;
		//}
		//return false;
	}

	void urlResponse(ofHttpResponse& response){
		if(response.request.name == "qrcode" && response.status == 200){
			loadImage(response.data);
			loaded=true;
			
		}else{
			ofLog()<< response.status << " " << response.error << endl;
		}
		ofUnregisterURLNotification(this);
	}

	
};


#endif