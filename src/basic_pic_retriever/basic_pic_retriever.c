#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <MagickCore/MagickCore.h>

#include <img_tools.h>
#include <xcorr.h>
#include <avghash.h>

#define WORKING_WIDTH 32
#define WORKING_HEIGHT 32
#define SERIES_SIZE WORKING_WIDTH * WORKING_HEIGHT
#define AVGHASH_SIZE SERIES_SIZE / 8
#define PRINTABLE_AVGHASH_SIZE AVGHASH_SIZE * 2 + 1

/*
 * Used to filter out '.' and '..' from scandir()
 * @param[in] directory entry
 * @return 0 if directory entry has to be excluded
 */
int filter_images_dir(const struct dirent *ent)
{
	if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))
		return 0;
	return 1;
}

/*
 * Find better matching image using cross-correlation
 * @param[in] process argv
 * @param[in] reference image series
 * @return processed images count on success, -1 on error
 */
int do_xcorr(char **argv, const double *series_ref)
{
	int ret, images_count = 0, image_idx = 0;
	struct dirent **ent = NULL;
	ExceptionInfo *exception = NULL;
	Image *image;
	ImageInfo *image_info = NULL;
	double r, r_max = -1.0;
	double series[SERIES_SIZE];
	char better_file[256];

	/* Load and compute cross-correlation coefficient for stream images */
	if (chdir(argv[2])) {
		perror("Unable to access images directory");
		ret = -1;
	   goto end;
	}

	exception = AcquireExceptionInfo();
	image_info = CloneImageInfo((ImageInfo *) NULL);
	images_count = scandir(argv[2], &ent, filter_images_dir, alphasort);
	if (images_count >= 0) {
		for (image_idx = 0; image_idx < images_count; image_idx++) {
			strcpy(image_info->filename, ent[image_idx]->d_name);
			image = ReadImage(image_info, exception);
			if (exception->severity != UndefinedException)
				CatchException(exception);
			if (image == (Image *) NULL) {
				fprintf(stderr, "Unable to read %s\n",
						ent[image_idx]->d_name);
				ret = -1;
				goto end;
			}
			if (image_to_series(image, series, SERIES_SIZE)) {
				fprintf(stderr, "Unable to serialize %s\n",
						ent[image_idx]->d_name);
				DestroyImage(image);
				ret = -1;
				goto end;
			}
			r = xcorr(series_ref, series, SERIES_SIZE);
			printf("%d (%s) %lf\n",
					image_idx, ent[image_idx]->d_name, r);
			if (r > r_max) {
				r_max = r;
				strncpy(better_file,
						ent[image_idx]->d_name, 256);
			}
			DestroyImage(image);
			free(ent[image_idx]);
		}
	} else {
		perror("Unable to read images directory");
		ret = -1;
		goto end;
	}
	free(ent);
	ent = NULL;
	fprintf(stderr, "Better match: %s - coeff: %f\n", better_file, r_max);
	ret = images_count;

end:
	if (image_info)
		DestroyImageInfo(image_info);
	if (exception)
		DestroyExceptionInfo(exception);
	if (ent) {
		for (; image_idx < images_count; image_idx++) {
			free(ent[image_idx]);
		};
		free(ent);
	}
	return ret;
}

/*
 * Find better matching image using average hashs
 * @param[in] process argv
 * @param[in] reference image series
 * @return processed images count on success, -1 on error
 */
