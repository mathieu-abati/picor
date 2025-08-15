#ifndef _XCORR_H_
#define _XCORR_H_

extern double xcorr_spent_time;

/*
 * Compute cross-correlation coefficient of two series of same size
 * Note: updates xcorr_spent_time
 * @param[in] 1st series
 * @param[in] 2nd series
 * @param[in] series size
 * @return cross-correlation coefficient
 */
double xcorr(const double *x, const double *y, int n);

#endif
