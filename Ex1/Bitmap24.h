#pragma once

class Bitmap24
{
	// fields
private:

	uint8_t* bitmap_;
	BITMAPINFOHEADER biInfo_;
	RGBTRIPLE** pixelMap_;
	int sizeByt_;
	int sizePix_;
	int widthPix_;
	int heigth_;
	int widthByt_;
	int fullWidthByt_;
	int offsetByt_;

	// constructors
public:
	Bitmap24() {
		bitmap_ = nullptr;

		sizeByt_ = 0;

		widthPix_ = 0;
		heigth_ = 0;
		sizePix_ = 0;
		fullWidthByt_ = 0;
		widthByt_ = 0;
		offsetByt_ = 0;

		pixelMap_ = nullptr;
	}

	Bitmap24(BITMAPINFOHEADER& biInfo, uint8_t* bitmap) {
		bitmap_ = bitmap;
		biInfo_ = biInfo;
		
		sizeByt_ = biInfo.biSize;
		
		widthPix_ = biInfo.biWidth;
		heigth_ = biInfo.biHeight;
		sizePix_ = biInfo.biWidth * biInfo.biHeight;
		fullWidthByt_ = biInfo.biSizeImage / biInfo.biHeight;
		widthByt_ = biInfo.biWidth * 3;
		offsetByt_ = fullWidthByt_ - widthByt_;


		pixelMap_ = SetPixelMap();
	}

	// methods
private:
	RGBTRIPLE** SetPixelMap();
	uint8_t* SetBitmapFromPixelMap();

public:

	static inline RGBTRIPLE rgbtAdd(RGBTRIPLE left, RGBTRIPLE right) {
		RGBTRIPLE sum;
		sum.rgbtBlue = left.rgbtBlue + right.rgbtBlue;
		sum.rgbtGreen = left.rgbtGreen + right.rgbtGreen;
		sum.rgbtRed = left.rgbtRed + right.rgbtRed;
		return sum;
	}

	static inline RGBTRIPLE rgbtSub(RGBTRIPLE left, RGBTRIPLE right) {
		RGBTRIPLE sub;
		sub.rgbtBlue = left.rgbtBlue - right.rgbtBlue;
		sub.rgbtGreen = left.rgbtGreen - right.rgbtGreen;
		sub.rgbtRed = left.rgbtRed - right.rgbtRed;
		return sub;
	}

	static inline RGBTRIPLE rgbtMul(RGBTRIPLE rgbt, double coef) {
		RGBTRIPLE mul;
		mul.rgbtBlue = rgbt.rgbtBlue * coef;
		mul.rgbtGreen = rgbt.rgbtGreen * coef;
		mul.rgbtRed = rgbt.rgbtRed * coef;
		return mul;
	}

	inline RGBTRIPLE GetPixel(int coordX, int coordY) {
		if (coordX < widthPix_ && coordX > -1 && coordY > -1 && coordY < heigth_)
		{
			return pixelMap_[coordX][coordY];
		}
		else {
			return RGBTRIPLE{ 0,0,0 };
		}
	};

	inline RGBTRIPLE SetPixel(int coordX, int coordY, RGBTRIPLE color) {
		if (coordX < widthPix_ && coordX > -1 && coordY > -1 && coordY < heigth_)
		{
			RGBTRIPLE temp = pixelMap_[coordX][coordY];
			pixelMap_[coordX][coordY] = color;
			return temp;
		}
		else {
			return RGBTRIPLE{ 0,0,0 };
		}
	};
	uint8_t* GetBitmap();

	inline static RGBTRIPLE linearInterpolation(RGBTRIPLE left, RGBTRIPLE right, double x) {
		RGBTRIPLE newRgbt{ 0,0,0 };

		newRgbt = rgbtAdd(rgbtMul(left, (1. - x)), rgbtMul(right, x));

		return newRgbt;
	}

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

