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

int file_exists(char *path)
{
	FILE *f;
	if ((f = fopen(path, "r")) == NULL)
		return 0;
	fclose(f);
	return 1;
}

/*
 * Print progression
 * @param[in] current index
 * @param[in] total to proceed
 */
void print_progress(int cur_idx, int total)
{
	int i, pc;
	if (cur_idx % (total / 100) == 0) {
		pc = cur_idx * 100 / total;
		printf(" [");
		for (i = 0; i <= pc / 10; i++)
			printf("=");
		for (; i < 10; i++)
			printf(" ");
		printf("]");
		printf(" %d%%\r", pc);
		fflush(stdout);
	}
}

/*
 * Index folder pictures using average hashes
 * @param[in] process argv
 * @return processed images count on success, -1 on error
 */
int index_folder(char *folder_path)
{
	int ret, images_count = 0, image_idx = 0, db_images_count, path_len;
	struct dirent **ent = NULL;
	ExceptionInfo *exception = NULL;
	Image *image;
	ImageInfo *image_info = NULL;
	double series[SERIES_SIZE];
	unsigned char hash[AVGHASH_SIZE];
	char printable_hash[PRINTABLE_AVGHASH_SIZE], cwd[PATH_MAX];
	FILE *db = NULL;

	/* Open the index database file */
	db = fopen(DB_INDEX_FILENAME, "ab");
	if (db == NULL) {
		perror("Unable to open index database file");
		ret = -1;
		goto end;
	}
	/* 0 images indexed */
	if (!ftell(db))
		fwrite(&images_count, sizeof(int), 1, db);

	/* Load and compute hash for stream images */
	if (!getcwd(cwd, PATH_MAX)) {
		perror("Unable to get current working directory");
		ret = -1;
		goto end;
	}
	if (chdir(folder_path)) {
		perror("Unable to access images directory");
		ret = -1;
		goto end;
	}
	exception = AcquireExceptionInfo();
	image_info = CloneImageInfo((ImageInfo *) NULL);
	images_count = scandir(folder_path, &ent, filter_images_dir, alphasort);
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
			DestroyImage(image);
			avghash(series, SERIES_SIZE, hash);
			get_printable_avghash(AVGHASH_SIZE, hash,
					printable_hash);
			fwrite(hash, sizeof(unsigned char), AVGHASH_SIZE, db);
			path_len = strlen(folder_path) + 1
				+ strlen(ent[image_idx]->d_name);
			fwrite(&path_len, sizeof(int), 1, db);
			fprintf(db, "%s/%s", folder_path,
					ent[image_idx]->d_name);
			free(ent[image_idx]);
			print_progress(image_idx, images_count);
		}
	} else {
		perror("Unable to read images directory");
		ret = -1;
		goto end;
	}
	free(ent);
	ent = NULL;

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
	if (db) {
		fclose(db);
		if (chdir(cwd)) {
			perror("Unable to return to working directory");
			ret = -1;
			goto end;
		}
		db = fopen(DB_INDEX_FILENAME, "r+b");
		if (db == NULL) {
			perror("Unable to open index database file to update images count");
			ret = -1;
			goto end;
		}
		fread(&db_images_count, sizeof(int), 1, db);
		rewind(db);
		db_images_count += image_idx;
		fwrite(&db_images_count, sizeof(int), 1, db);
		fclose(db);
	}
	return ret;
}

int main(int argc, char **argv)
{
	int ret = EXIT_SUCCESS, images_count;

	/* Initializations */
	if (argc != 2) {
		fprintf(stderr,"Usage: %s images_dir\n", argv[0]);
		fprintf(stderr, "\timages_dir: images library absolute path\n");
		return EXIT_FAILURE;
	}
	MagickCoreGenesis(*argv, MagickTrue);

	if ((images_count = index_folder(argv[1])) < 0) {
		ret = EXIT_FAILURE;
		goto end;
	}
	fprintf(stderr, "Indexed %d images in %fs\n",
			images_count, avghash_spent_time / CLOCKS_PER_SEC);

end:
	MagickCoreTerminus();
	return ret;
}
