#include "pch.h"
#include "Bitmap24.h"

RGBTRIPLE** Bitmap24::SetPixelMap()
{	

	// Инициализация памяти для массива пикселей
	RGBTRIPLE** newPixelMap = new RGBTRIPLE * [widthPix_];
	for (int x = 0; x < widthPix_; x++)
	{
		newPixelMap[x] = new RGBTRIPLE[height_];
		for (int y = 0; y < height_; y++)
		{
			newPixelMap[x][y] = RGBTRIPLE{0,0,0};
		}
	}

	uint32_t colOffset = 0;
	uint32_t rowOffset = 0;
	uint32_t k = 0;

	for (int y = 0; y < height_; y++)
	{
		for (int x = 0; x < widthPix_; x++)
		{
			rowOffset = (height_ - y - 1) * fullWidthByt_;
			colOffset = x * 3;
			k = rowOffset + colOffset;
			newPixelMap[x][y].rgbtBlue = bitmap_[k];
			newPixelMap[x][y].rgbtGreen = bitmap_[k + 1];
			newPixelMap[x][y].rgbtRed = bitmap_[k + 2];
		}
	}

    return newPixelMap;
}

uint8_t* Bitmap24::GetBitmap()
{
	
	uint8_t* newBitmap = new uint8_t[height_ * fullWidthByt_];

	uint32_t rowOffset = 0;
	uint32_t colOffset = 0;
	uint64_t k = 0;

	for (int y = 0; y < height_; y++)
	{
		for (int x = 0; x < widthPix_; x++)
		{
			rowOffset = (height_ - y - 1) * fullWidthByt_;
			colOffset = x * 3;
			k = rowOffset + colOffset;
			newBitmap[k] = pixelMap_[x][y].rgbtBlue;
			newBitmap[k + 1] = pixelMap_[x][y].rgbtGreen;
			newBitmap[k + 2] = pixelMap_[x][y].rgbtRed;
		}
	}

	return newBitmap;
}
