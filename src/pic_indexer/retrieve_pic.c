#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <MagickCore/MagickCore.h>

#include <img_tools.h>
#include <avghash.h>

#include "pic_indexer.h"

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
 * Find better matching image using average hashs
 * @param[in] reference image series
 * @return analyzed indexed images count on success, -1 on error
 */
int find_image(const double *series_ref)
{
	int ret, images_count = 0, image_idx = 0, path_len;
	int dist, dist_min = INT_MAX;
	char better_file[PATH_MAX];
	unsigned char ref_hash[AVGHASH_SIZE], hash[AVGHASH_SIZE];
	char printable_hash[PRINTABLE_AVGHASH_SIZE];
	FILE *db = NULL;

	/* Compute reference image hash */
	avghash(series_ref, SERIES_SIZE, ref_hash);
	get_printable_avghash(AVGHASH_SIZE, ref_hash, printable_hash);
	fprintf(stderr, "Ref average hash: %s\n", printable_hash);

	/* Find better image among indexed ones */
	db = fopen(DB_INDEX_FILENAME, "rb");
	if (db == NULL) {
		perror("Unable to open index database file");
		ret = -1;
		goto end;
	}
	fread(&images_count, sizeof(int), 1, db);
	for (image_idx = 0; image_idx < images_count; image_idx++) {
		fread(hash, sizeof(unsigned char), AVGHASH_SIZE, db);
		dist = dist_avghash(AVGHASH_SIZE, ref_hash, hash);
		fread(&path_len, sizeof(int), 1, db);
		if (dist < dist_min) {
			dist_min = dist;
			fread(better_file, sizeof(char), path_len, db);
			better_file[path_len] = '\0';
		} else {
			fseek(db, path_len, SEEK_CUR);
		}
	}
	fprintf(stderr, "Better match: %s - distance: %d\n",
			better_file, dist_min);

	ret = images_count;

end:
	return ret;
}

int main(int argc, char **argv)
{
	int ret = EXIT_SUCCESS, images_count;
	ExceptionInfo *exception;
	Image *image;
	ImageInfo *image_info;
	double series_ref[SERIES_SIZE];

	/* Initializations */
	if (argc != 2) {
		fprintf(stdout,"Usage: %s reference_image\n", argv[0]);
		fprintf(stderr, "\timages_dir: images library absolute path\n");
		return EXIT_FAILURE;
	}
	MagickCoreGenesis(*argv, MagickTrue);
	exception = AcquireExceptionInfo();
	image_info = CloneImageInfo((ImageInfo *) NULL);

	/* Load reference image */
	strcpy(image_info->filename, argv[1]);
	image = ReadImage(image_info, exception);
	if (exception->severity != UndefinedException)
		CatchException(exception);
	if (image == (Image *) NULL) {
		fprintf(stderr, "Unable to read reference image\n");
		ret = EXIT_FAILURE;
		goto end;
	}
	if (image_to_series(image, series_ref, SERIES_SIZE)) {
		fprintf(stderr, "Unable to serialize reference image\n");
		DestroyImage(image);
		ret = EXIT_FAILURE;
		goto end;
	}
	DestroyImage(image);

	if ((images_count = find_image(series_ref)) < 0) {
		ret = EXIT_FAILURE;
		goto end;
	}
	fprintf(stderr, "Processed %d images in %fs\n",
			images_count, avghash_dist_spent_time / CLOCKS_PER_SEC);
end:
	DestroyImageInfo(image_info);
	DestroyExceptionInfo(exception);
	MagickCoreTerminus();
	return ret;
}
