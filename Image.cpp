//*****************************************************************************
//
// Image.cpp : Defines the class operations on images
//
// Author - Parag Havaldar
// Code used by students as starter code to display and modify images
//
//*****************************************************************************

#include "Image.h"
#include <iostream>

// Constructor and Desctructors
MyImage::MyImage() 
{
	Data = nullptr;
	Width = -1;
	Height = -1;
	ImagePath[0] = 0;

	n_pixels = 1920 * 1080; // fixed size
	YUV_Data = nullptr;
	YUV_Data_Smp = nullptr;
}

MyImage::~MyImage()
{
	if (Data)
		delete Data;
	if (YUV_Data)
		delete YUV_Data;
	if (YUV_Data_Smp)
		delete YUV_Data_Smp;
}


// Copy constructor
MyImage::MyImage(MyImage *otherImage)
{
	Height = otherImage->Height;
	Width  = otherImage->Width;
	Data   = new unsigned char[Width*Height*3];

	n_pixels = otherImage->n_pixels;
	strcpy(otherImage->ImagePath, ImagePath );

	for (int i = 0; i < (Height*Width*3); i++)
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
	delete[] Rbuf;
	delete[] Gbuf;
	delete[] Bbuf;

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
	
	// format: [YYY ... n_pix][UUU ... n_pix][VVV ... n_pix]
	// ease for further steps
	YUV_Data = new float [n_pixels * 3];

	for (int i = 0; i < n_pixels; ++i) {
		// Data is in [BGR][BGR] ...
		unsigned char curr_B = Data[3 * i];
		unsigned char curr_G = Data[3 * i + 1];
		unsigned char curr_R = Data[3 * i + 2];
		
		// Y U V
		YUV_Data[i] = 0.299 * curr_R + 0.587 * curr_G + 0.114 * curr_B;
		YUV_Data[i + n_pixels] = 0.596 * curr_R + (-0.274) * curr_G + (-0.322) * curr_B;
		YUV_Data[i + n_pixels * 2] = 0.211 * curr_R + (-0.523) * curr_G + 0.312 * curr_B;

	}

	return 0;
}



/* convert to RGB space */
bool MyImage::YUV2RGB()
{
	for (int i = 0; i < n_pixels; ++i) {

		float curr_Y = YUV_Data_Smp[i];
		float curr_U = YUV_Data_Smp[i + n_pixels];
		float curr_V = YUV_Data_Smp[i + n_pixels * 2];

		float r = 1 * curr_Y + 0.956 * curr_U + 0.621 * curr_V;
		float g = 1 * curr_Y + (-0.272) * curr_U + (-0.647) * curr_V;
		float b = 1 * curr_Y + (-1.106) * curr_U + 1.703 * curr_V;

		// B G R
		Data[3 * i] = (b < 0 ? 0 : b > 255 ? 255 : b);
		Data[3 * i + 1] = (g < 0 ? 0 : g > 255 ? 255 : g);
		Data[3 * i + 2] = (r < 0 ? 0 : r > 255 ? 255 : r);

	}
	return 0;
}



bool MyImage::helper_subsample(int step, int offset)
{
	/* 
	step: step of Y, U, V
	offset: shift to target channel address
		(since we store 3d channel in 1d array)
	*/
	if (step > 1) {
		for (int i = 0; i < n_pixels; ++i) {
			int col = i % Width;
			if (col % step == 0) {
				YUV_Data_Smp[i + offset] = YUV_Data[i + offset];
			}
			else {
				YUV_Data_Smp[i + offset] = 0;
			}
		}
	}
	else {
		for (int i = 0; i < n_pixels; ++i) {
			YUV_Data_Smp[i + offset] = YUV_Data[i + offset];
		}
	}

	return 0;
}

/* Subsample Y, U, V along width dimension */
bool MyImage::SubSampling(int step_y, int step_u, int step_v)
{
	YUV_Data_Smp = new float[Width * Height * 3];
	// subsample y
	helper_subsample(step_y, 0);
	// subsample u
	helper_subsample(step_u, n_pixels);
	// subsample v
	helper_subsample(step_v, 2 * n_pixels);

	return 0;
}



/* helper function: replace non-zero values */
bool MyImage::helper_upsample(int step, int offset)
{
	for (int i = 0; i < n_pixels; ++i) {
		int row = i / Width;
		int col = i % Width;
		if (col % step != 0) {
			// find prev and next non-zero values
			int prev = col - col % step;
			int next = col + step - col % step;
			if (next > Width) {
				next = prev;
			}
			prev += row*Width;
			next += row*Width;
			YUV_Data_Smp[i + offset] = \
				(YUV_Data_Smp[prev + offset] + YUV_Data_Smp[next + offset]) / 2;
		}
	}

	return 0;
}

/* Upsampling for display*/
bool MyImage::UpSampling(int step_y, int step_u, int step_v)
{
	if (step_y > 1)
		helper_upsample(step_y, 0);
	if (step_u > 1)
		helper_upsample(step_u, n_pixels);
	if (step_v > 1)
		helper_upsample(step_v, 2*n_pixels);
	
	return 0;
}



bool MyImage::Scaling(float Sw, float Sh, bool anti_alias)
{
	if (!Sw || !Sh) {
		return 0;
	}

	int newWidth = Width * Sw;
	int newHeight = Height * Sh;
	int new_pixels = newWidth * newHeight;
	unsigned char* newData = new unsigned char[new_pixels * 3];


	if (!anti_alias) {
		for (int i = 0; i < newHeight; ++i) {
			for (int j = 0; j < newWidth; ++j) {
				// inverse map to source pixel
				int src_i = (int)(i / Sh); int src_j = (int)(j / Sw);
				newData[3 * (i * newWidth + j)] = Data[3 * (src_i * Width + src_j)];
				newData[3 * (i * newWidth + j) + 1] = Data[3 * (src_i * Width + src_j) + 1];
				newData[3 * (i * newWidth + j) + 2] = Data[3 * (src_i * Width + src_j) + 2];
			}
		}
	} else {
		// calculate average of the neighbor (3x3) of source pixel for anti_aliasing

		// unsigned char* avgData = get_average();
		for (int i = 0; i < newHeight; ++i) {
			for (int j = 0; j < newWidth; ++j) {
				// inverse map to source pixel
				int src_i = (int)(i / Sh); int src_j = (int)(j / Sw);

				int Bsum = 0;
				int Gsum = 0;
				int Rsum = 0;

				int miss_val = 0;
				for (int k = src_j - 1; k <= src_j + 1; k++) {
					for (int l = src_i - 1; l <= src_i + 1; l++) {
						if (k < 0 || k >= Width || l < 0 || l >= Height) {
							miss_val += 1;
							continue;
						}

						Bsum += Data[3 * (l * Width + k)];
						Gsum += Data[3 * (l * Width + k) + 1];
						Rsum += Data[3 * (l * Width + k) + 2];
					}
				}

				newData[3 * (i * newWidth + j)] = Bsum / (9 - miss_val);
				newData[3 * (i * newWidth + j) + 1] = Gsum / (9 - miss_val);
				newData[3 * (i * newWidth + j) + 2] = Rsum / (9 - miss_val);
			}
		}
		
	}
	


	// update object properties
	delete[] Data;

	Data = newData;
	Width = newWidth;
	Height = newHeight;
	n_pixels = new_pixels;

	return 0;
}


