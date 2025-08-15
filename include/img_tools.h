#ifndef _IMG_TOOLS_H_
#define _IMG_TOOLS_H_

/*
 * Prepare an image to a series.
 * - resize image
 * - transform image color space to gray scale
 * @param[in] image
 * @param[out] series
 * @param[in] series size
 * @return 0 on success
 */
int image_to_series(Image *image, double *series, int size);

#endif
