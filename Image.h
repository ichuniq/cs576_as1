//*****************************************************************************
//
// Image.h : Defines the class operations on images
//
// Author - Parag Havaldar
// Main Image class structure 
//
//*****************************************************************************

#ifndef IMAGE_DISPLAY
#define IMAGE_DISPLAY

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "afxwin.h"

// C RunTime Header Files
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// Class structure of Image 
// Use to encapsulate an RGB image
class MyImage 
{

private:
	int		Width;					// Width of Image
	int		Height;					// Height of Image
	char	ImagePath[_MAX_PATH];	// Image location

	int		n_pixels;

	unsigned char*	Data;			// image in BGR format (for display); format: [BGR][BGR] ...
	

	// modified
	float* YUV_Data;				// YUV data of the image; format: [YYY...n_pix][UUU...n_pix][VVV...n_pix]
	float* YUV_Data_Smp;			// YUV data after sampling (downsample + upsample)


public:
	// Constructor
	MyImage();
	// Copy Constructor
	MyImage::MyImage( MyImage *otherImage);
	// Destructor
	~MyImage();

	// operator overload
	MyImage & operator= (const MyImage & otherImage);

	// Reader & Writer functions
	void	setWidth( const int w)  { Width = w; }; 
	void	setHeight(const int h) { Height = h; }; 
	void	setImageData( const char *img ) { Data = (unsigned char *)img; };
	void	setImagePath( const char *path) { strcpy_s(ImagePath, path); }
	int		getWidth() { return Width; };
	int		getHeight() { return Height; };
	char*	getImagePath() { return ImagePath; }
	unsigned char* getImageData() { return Data; };
	float* getYUVData() { return YUV_Data;  };
	float* getYUVDataSmp() { return YUV_Data_Smp; };

	// Input Output operations
	bool	ReadImage();
	bool	WriteImage();

	// Modifications
	bool	Modify();

	bool RGB2YUV();

	bool YUV2RGB();

	bool helper_subsample(int step, int offset);
	bool SubSampling(int step_y,int step_u, int step_v);

	bool helper_upsample(int step, int offset);
	bool UpSampling(int step_y, int step_u, int step_v);

	bool Scaling();
	bool Antialiasing();

};

#endif //IMAGE_DISPLAY
