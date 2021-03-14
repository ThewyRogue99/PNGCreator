#pragma once

//Chunk Types
#define TYPE_IHDR 0x49484452
#define TYPE_PLTE 0x504c5445
#define TYPE_IDAT 0x49444154
#define TYPE_IEND 0x49454e44

//Chunk data sizes
#define IHDR_SIZE 13
#define PIXEL_SIZE 3

//Pixel formats
#define FORMAT_GRAYSCALE 0
#define FORMAT_RGB 2
#define FORMAT_INDEXED 3
#define FORMAT_GRAYSCALE_ALPHA 4
#define FORMAT_RGBA 6

//CRC
#define WIDTH  (8 * sizeof(uint32_t))
#define TOPBIT (1 << (WIDTH - 1))
#define CRC_POLYNOMIAL 0xB71DC10401

const unsigned char PNGSignature[] = { 137, 80, 78, 71, 13, 10, 26, 10 };

typedef struct pngchunk
{
	uint32_t dataLength; //Length of chunk data
	uint32_t chunkType; // Type of this chunk (eg. IHDR, PLTE, IDAT...)
	uint8_t* data;
	uint32_t CRC;

}PNG_CHUNK;

class PNG_PIXEL
{
public:
	uint8_t red;
	uint8_t green;
	uint8_t blue;

	PNG_PIXEL() : red(0), green(0), blue(0) {}

	PNG_PIXEL(uint8_t _red, uint8_t _green, uint8_t _blue) : red(_red), green(_green), blue(_blue) {}
};

class Point
{
public:
	unsigned int x;
	unsigned int y;

	Point() : x(0), y(0) {}

	Point(unsigned int _x, unsigned int _y) : x(_x), y(_y) {}
};