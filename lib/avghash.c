/*
Picor - Average hash implementation
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

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

#include <avghash.h>

double avghash_spent_time = 0.0;
double avghash_dist_spent_time = 0.0;

void avghash(const double *s, int n, unsigned char *h)
{
	double m;
	int i;
	struct timeval  t0, t1;

	gettimeofday(&t0, NULL);

	memset(h, 0, n / 8);

	/* Compute mean */
	m = 0;
	for (i = 0; i < n; i++)
		m += s[i];
	m /= n;

	/* Compute hash */
	for (i = 0; i < n; i++)
		h[i / 8] |= (s[i] >= m) << (i % 8);

	gettimeofday(&t1, NULL);
	avghash_spent_time += ((t1.tv_sec - t0.tv_sec) * 1000000L
			+ t1.tv_usec - t0.tv_usec) / 1e6;
}

int dist_avghash(int n, const unsigned char *h1, const unsigned char *h2)
{
	int i, dist = 0;
	unsigned char xor_byte;
	struct timeval  t0, t1;

	gettimeofday(&t0, NULL);

	for (i = 0; i < n; i++) {
		xor_byte = h1[i] ^ h2[i];
		dist +=
			(xor_byte & (unsigned char)1) +
			(xor_byte & (unsigned char)2) +
			(xor_byte & (unsigned char)4) +
			(xor_byte & (unsigned char)8) +
			(xor_byte & (unsigned char)16) +
			(xor_byte & (unsigned char)32) +
			(xor_byte & (unsigned char)64) +
			(xor_byte & (unsigned char)128);
	}

	gettimeofday(&t1, NULL);
	avghash_dist_spent_time += ((t1.tv_sec - t0.tv_sec) * 1000000L
			+ t1.tv_usec - t0.tv_usec) / 1e6;
	return dist;
}

void get_printable_avghash(int n, const unsigned char *h, char *pr_h)
{
	int i;
	
	for (i = 0; i < n; i++)
		sprintf(&pr_h[i * 2], "%02x", h[i]);
}
