#include "PNGFunctions.h"

void PNGFunctions::reverse(uint8_t* arr, int n)
{
	// create an empty stack of integers
	std::stack<uint8_t> stack;

	// push each element in the array into a stack
	for (int i = 0; i < n; i++) {
		stack.push(arr[i]);
	}

	// start from index 0
	int index = 0;

	// pop values from the stack until it is empty
	while (!stack.empty())
	{
		// assign each popped item back to the original array
		arr[index++] = stack.top();
		stack.pop();
	}
}

void PNGFunctions::ReverseCopyMem(void* dst, const void* src, size_t len)
{
	memcpy(dst, src, len);
	reverse((uint8_t*)dst, len);
}

void PNGFunctions::AddChunk(std::ofstream& file, uint32_t chunk_type, uint8_t* data, int data_length)
{
	PNG_CHUNK chunk;
	chunk.CRC = 0;
	chunk.data = data;

	ReverseCopyMem(&(chunk.dataLength), &data_length, 4);
	ReverseCopyMem(&(chunk.chunkType), &chunk_type, 4);

	int chunk_size = sizeof(chunk) - sizeof(chunk.data) + data_length;

	uint8_t* chunk_in_bytes = (uint8_t*)malloc(chunk_size);

	memcpy(chunk_in_bytes, &chunk, 8);
	memcpy(&chunk_in_bytes[8], chunk.data, data_length);

	chunk.CRC = CalculateCRC(&chunk_in_bytes[4], data_length + 4);
	ReverseCopyMem(&chunk_in_bytes[chunk_size - 4], &chunk.CRC, 4);

	file.write((char*)chunk_in_bytes, chunk_size);
	free(chunk_in_bytes);
}

uint8_t* PNGFunctions::CreateIHDRData(uint32_t width, uint32_t height, uint8_t bit_depth, uint8_t color_type, uint8_t compression_method, uint8_t filter_method, uint8_t interface_method)
{
	uint8_t* data = (uint8_t*)malloc(IHDR_SIZE);

	if (!data)
		return nullptr;

	ReverseCopyMem(data, &width, 4);
	ReverseCopyMem(&data[4], &height, 4);
	ReverseCopyMem(&data[8], &bit_depth, 1);
	ReverseCopyMem(&data[9], &color_type, 1);
	ReverseCopyMem(&data[10], &compression_method, 1);
	ReverseCopyMem(&data[11], &filter_method, 1);
	ReverseCopyMem(&data[12], &interface_method, 1);

	return data;
}

uint8_t* PNGFunctions::CreatePLTEData(std::vector<PNG_PIXEL> palette_list)
{
	uint8_t* data = (uint8_t*)malloc(palette_list.size() * PIXEL_SIZE);

	int i = 0;
	for (PNG_PIXEL pixel : palette_list)
	{
		memcpy(&data[PIXEL_SIZE * i], &pixel, PIXEL_SIZE);
		++i;
	}

	return data;
}

bool PNGFunctions::IsImageValid(std::string filepath)
{
	std::ifstream imageFile(filepath, std::ios::ate | std::ios::binary);

	unsigned char pngSignature[8];

	if (imageFile.is_open())
		for (int i = 0; i < sizeof(pngSignature); i++)
		{
			char temp;
			imageFile.read(&temp, 1);
			pngSignature[i] = temp;
		}
	else
		return false;

	imageFile.close();

	for (int i = 0; i < sizeof(pngSignature); i++)
		if (PNGSignature[i] != pngSignature[i])
			return false;

	return true;
}

bool PNGFunctions::pixel_comp(PNG_PIXEL first, PNG_PIXEL second)
{
	if (first.red == second.red && first.green == second.green && first.blue == second.blue)
		return true;

	return false;
}

unsigned long crc_table[256];

/* Flag: has the table been computed? Initially false. */
int crc_table_computed = 0;

/* Make the table for a fast CRC. */
void make_crc_table(void)
{
	unsigned long c;
	int n, k;

	for (n = 0; n < 256; n++) {
		c = (unsigned long)n;
		for (k = 0; k < 8; k++) {
			if (c & 1)
				c = 0xedb88320L ^ (c >> 1);
			else
				c = c >> 1;
		}
		crc_table[n] = c;
	}
	crc_table_computed = 1;
}

/* Update a running CRC with the bytes buf[0..len-1]--the CRC
   should be initialized to all 1's, and the transmitted value
   is the 1's complement of the final running CRC (see the
   crc() routine below)). */

unsigned long update_crc(unsigned long crc, unsigned char* buf,
	int len)
{
	unsigned long c = crc;
	int n;

	if (!crc_table_computed)
		make_crc_table();
	for (n = 0; n < len; n++) {
		c = crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
	}
	return c;
}

/* Return the CRC of the bytes buf[0..len-1]. */
unsigned long PNGFunctions::CalculateCRC(unsigned char* buf, int len)
{
	return update_crc(0xffffffffL, buf, len) ^ 0xffffffffL;
}