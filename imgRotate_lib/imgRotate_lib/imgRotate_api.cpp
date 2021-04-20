#include "pch.h"
#include "framework.h"
#include "rotate_image.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef IMGROTATELIB_EXPORTS
#define IMGROTATEDLL   extern "C" __declspec(dllexport)
#else
#define IMGROTATEDLL   extern "C" __declspec(dllimport)
#endif

IMGROTATEDLL BYTE *IMGRotate(BYTE* img, int width, int height, int rot, int *new_width, int *new_height)
{
	BYTE *ro_img = NULL;
	ro_img = rotate_image(img, width, height, rot, new_width, new_height);
	return ro_img;
}