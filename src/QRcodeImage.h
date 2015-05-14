
#ifndef QRCODE_IMAGE
#define QRCODE_IMAGE

#include "ofURLFileLoader.h"
#include "ofImage.h"
#include <string>
#include <sstream>

class QRcodeImage:public ofImage{
public:
	void fetch(const std::string& data, size_t size, size_t margin){
		std::stringstream googleChartsQRurl;
		googleChartsQRurl
		<< "http://chart.googleapis.com/chart?"   // Google Charts Endpoint
		<< "chs=" << size << "x" << size << "&"   // chart size
		<< "cht=qr&"                              // chart type: QR Code
		<< "chld=L|" << margin << "&"             // Error correction [L,M,Q,H], margin
		<< "choe=UTF-8&"                          // Encoding
		<< "chl=" << data;                        // QR code data
  
		ofRegisterURLNotification(this);
		ofLoadURLAsync(googleChartsQRurl.str(), "qrcode");
	}

	void urlResponse(ofHttpResponse& response){
		if(response.request.name == "qrcode"){
			if(response.status == 200) loadImage(response.data);
			ofUnregisterURLNotification(this);
		}
	}


};


#endif