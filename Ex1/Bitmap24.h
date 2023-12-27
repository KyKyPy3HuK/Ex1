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
	int height_;
	int widthByt_;
	int fullWidthByt_;
	int offsetByt_;

	// constructors
public:
	Bitmap24() {
		bitmap_ = nullptr;

		sizeByt_ = 0;

		widthPix_ = 0;
		height_ = 0;
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
		height_ = biInfo.biHeight;
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

	inline uint32_t getWidth() {
		return widthPix_;
	}
	inline uint32_t getHeight() {
		return height_;
	}
	static inline RGBTRIPLE rgbtAdd(RGBTRIPLE left, RGBTRIPLE right) {
		RGBTRIPLE sum;
		int blue = left.rgbtBlue + right.rgbtBlue;
		if (blue > 255)
		{
			sum.rgbtBlue = 255;
		}
		else
		{
			sum.rgbtBlue = blue;
		}

		int green = left.rgbtGreen + right.rgbtGreen;
		if (green > 255)
		{
			sum.rgbtGreen = 255;
		}
		else
		{
			sum.rgbtGreen = green;
		}

		int red = left.rgbtRed + right.rgbtRed;
		if (red > 255)
		{
			sum.rgbtRed = 255;
		}
		else
		{
			sum.rgbtRed = red;
		}

		//sum.rgbtBlue = left.rgbtBlue + right.rgbtBlue;
		//sum.rgbtGreen = left.rgbtGreen + right.rgbtGreen;
		//sum.rgbtRed = left.rgbtRed + right.rgbtRed;
		return sum;
	}

	static inline RGBTRIPLE rgbtSub(RGBTRIPLE left, RGBTRIPLE right) {
		RGBTRIPLE sub;
		int nB = left.rgbtBlue - right.rgbtBlue;
		if (nB > 255)
		{
			sub.rgbtBlue = 255;
		}
		else if(nB < 0)
		{
			sub.rgbtBlue = 0;
		}
		else
		{
			sub.rgbtBlue = nB;
		}
		
		int nG = left.rgbtGreen - right.rgbtGreen;
		if (nG > 255)
		{
			sub.rgbtGreen = 255;
		}
		else if (nG < 0)
		{
			sub.rgbtGreen = 0;
		}
		else
		{
			sub.rgbtGreen = nG;
		}

		int nR = left.rgbtRed - right.rgbtRed;
		if (nR > 255)
		{
			sub.rgbtRed = 255;
		}
		else if (nR < 0)
		{
			sub.rgbtRed = 0;
		}
		else
		{
			sub.rgbtRed = nR;
		}

		return sub;
	}

	static inline RGBTRIPLE rgbtMul(RGBTRIPLE rgbt, double coef) {
		RGBTRIPLE mul;
		mul.rgbtBlue = rgbt.rgbtBlue * coef;
		mul.rgbtGreen = rgbt.rgbtGreen * coef;
		mul.rgbtRed = rgbt.rgbtRed * coef;

		int nB = rgbt.rgbtBlue * coef;
		if (nB > 255)
		{
			mul.rgbtBlue = 255;
		}
		else if (nB < 0)
		{
			mul.rgbtBlue = 0;
		}
		else
		{
			mul.rgbtBlue = nB;
		}

		int nG = rgbt.rgbtGreen * coef;
		if (nG > 255)
		{
			mul.rgbtGreen = 255;
		}
		else if (nG < 0)
		{
			mul.rgbtGreen = 0;
		}
		else
		{
			mul.rgbtGreen = nG;
		}

		int nR = rgbt.rgbtRed * coef;
		if (nR > 255)
		{
			mul.rgbtRed = 255;
		}
		else if (nR < 0)
		{
			mul.rgbtRed = 0;
		}
		else
		{
			mul.rgbtRed = nR;
		}
		return mul;
	}

	inline RGBTRIPLE GetPixel(int coordX, int coordY) {
		if (coordX < widthPix_ && coordX > -1 && coordY > -1 && coordY < height_)
		{
			return pixelMap_[coordX][coordY];
		}
		else {
			return RGBTRIPLE{ 0,0,0 };
		}
	};

	inline RGBTRIPLE SetPixel(int coordX, int coordY, RGBTRIPLE color) {
		if (coordX < widthPix_ && coordX > -1 && coordY > -1 && coordY < height_)
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

	inline bool tryGetPixel(int coordX, int coordY) {
		if (coordX < widthPix_ && coordX > -1 && coordY > -1 && coordY < height_)
		{
			return true;
		}
		else {
			return false;
		}
	}
	

	inline static RGBTRIPLE linearInterpolation(RGBTRIPLE left, RGBTRIPLE right, double x) {

		return rgbtAdd(rgbtMul(left, (1. - x)), rgbtMul(right, x));
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

