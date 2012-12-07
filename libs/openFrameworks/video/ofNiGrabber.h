#pragma once

#include "ofConstants.h"
#include "ofTexture.h"
#include "ofBaseTypes.h"
#include "ofPixels.h"


#include "oni/OpenNI.h"


class ofNiGrabber : public ofBaseVideoGrabber{

	public :

		ofNiGrabber();
		virtual ~ofNiGrabber();

		void					listDevices();
		bool					initGrabber(int w, int h);
		void					update();
		bool					isFrameNew();
		
		bool					setPixelFormat(ofPixelFormat pixelFormat);
		ofPixelFormat			getPixelFormat();		

		unsigned char		* 	getPixels();
		ofPixelsRef				getPixelsRef();
		
		void					close();
		void					clearMemory();

		void					videoSettings();
		
		float					getWidth();
		float					getHeight();

		void					setVerbose(bool bTalkToMe);
		void					setDeviceID(int _deviceID);
		void					setDesiredFrameRate(int framerate);

		void draw(int x, int y);
		void draw(){draw(0,0);}
		

	protected:
	
		bool					bChooseDevice;
		int						deviceID;
		bool 					bVerbose;
		bool 					bGrabberInited;
	    ofPixels		 		pixels;
		int						attemptFramerate;
		bool 					bIsFrameNew;	
		
		int						width, height;	
		//--------------------------------- directshow
		#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
			int 					device;
			//videoInput 				VI;
			bool 					bDoWeNeedToResize;
		#endif	
};

