//*****************************************************************************
//
// Image.cpp : Defines the class operations on images
//
// Author - Parag Havaldar
// Code used by students as starter code to display and modify images
//
//*****************************************************************************

#include "Image.h"


// Constructor and Desctructors
MyImage::MyImage() 
{
	Data = NULL;
	Width = -1;
	Height = -1;
	ImagePath[0] = 0;
}

MyImage::~MyImage()
{
	if (Data)
		delete Data;
	if (YUV_Data)
		delete YUV_Data;
}


// Copy constructor
MyImage::MyImage( MyImage *otherImage)
{
	Height = otherImage->Height;
	Width  = otherImage->Width;
	Data   = new unsigned char[Width*Height*3];
	strcpy(otherImage->ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*3); i++ )
	{
		Data[i]	= otherImage->Data[i];
	}


}



// = operator overload
MyImage & MyImage::operator= (const MyImage &otherImage)
{
	Height = otherImage.Height;
	Width  = otherImage.Width;
	Data   = new unsigned char[Width*Height*3];
	strcpy( (char *)otherImage.ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*3); i++ )
	{
		Data[i]	= otherImage.Data[i];
	}
	
	return *this;

}


// MyImage::ReadImage
// Function to read the image given a path
bool MyImage::ReadImage()
{

	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		fprintf(stderr, "Usage is `Image.exe Imagefile w h`");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *IN_FILE;
	IN_FILE = fopen(ImagePath, "rb");
	if ( IN_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Reading");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	for (i = 0; i < Width*Height; i ++)
	{
		Rbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Gbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Bbuf[i] = fgetc(IN_FILE);
	}
	
	// Allocate Data structure and copy
	Data = new unsigned char[Width*Height*3];
	for (i = 0; i < Height*Width; i++)
	{
		Data[3*i]	= Bbuf[i];
		Data[3*i+1]	= Gbuf[i];
		Data[3*i+2]	= Rbuf[i];
	}

	// Clean up and return
	delete [] Rbuf;
	delete [] Gbuf;
	delete [] Bbuf;

	fclose(IN_FILE);

	return true;

}



// MyImage functions defined here
bool MyImage::WriteImage()
{
	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *OUT_FILE;
	OUT_FILE = fopen(ImagePath, "wb");
	if ( OUT_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Writing");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	for (i = 0; i < Height*Width; i++)
	{
		Bbuf[i] = Data[3*i];
		Gbuf[i] = Data[3*i+1];
		Rbuf[i] = Data[3*i+2];
	}

	
	// Write data to file
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Rbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Gbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Bbuf[i], OUT_FILE);
	}
	
	// Clean up and return
	delete [] Rbuf;
	delete [] Gbuf;
	delete [] Bbuf;
	fclose(OUT_FILE);

	return true;

}




// Here is where you would place your code to modify an image
// eg Filtering, Transformation, Cropping, etc.
bool MyImage::Modify()
{

	// TODO by student

	// sample operation
	for (int i = 0; i < Width * Height; i++) {
		Data[3*i] = 0;
		Data[3*i+1] = 0;
		Data[3 * i + 2] = 0;
	}

	return false;
}


/* convert to YUV space */
bool MyImage::RGB2YUV() 
{
	
	// [YUV][YUV] ...
	YUV_Data = new float [Width * Height * 3];

	for (int i = 0; i < Width * Height; ++i) {
		// Data is in BGR
		unsigned char curr_B = Data[3 * i];
		unsigned char curr_G = Data[3 * i + 1];
		unsigned char curr_R = Data[3 * i + 2];
		
		// Y U V
		YUV_Data[3 * i] = 0.299 * curr_R + 0.587 * curr_G + 0.114 * curr_B;
		YUV_Data[3 * i + 1] = 0.596 * curr_R + (-0.274) * curr_G + (-0.322) * curr_B;
		YUV_Data[3 * i + 2] = 0.211 * curr_R + (-0.523) * curr_G + 0.312 * curr_B;

	}

	return 0;
}



/* convert to RGB space */
bool MyImage::YUV2RGB()
{
	for (int i = 0; i < Width * Height; ++i) {
		float curr_Y = YUV_Data[3 * i];
		float curr_U = YUV_Data[3 * i + 1];
		float curr_V = YUV_Data[3 * i + 2];

		float b = 1 * curr_Y + (-1.106) * curr_U + 1.703 * curr_V;
		float g = 1 * curr_Y + (-0.272) * curr_U + (-0.647) * curr_V;
		float r = 1 * curr_Y + 0.956 * curr_U + 0.621 * curr_V;

		// B G R
		Data[3 * i] = (b < 0 ? 0 : b > 255 ? 255 : b);
		Data[3 * i + 1] = (g < 0 ? 0 : g > 255 ? 255 : g);
		Data[3 * i + 2] = (r < 0 ? 0 : r > 255 ? 255 : r);

	}
	return 0;
}



/* YUV subsampling */
bool MyImage::SubSampling()
{

	return 0;
}



/* Adjust upsampling for display*/
bool MyImage::UpSampling()
{

	return 0;
}



