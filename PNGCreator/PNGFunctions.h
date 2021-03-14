#pragma once
#include <string>
#include <fstream>
#include <stack>
#include <vector>

#include "Structures.h"

namespace PNGFunctions
{
	bool IsImageValid(std::string filepath);

	bool pixel_comp(PNG_PIXEL first, PNG_PIXEL second);

	void AddChunk(std::ofstream& file, uint32_t chunk_type, uint8_t* data, int data_length);

	void reverse(uint8_t* arr, int n);

	void ReverseCopyMem(void* dst, const void* src, size_t len);

	uint8_t* CreateIHDRData(uint32_t width, uint32_t height, uint8_t bit_depth, uint8_t color_type, uint8_t compression_method, uint8_t filter_method, uint8_t interface_method);

	uint8_t* CreatePLTEData(std::vector<PNG_PIXEL> palette_list);

	unsigned long CalculateCRC(unsigned char* buf, int len);
}
