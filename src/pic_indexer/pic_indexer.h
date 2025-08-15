#ifndef _PIC_INDEXER_H_
#define _PIC_INDEXER_H_

#define WORKING_WIDTH 32
#define WORKING_HEIGHT 32
#define SERIES_SIZE WORKING_WIDTH * WORKING_HEIGHT
#define AVGHASH_SIZE SERIES_SIZE / 8
#define PRINTABLE_AVGHASH_SIZE AVGHASH_SIZE * 2 + 1

#define DB_INDEX_FILENAME "db.idx"

/*
Indexes database format:
- (int x 1) indexed images count
- (unsigned char x AVGHASH_SIZE) image 1 average hash
- (int x 1) image 1 path length
- (char x path_len) image 1 path
- (unsigned char x AVGHASH_SIZE) image 2 average hash
- (int x 1) image 2 path length
- (char x path_len) image 2 path
*/

#endif