int do_avghash(char **argv, const double *series_ref)
{
	int ret, images_count = 0, image_idx = 0;
	struct dirent **ent = NULL;
	ExceptionInfo *exception = NULL;
	Image *image;
	ImageInfo *image_info = NULL;
	int dist, dist_min = INT_MAX;
	double series[SERIES_SIZE];
	char better_file[256];
	unsigned char hash[AVGHASH_SIZE], ref_hash[AVGHASH_SIZE];
	char printable_hash[PRINTABLE_AVGHASH_SIZE];

	/* Compute reference image hash */
	avghash(series_ref, SERIES_SIZE, ref_hash);
	get_printable_avghash(AVGHASH_SIZE, ref_hash, printable_hash);
	fprintf(stderr, "Ref average hash: %s\n", printable_hash);

	/* Load and compute hash for stream images */
	if (chdir(argv[2])) {
		perror("Unable to access images directory");
		ret = -1;
	   goto end;
	}
	exception = AcquireExceptionInfo();
	image_info = CloneImageInfo((ImageInfo *) NULL);
	images_count = scandir(argv[2], &ent, filter_images_dir, alphasort);
	if (images_count >= 0) {
		for (image_idx = 0; image_idx < images_count; image_idx++) {
			strcpy(image_info->filename, ent[image_idx]->d_name);
			image = ReadImage(image_info, exception);
			if (exception->severity != UndefinedException)
				CatchException(exception);
			if (image == (Image *) NULL) {
				fprintf(stderr, "Unable to read %s\n",
						ent[image_idx]->d_name);
				ret = -1;
				goto end;
			}
			if (image_to_series(image, series, SERIES_SIZE)) {
				fprintf(stderr, "Unable to serialize %s\n",
						ent[image_idx]->d_name);
				DestroyImage(image);
				ret = -1;
				goto end;
			}
			avghash(series, SERIES_SIZE, hash);
			dist = dist_avghash(AVGHASH_SIZE, ref_hash, hash);
			get_printable_avghash(AVGHASH_SIZE, hash, printable_hash);
			printf("%d (%s) %d %s\n",
					image_idx,
					ent[image_idx]->d_name, dist,
					printable_hash);
			if (dist < dist_min) {
				dist_min = dist;
				strncpy(better_file, ent[image_idx]->d_name, 256);
			}
			DestroyImage(image);
			free(ent[image_idx]);
		}
	} else {
		perror("Unable to read images directory");
		ret = -1;
		goto end;
	}
	free(ent);
	ent = NULL;
	fprintf(stderr, "Better match: %s - distance: %d\n",
			better_file, dist_min);

	ret = images_count;

end:
	if (image_info)
		DestroyImageInfo(image_info);
	if (exception)
		DestroyExceptionInfo(exception);
	if (ent) {
		for (; image_idx < images_count; image_idx++) {
			free(ent[image_idx]);
		};
		free(ent);
	}
	return ret;
}

void usage(char *bin_name) {
	fprintf(stderr, "Usage: %s <method> <images_dir> <reference_image>\n",
			bin_name);
	fprintf(stderr, "\tmethod: image comparison method (xcorr or avghash)\n");
	fprintf(stderr, "\timages_dir: images library absolute path\n");
	fprintf(stderr, "\treference_image: image for which we want to find the closed in images_dir\n");
}

int main(int argc, char **argv)
{
	int ret = EXIT_SUCCESS, images_count;
	ExceptionInfo *exception;
	Image *image;
	ImageInfo *image_info;
	double series_ref[SERIES_SIZE];

	/* Initializations */
	if (argc != 4) {
		usage(argv[0]);
		return EXIT_FAILURE;
	}
	MagickCoreGenesis(*argv, MagickTrue);
	exception = AcquireExceptionInfo();
	image_info = CloneImageInfo((ImageInfo *) NULL);

	/* Load reference image */
	strcpy(image_info->filename, argv[3]);
	image = ReadImage(image_info, exception);
	if (exception->severity != UndefinedException)
		CatchException(exception);
	if (image == (Image *) NULL) {
		fprintf(stderr, "Unable to read reference image\n");
		usage(argv[0]);
		ret = EXIT_FAILURE;
		goto end;
	}
	if (image_to_series(image, series_ref, SERIES_SIZE)) {
		fprintf(stderr, "Unable to serialize reference image\n");
		usage(argv[0]);
		DestroyImage(image);
		ret = EXIT_FAILURE;
		goto end;
	}
	DestroyImage(image);

	if (!strcmp(argv[1], "xcorr")) {
		if ((images_count = do_xcorr(argv, series_ref)) < 0) {
			ret = EXIT_FAILURE;
			goto end;
		}
		fprintf(stderr, "Processed %d images in %fs\n",
				images_count,
				xcorr_spent_time);
	} else if (!strcmp(argv[1], "avghash")) {
		if ((images_count = do_avghash(argv, series_ref)) < 0) {
			ret = EXIT_FAILURE;
			goto end;
		}
		fprintf(stderr, "Processed %d images in %fs (hashes computation) +  %fs (hashes comparison)\n",
				images_count,
				avghash_spent_time,
				avghash_dist_spent_time);
	} else {
		fprintf(stderr, "Unknown method %s\n", argv[1]);
		usage(argv[0]);
		ret = EXIT_FAILURE;
		goto end;
	}
end:
	DestroyImageInfo(image_info);
	DestroyExceptionInfo(exception);
	MagickCoreTerminus();
	return ret;
}
