#include "pch.h"
#include "Bitmap24.h"

RGBTRIPLE** Bitmap24::SetPixelMap()
{	

	// Инициализация памяти для массива пикселей
	RGBTRIPLE** newPixelMap = new RGBTRIPLE * [this->widthPix];
	for (int i = 0; i < this->widthPix; i++)
	{
		newPixelMap[i] = new RGBTRIPLE[this->heigth];
		for (int j = 0; j < this->heigth; j++)
		{
			newPixelMap[i][j] = RGBTRIPLE{0,0,0};
		}
	}

	for (int i = 0; i < this->heigth; ++i)
	{
		for (int j = 0; j < this->widthByt; ++j)
		{
			UINT column = j;
			UINT row = this->heigth - i;
			UINT k = (this->fullWidthByt * i + j * 3);
			newPixelMap[column][row] = RGBTRIPLE{this-> bitmap[k + 2],this->bitmap[k + 1], this->bitmap[k]};
		}
	}
    return newPixelMap;
}
