/*
Picor - Average hash header
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

#ifndef _AVGHASH_H_
#define _AVGHASH_H_

extern double avghash_spent_time;
extern double avghash_dist_spent_time;

/*
 * Compute average hash coefficient of a series
 * Note: updates avghash_spent_time
 * @param[in] series
 * @param[in] series size
 * @param[out] average hash
 */
void avghash(const double *s, int n, unsigned char *h);

/*
 * Compute Hamming distance between two average hashes
 * Note: updates avghash_dist_spent_time
 * @param[in] hash size
 * @param[in] first hash
 * @param[in] second hash
 * @return Hamming distance
 */
int dist_avghash(int n, const unsigned char *h1, const unsigned char *h2);

/*
 * Get a printable average hash (as string)
 * @param[in] hash size
 * @param[in] hash
 * @param[out] printable hash string
 */
void get_printable_avghash(int n, const unsigned char *h, char *pr_h);

#endif
