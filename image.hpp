// vim: set ts=4:
#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <Magick++.h>


enum GI_ERR
{
	GI_ERR_SUCCESS   = 1,
	GI_ERR_ERROR     = GI_ERR_SUCCESS   <<1,
	GI_ERR_OUTPUT    = GI_ERR_ERROR     <<1,
	GI_ERR_OUTSPEED  = GI_ERR_OUTPUT    <<1,
	GI_ERR_OUTCOURSE = GI_ERR_OUTSPEED  <<1,
	GI_ERR_LAYER     = GI_ERR_OUTCOURSE <<1,
	GI_ERR_FONT      = GI_ERR_LAYER     <<1
};




class GpxImage
{
	private:
	Magick::Image mSpeedBackg;
	Magick::Image mSpeedGauge;
	Magick::Image mSpeedArrow;
	Magick::Image mCourseBackg;
	Magick::Image mCourseGauge;
	Magick::Image mCourseArrow;
	
	Magick::Color mTransp;
	char mSize[32] = "256x256";
	
	Magick::Image* mpB;
	Magick::Image* mpG;
	Magick::Image* mpA;
	Magick::Image mT; // temorary for rotation
	Magick::Image mI;
	char mLayerDef[7] = "";
	float mValue = 0.0;      // plain Value
	float mValueMax = 0.0;   // max value on scale/gauge-graphic
	float mValueScale = 0.0; // angle on scale from 0 to max
	float mValueFac = 1.0;   // conversion factor from ms to kmh/mph/knt
	float mValueConv = 0.0;  // converted value ie. value in kmh
	float mValueDeg = 0.0;   // calculated rotation in deg dep. on Conv/Max/Scale
	char mText[64] = "";
	int mTextSize = 20;
	int mTextX = 128;
	int mTextY = 128;
	char mFont[128] = "";
	char mFontColor[32] = "";
	char mCropDef[32] = "";
	
	public:
	GpxImage();
	~GpxImage();
	GI_ERR init( bool speed, bool course );
	void src( Magick::Image& refBackg, Magick::Image& refGauge, Magick::Image& refArrow );
	void value( char* value, float max, float scaleSize );
	void setKmh();
	void setMph();
	void setKnt();
	void setRaw();
	GI_ERR layer( char layerDef[] );
	void text( char text[], int fontSize, int x, int y, const char color[]=NULL );
	GI_ERR setFontTTF( const char font[] );
	GI_ERR setFontColor( const char color[] );
	void crop( char cropDef[] );
	GI_ERR save( char filename[] );
	
	void speedImage();
	void courseImage();
};


#endif
// EOF
