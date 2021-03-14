#pragma once
#include "PNGFunctions.h"

class PNGFile
{
public:
	PNGFile(int width, int height);

	PNGFile(std::string filename);

	bool SaveFile(std::string filename);

	void Dispose();

	void WritePixel(Point point, PNG_PIXEL pixel_data);

	void print_palette();

	void print_pixels();

	int Width;

	int Height;

private:

	std::fstream image;

	uint8_t* png_index_data;

	std::vector<PNG_PIXEL> palette;
};

