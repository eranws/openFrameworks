#include "ofNiGrabber.h"
#include "ofUtils.h"
#ifdef TARGET_WIN32

using namespace openni;
//using openni::OpenNI;

//--------------------------------------------------------------------
ofNiGrabber::ofNiGrabber(){

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
	//---------------------------------

		bVerbose 			= false;
		bDoWeNeedToResize 	= false;

	//---------------------------------
	#endif
	//---------------------------------

	// common
	bIsFrameNew				= false;
	bVerbose 				= false;
	bGrabberInited 			= false;
	bChooseDevice			= false;
	deviceID				= 0;
	width 					= 320;	// default setting
	height 					= 240;	// default setting
	attemptFramerate		= -1;
}


//--------------------------------------------------------------------
ofNiGrabber::~ofNiGrabber(){
	close();
}


//--------------------------------------------------------------------
bool ofNiGrabber::initGrabber(int w, int h){

	Status rc = OpenNI::initialize();
	if (rc != STATUS_OK)
	{
		printf("Initialize failed\n%s\n", OpenNI::getExtendedError());
		return false;
	}

	Device device;
	rc = device.open(ANY_DEVICE);
	if (rc != STATUS_OK)
	{
		printf("Couldn't open device\n%s\n", OpenNI::getExtendedError());
		return false;
	}

	VideoStream depth;

	if (device.getSensorInfo(SENSOR_DEPTH) != NULL)
	{
		rc = depth.create(device, SENSOR_DEPTH);
		if (rc != STATUS_OK)
		{
			printf("Couldn't create depth stream\n%s\n", OpenNI::getExtendedError());
			return false;
		}
	}

	rc = depth.start();
	if (rc != STATUS_OK)
	{
		printf("Couldn't start the depth stream\n%s\n", OpenNI::getExtendedError());
		return false;
	}

//	VideoFrameRef frame;

	
	if (rc == STATUS_OK)
	{
		width = w;
		height = h;
		bGrabberInited = false;

		if( attemptFramerate >= 0){
			//TODO VI.setIdealFramerate(device, attemptFramerate);
		}
		//bool bOk = VI.setupDevice(device, width, height);

/* DX ?? 
int ourRequestedWidth = width;
		int ourRequestedHeight = height;

			bGrabberInited = true;
			width 	= VI.getWidth(device);
			height 	= VI.getHeight(device);

			if (width == ourRequestedWidth && height == ourRequestedHeight){
				bDoWeNeedToResize = false;
			} else {
				bDoWeNeedToResize = true;
				width = ourRequestedWidth;
				height = ourRequestedHeight;
			}
			*/

			pixels.allocate(width, height, 3);
			return true;
		} else {
			ofLog(OF_LOG_ERROR, "error allocating a video device");
			ofLog(OF_LOG_ERROR, "please check your camera with AMCAP or other software");
			bGrabberInited = false;
			return false;
		}

}

//---------------------------------------------------------------------------
bool ofNiGrabber::setPixelFormat(ofPixelFormat pixelFormat){
	//TODO: DEPTH!!!
	//note as we only support RGB we are just confirming that this pixel format is supported
	if( pixelFormat == OF_PIXELS_RGB ){
		return true;
	}
	ofLogWarning("ofNiGrabber") << "requested pixel format not supported" << endl;
	return false;
}

//---------------------------------------------------------------------------
ofPixelFormat ofNiGrabber::getPixelFormat(){
	//note if you support more than one pixel format you will need to return a ofPixelFormat variable. 
	//TODO DEPTH!!!!
#pragma warning (0)
	return OF_PIXELS_RGB;
}

void ofNiGrabber::draw( int x, int y )
{
	printf("DRAW!");
}


