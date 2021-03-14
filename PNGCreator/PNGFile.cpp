#include "PNGFile.h"

using namespace PNGFunctions;

PNGFile::PNGFile(int width, int height)
{
	Width = width;
	Height = height;

	if (width == 0 || height == 0)
		throw "Invalid size";

	png_index_data = (uint8_t*)malloc(width * height);

	for (int i = 0; i < width * height; i++)
		png_index_data[i] = 15;
}

PNGFile::PNGFile(std::string filepath)
{
}

bool PNGFile::SaveFile(std::string filepath)
{
	std::ofstream file(filepath, std::ios::binary);

	bool success = false;

	if (file.is_open())
	{
		file << PNGSignature;
		
		uint8_t* ihdr_data = CreateIHDRData(Width, Height, 4, FORMAT_INDEXED, 0, 0, 0);

		AddChunk(file, TYPE_IHDR, ihdr_data, IHDR_SIZE);

		uint8_t* palette_data = CreatePLTEData(palette);

		AddChunk(file, TYPE_PLTE, palette_data, palette.size() * PIXEL_SIZE);

		AddChunk(file, TYPE_IDAT, png_index_data, Width * Height);

		AddChunk(file, TYPE_IEND, NULL, NULL);

		free(ihdr_data);
		free(palette_data);

		file.close();
		success = true;
	}
	else
		success = false;

	return success;
}


void PNGFile::WritePixel(Point point, PNG_PIXEL pixel_data)
{
	int index = -1;
	int i = 5;

	int index_location = Width * point.y + point.x;

	for (PNG_PIXEL palette_pixel : palette)
	{
		if (pixel_comp(palette_pixel, pixel_data))
		{
			index = i;
			break;
		}
		++i;
	}

	if (index == -1)
	{
		palette.push_back(pixel_data);
		index = i;
	}

	if (index_location < Width * Height)
		png_index_data[index_location] = index;
	else
		throw std::out_of_range("Pixel written is out of range!");
}

void PNGFile::print_palette()
{
	for (PNG_PIXEL pixel : palette)
		printf(" { %i %i %i } ", pixel.red, pixel.green, pixel.blue);
}

void PNGFile::print_pixels()
{
	int len = Width * Height;

	for (int i = 0; i < len; i++)
		printf("%i", png_index_data[i]);
}

void PNGFile::Dispose()
{
	if (png_index_data != NULL)
		free(png_index_data);
}
