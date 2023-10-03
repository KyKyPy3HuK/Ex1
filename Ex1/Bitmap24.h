#pragma once

class Bitmap24
{
	// fields
private:

	uint8_t* bitmap;
	BITMAPINFOHEADER biInfo;
	RGBTRIPLE** pixelMap;
	int sizeByt;
	int sizePix;
	int widthPix;
	int heigth;
	int widthByt;
	int fullWidthByt;
	int offsetByt;

	// constructors
public:
	Bitmap24() {
		this->bitmap = nullptr;

		this->sizeByt = 0;

		this->widthPix = 0;
		this->heigth = 0;
		this->sizePix = 0;
		this->fullWidthByt = 0;
		this->widthByt = 0;
		this->offsetByt = 0;

		this->pixelMap = nullptr;
	}

	Bitmap24(BITMAPINFOHEADER& biInfo, uint8_t* bitmap) {
		this-> bitmap = bitmap;
		this-> biInfo = biInfo;
		
		this-> sizeByt = biInfo.biSize;
		
		this-> widthPix = biInfo.biWidth;
		this-> heigth = biInfo.biHeight;
		this-> sizePix = biInfo.biWidth * biInfo.biHeight;
		this-> fullWidthByt = biInfo.biSize / biInfo.biHeight;
		this-> widthByt = biInfo.biWidth * 3;
		this-> offsetByt = fullWidthByt - widthByt;


		this-> pixelMap = SetPixelMap();
	}

	// methods
private:
	RGBTRIPLE** SetPixelMap();
	uint8_t* SetBitmapFromPixelMap();

public:
	inline RGBTRIPLE GetPixel(int coordX, int coordY) {
		if (coordX < widthPix && coordX > -1 && coordY > -1 && coordY < heigth)
		{
			return this->pixelMap[coordX][coordY];
		}
		else {
			return RGBTRIPLE{ 0,0,0 };
		}
	};

	inline RGBTRIPLE SetPixel(int coordX, int coordY, RGBTRIPLE color) {
		if (coordX < widthPix && coordX > -1 && coordY > -1 && coordY < heigth)
		{
			RGBTRIPLE temp = this->pixelMap[coordX][coordY];
			this->pixelMap[coordX][coordY] = color;
			return temp;
		}
		else {
			return RGBTRIPLE{ 0,0,0 };
		}
	};
	uint8_t* GetBitmap();

	// static methods
private:
	inline int alignWidthInBytes(int imageWidth) {
		uint32_t	alignBytes = (4 - (imageWidth % 4)) % 4;
		uint32_t	fullWidthInBytes = imageWidth + alignBytes;
		return fullWidthInBytes;
	}

	inline int alignImageSizeInBytes(int imageWidth, int imageHeight) {

		uint32_t fullWidthInBytes = alignWidthInBytes(imageWidth * 3);
		return fullWidthInBytes * imageHeight;
	}
};

