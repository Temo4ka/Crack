#pragma once

#define ERR_EXE(ERROR_CODE) {							   \
	if (ERROR_CODE) {										\
		fprintf(stderr, "An error %d occured", ERROR_CODE);  \
		return EXIT_FAILURE;								  \
	}														   \
}																
	