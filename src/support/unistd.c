#include <sys/types.h>

int ftruncate(int fildes, off_t length) {
	(void)fildes;
	(void)length;
	return -1;
}
