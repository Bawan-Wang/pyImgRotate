#include "rotate_image.h"

void rotate_image_mapping(BYTE *srcImg, BYTE *dstImg, SizeConvert stSize, double dAngle)
{
	Point2f stOriPoint;
	Point2D stMapPoint;
	int vIdx, hIdx;
	double cos_theta, sin_theta, new_centerX, new_centerY, old_centerX, old_centerY, offsetX, offsetY;

	cos_theta = cos(dAngle);
	sin_theta = sin(dAngle);

	old_centerX = stSize.old_width / 2.;
	old_centerY = stSize.old_height / 2.;
	new_centerX = (old_centerX * cos_theta - old_centerY * sin_theta);
	new_centerY = (old_centerX * sin_theta + old_centerY * cos_theta);
	offsetX = (old_centerX - new_centerX) + ((stSize.new_width - stSize.old_width) / 2.);
	offsetY = (old_centerY - new_centerY) + ((stSize.new_height - stSize.old_height) / 2.);

	for (vIdx = 0; vIdx < stSize.old_height; vIdx++)
	{
		for (hIdx = 0; hIdx < stSize.old_width; hIdx++)
		{
			stMapPoint.x = do_round(hIdx * cos_theta - vIdx * sin_theta + offsetX);
			stMapPoint.y = do_round(hIdx * sin_theta + vIdx * cos_theta + offsetY);
			if ((stMapPoint.x < 0) || (stMapPoint.y < 0) || (stMapPoint.x >= stSize.new_width) || (stMapPoint.y >= stSize.new_height))
				continue;
			dstImg[(stMapPoint.y * (stSize.new_width)) + stMapPoint.x] = srcImg[(vIdx * (stSize.old_width)) + hIdx];
		}
	}

	return;
}

void rotate_image_inverse_mapping1(BYTE *srcImg, BYTE *dstImg, SizeConvert stSize, double dAngle)
{
	Point2f stOriPoint;
	Point2D stMapPoint;
	int vIdx, hIdx;
	double cos_theta, sin_theta, new_centerX, new_centerY, old_centerX, old_centerY, offsetX, offsetY;

	cos_theta = cos(dAngle);
	sin_theta = sin(dAngle);

	new_centerX = stSize.new_width / 2.;
	new_centerY = stSize.new_height / 2.;
	old_centerX = ( new_centerX * cos_theta + new_centerY * sin_theta);
	old_centerY = (-new_centerX * sin_theta + new_centerY * cos_theta);
	offsetX = (new_centerX - old_centerX) - ((stSize.new_width - stSize.old_width) / 2.);
	offsetY = (new_centerY - old_centerY) - ((stSize.new_height - stSize.old_height) / 2.);

	int round_delta = 512;
	for (vIdx = 0; vIdx < stSize.new_height; vIdx++)
	{
		for (hIdx = 0; hIdx < stSize.new_width; hIdx++)
		{
			stMapPoint.x = do_round( hIdx * cos_theta + vIdx * sin_theta + offsetX);
			stMapPoint.y = do_round(-hIdx * sin_theta + vIdx * cos_theta + offsetY);
			if (((unsigned)stMapPoint.x < stSize.old_width) && ((unsigned)stMapPoint.y < stSize.old_height)) {
				dstImg[vIdx * stSize.new_width + hIdx] = srcImg[(stMapPoint.y * (stSize.old_width)) + stMapPoint.x];
			}
		}
	}

	return;
}

void rotate_image_inverse_mapping2(BYTE *srcImg, BYTE *dstImg, SizeConvert stSize, double dAngle)
{
	Point2f stOriPoint;
	Point2D stMapPoint;
	int vIdx, hIdx;
	double cos_theta, sin_theta, new_centerX, new_centerY, old_centerX, old_centerY, offsetX, offsetY;

	cos_theta = cos(dAngle);
	sin_theta = sin(dAngle);

	new_centerX = stSize.new_width / 2.;
	new_centerY = stSize.new_height / 2.;
	old_centerX = (new_centerX * cos_theta + new_centerY * sin_theta);
	old_centerY = (-new_centerX * sin_theta + new_centerY * cos_theta);
	offsetX = (new_centerX - old_centerX) - ((stSize.new_width - stSize.old_width) / 2.);
	offsetY = (new_centerY - old_centerY) - ((stSize.new_height - stSize.old_height) / 2.);

	int round_delta = 512;
	for (vIdx = 0; vIdx < stSize.new_height; vIdx++)
	{
		for (hIdx = 0; hIdx < stSize.new_width; hIdx++)
		{
			int adelta = (cos_theta * hIdx * 1024);
			int bdelta = (-sin_theta * hIdx * 1024);
			int X0 = ((sin_theta * vIdx + offsetX) * 1024) + round_delta;
			int Y0 = ((cos_theta * vIdx + offsetY) * 1024) + round_delta;
			stMapPoint.x = (X0 + adelta) >> 10;
			stMapPoint.y = (Y0 + bdelta) >> 10;

			if (((unsigned)stMapPoint.x < stSize.old_width) && ((unsigned)stMapPoint.y < stSize.old_height)) {
				dstImg[vIdx * stSize.new_width + hIdx] = srcImg[(stMapPoint.y * (stSize.old_width)) + stMapPoint.x];
			}
		}
	}

	return;
}

BYTE *rotate_image(BYTE* img, int width, int height, int rot, int *new_width, int *new_height) {
	BYTE *ret_img = NULL;
	double dAngle;
	SizeConvert stSize;

	if (NULL == img) {
		printf("Error : img is NULL");
		return ret_img;
	}

	if (NULL == new_width) {
		printf("Error : new_width is NULL");
		return ret_img;
	}

	if (NULL == new_height) {
		printf("Error : new_height = NULL");
		return ret_img;
	}

	if (rot < 0) {
		printf("Error degree (%d): input degree should be 0 ~ 359", rot);
		return ret_img;
	}

	//Calculate angles
	rot %= 360;
	printf("degree : %d\n", rot);
	dAngle = (double)rot * GET_RAD;

	//Set the size of old image
	stSize.old_width  = width;
	stSize.old_height = height;

	//Calculate new size after image rotation
	*new_width  = stSize.new_width  = do_round(stSize.old_width  * fabs(cos(dAngle)) + stSize.old_height * fabs(sin(dAngle)));
	*new_height = stSize.new_height = do_round(stSize.old_height * fabs(cos(dAngle)) + stSize.old_width  * fabs(sin(dAngle)));

	//Allocate output image
	ret_img = (BYTE *)malloc((*new_width) * (*new_height) * sizeof(BYTE));
	if (NULL == ret_img) {
		printf("Error : ret_img is NULL");
		return ret_img;
	}
	memset(ret_img, 0, (*new_width) * (*new_height) * sizeof(BYTE));

	//Process rotation
#if INVERSE_MAPPING
	rotate_image_inverse_mapping1(img, ret_img, stSize, dAngle);
#else
	rotate_image_mapping(img, ret_img, stSize, dAngle);
#endif

	return ret_img;
}

void rotate_image_release(BYTE ** releaseImg)
{
	if (*releaseImg != NULL) {
		free(*releaseImg);
	}
}