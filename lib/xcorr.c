/*
Picor - Cross correlation implementation
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

#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

#include <xcorr.h>

double xcorr_spent_time = 0.0;

double xcorr(const double *x, const double *y, int n)
{
	int i;
	double mx, my, sx, sy, sxy, r;
	struct timeval  t0, t1;

	gettimeofday(&t0, NULL);

	/* Compute mean */
	mx = 0;
	my = 0;
	for (i = 0; i < n; i++) {
		mx += x[i];
		my += y[i];
	}
	mx /= n;
	my /= n;

	/* Prepare numerator and denominator stuff */
	sxy = 0;
	sx = 0;
	sy = 0;
	for (i = 0; i < n; i++) {
		sxy += (x[i] - mx) * (y[i] - my);
		sx += (x[i] - mx) * (x[i] - mx);
		sy += (y[i] - my) * (y[i] - my);
	}

	/* Cross-correlation */
	r = sxy / sqrt(sx * sy);

	gettimeofday(&t1, NULL);
	xcorr_spent_time += ((t1.tv_sec - t0.tv_sec) * 1000000L
			+ t1.tv_usec - t0.tv_usec) / 1e6;
	return r;
}
