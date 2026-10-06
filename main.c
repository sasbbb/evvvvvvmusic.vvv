#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// number of headers in the vvvvvvmusic.vvv file (should be 128)
#define NUM_HEADERS 128

struct header {
	char name[48];
	int32_t off_UNUSED;
	int32_t size;
	uint8_t valid;
};

int extract(const char *filename)
{
	FILE *f = fopen(filename, "rb");
	if (f == NULL) {
		fprintf(stderr, "failed to open file %s\n", filename);
		return 0;
	}

	struct header *hdr = malloc(sizeof(*hdr)*NUM_HEADERS);
	if (hdr == NULL) {
		fprintf(stderr, "failed to allocate memory\n");
		return 1;
	}

	for (int i = 0; i < NUM_HEADERS; ++i) {
		fread(&hdr[i], sizeof(*hdr), 1, f);
	}

	for (int i = 0; i < NUM_HEADERS; ++i) {
		if (!hdr[i].valid)
			continue;

		FILE *m = fopen(hdr[i].name, "wb");
		if (m == NULL) {
			fprintf(stderr, "failed to open file %s\n", hdr[i].name);
			return 1;
		}

		char *buf = malloc(hdr[i].size);
		if (buf == NULL) {
			fprintf(stderr, "failed to allocate memory\n");
			return 1;
		}

		fread(buf, 1, hdr[i].size, f);
		fwrite(buf, 1, hdr[i].size, m);
		fclose(m);
		free(buf);
		printf("extracted %s\n", hdr[i].name);
	}

	return 0;
}

int main(int argc, char *argv[])
{
	if (argc == 1) {
		printf("no filenames given; trying vvvvvvmusic.vvv\n");
		extract("vvvvvvmusic.vvv");
	}

	for (int i = 1; i < argc; ++i) {
		printf("extracting from %s\n", argv[i]);
		if (extract(argv[i]) != 0)
			break;
	}

	return 0;
}
