#include "PNGFile.h"
#include <Windows.h>

int main()
{
	PNGFile image(200, 200);

	for (int height = 0; height < image.Height; height++)
	{
		for (int width = 0; width < image.Width; width++)
		{
			if (width < image.Width / 2)
				image.WritePixel(Point(width, height), PNG_PIXEL(255, 0, 0));
			else
				image.WritePixel(Point(width, height), PNG_PIXEL(0, 255, 0));
		}
	}


	image.SaveFile("C:\\Users\\batuh\\Desktop\\Selam.png");

	image.Dispose();
	return 0;
}