//--------------------------------------------------------------------
void ofNiGrabber::listDevices(){
#if 0
	//---------------------------------
	//	#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
	//---------------------------------
		ofLog(OF_LOG_NOTICE, "---");

	Array<DeviceInfo>* deviceInfoList;
	OpenNI::enumerateDevices(deviceInfoList);
	
	for (int i = 0; i < deviceInfoList->getSize(); i++)
	{
	//const openni:DeviceInfo& deviceInfo = deviceInfoList->operator[](i);
		ofLog(OF_LOG_NOTICE, "%s\n%s\n%i\n%i\n%s\n",
		deviceInfoList[i].getName(),
		deviceInfoList[i].getUri(),
		deviceInfoList[i].getUsbProductId(),
		deviceInfoList[i].getUsbVendorId(), 
		deviceInfoList[i].getVendor ()
	)

	/*
const char *	getName () const 
const char *	getUri () const 
uint16_t		getUsbProductId () const 
uint16_t		getUsbVendorId () const 
const char *	getVendor () const 
*/

	ofLog(OF_LOG_NOTICE, "---");

	//---------------------------------
	//#endif
	//---------------------------------
#endif
}

//--------------------------------------------------------------------
void ofNiGrabber::update(){

#if 0
	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
	//---------------------------------

		if (bGrabberInited == true){
			bIsFrameNew = false;
			if (VI.isFrameNew(device)){

				bIsFrameNew = true;


				/*
				 	rescale --
				 	currently this is nearest neighbor scaling
				 	not the greatest, but fast
				 	this can be optimized too
				 	with pointers, etc

				 	better --
				 	make sure that you ask for a "good" size....

				*/

				unsigned char * viPixels = VI.getPixels(device, true, true);


				if (bDoWeNeedToResize == true){

					int inputW = VI.getWidth(device);
					int inputH = VI.getHeight(device);

					float scaleW =	(float)inputW / (float)width;
					float scaleH =	(float)inputH / (float)height;

					for(int i=0;i<width;i++){
						for(int j=0;j<height;j++){

							float posx = i * scaleW;
							float posy = j * scaleH;

							/*

							// start of calculating
							// for linear interpolation

							int xbase = (int)floor(posx);
							int xhigh = (int)ceil(posx);
							float pctx = (posx - xbase);

							int ybase = (int)floor(posy);
							int yhigh = (int)ceil(posy);
							float pcty = (posy - ybase);
							*/

							int posPix = (((int)posy * inputW * 3) + ((int)posx * 3));

							pixels.getPixels()[(j*width*3) + i*3    ] = viPixels[posPix  ];
							pixels.getPixels()[(j*width*3) + i*3 + 1] = viPixels[posPix+1];
							pixels.getPixels()[(j*width*3) + i*3 + 2] = viPixels[posPix+2];

						}
					}

				} else {

					pixels.setFromPixels(viPixels,width,height,OF_IMAGE_COLOR);

				}


			}
		}

	//---------------------------------
	#endif
	//---------------------------------

#endif

}

//--------------------------------------------------------------------
void ofNiGrabber::close(){

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
	//---------------------------------

		if (bGrabberInited == true){
			OpenNI::shutdown();
//			VI.stopDevice(device);
			bGrabberInited = false;
		}

	//---------------------------------
	#endif
	//---------------------------------

	clearMemory();

}


//--------------------------------------------------------------------
void ofNiGrabber::clearMemory(){
	pixels.clear();
}

//---------------------------------------------------------------------------
unsigned char * ofNiGrabber::getPixels(){
	return pixels.getPixels();
}

//---------------------------------------------------------------------------
ofPixelsRef ofNiGrabber::getPixelsRef(){
	return pixels;
}

//--------------------------------------------------------------------
float ofNiGrabber::getWidth(){
	return width;
}

//--------------------------------------------------------------------
float ofNiGrabber::getHeight(){
	return height;
}

//---------------------------------------------------------------------------
bool  ofNiGrabber::isFrameNew(){
	return bIsFrameNew;
}

//--------------------------------------------------------------------
void ofNiGrabber::setVerbose(bool bTalkToMe){
	bVerbose = bTalkToMe;
}

//--------------------------------------------------------------------
void ofNiGrabber::setDeviceID(int _deviceID){
	deviceID		= _deviceID;
	bChooseDevice	= true;
}

//--------------------------------------------------------------------
void ofNiGrabber::setDesiredFrameRate(int framerate){
	attemptFramerate = framerate;
}


//--------------------------------------------------------------------
void ofNiGrabber::videoSettings(void){

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
	//---------------------------------

//		if (bGrabberInited == true) VI.showSettingsWindow(device);

	//---------------------------------
	#endif
	//---------------------------------
}

#endif
