/*
Picor - Image manipulation header
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
