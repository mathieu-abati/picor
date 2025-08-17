/*
Picor - Images manipulation tools implementation
Copyright (C) 2015-2025 Mathieu Abati

This file is part of Picor.
Picor is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 2 of the License, or (at your option) any later
version.

Picor is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
Picor. If not, see <https://www.gnu.org/licenses/>.
*/

#include <math.h>
#include <MagickCore/MagickCore.h>

#include <img_tools.h>

int image_to_series(Image *image, double *series, int size)
{
	int ret = 0, i, img_side_size;
	Image *prepared_image;
	ExceptionInfo *exception;
	Quantum *pixels;

	exception = AcquireExceptionInfo();

	img_side_size = sqrt(size);
	prepared_image = ResizeImage(
			image,
			img_side_size, img_side_size,
			LanczosFilter,
			exception);
	if (exception->severity != UndefinedException)
		CatchException(exception);
	if (prepared_image == (Image *) NULL) {
		ret = -1;
		goto end;
	}
	if (!TransformImageColorspace(prepared_image, GRAYColorspace,
				exception)) {
		ret = -2;
		goto end;
	}
	pixels = GetAuthenticPixels(
			prepared_image,
			0, 0, img_side_size, img_side_size,
			exception);
	if (pixels == NULL) {
		ret = -3;
		goto end;
	}
	for (i = 0; i < size; i++) {
		series[i] = pixels[i];
	}

end:
	if (prepared_image)
		DestroyImage(prepared_image);
	return ret;
}

