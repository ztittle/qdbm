#include <stdio.h>
#include <sys/types.h>
#include <errno.h>

int ftruncate(int fildes, off_t length) {
	if (length == 0) {
		FILE *file = (FILE*)fdopen(fildes, "w");
		if (file == NULL) {
			return -1;
		}

		return 0;
	}

	errno = ENOSYS;

	return -1;
}
