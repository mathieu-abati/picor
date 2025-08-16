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
