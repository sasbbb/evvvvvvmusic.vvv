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

int main(void)
{
	FILE *f = fopen("vvvvvvmusic.vvv", "rb");
	if (f == NULL) {
		fprintf(stderr, "failed to open file %s\n", "vvvvvvmusic.vvv");
		return 0;
	}
	struct header *hdr = malloc(sizeof(*hdr)*NUM_HEADERS);
	if (hdr == NULL) {
		fprintf(stderr, "failed to allocate memory\n");
		return 0;
	}

	for (int i = 0; i < NUM_HEADERS; ++i) {
		fread(&hdr[i], 1, sizeof(*hdr), f);
	}

	for (int i = 0; i < NUM_HEADERS; ++i) {
		if (!hdr[i].valid)
			continue;
		FILE *m = fopen(hdr[i].name, "wb");
		if (m == NULL) {
			fprintf(stderr, "failed to open file %s\n", hdr[i].name);
			break;
		}
		char *buf = malloc(hdr[i].size);
		if (buf == NULL) {
			fprintf(stderr, "failed to allocate memory\n");
			break;
		}
		fread(buf, 1, hdr[i].size, f);
		fwrite(buf, 1, hdr[i].size, m);
		fclose(m);
		free(buf);
		printf("extracted \"%s\"\n", hdr[i].name);
	}
	free(hdr);
	fclose(f);
	return 0;
}
