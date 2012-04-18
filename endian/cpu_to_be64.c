#include <stdio.h>
#include <stdint.h>
#include <endian.h>
#include <sys/types.h>

#include <byteorder/generic.h>

int main()
{
	int64_t test = 0x1234567890abcdefLL;

	printf("%llx\n", test);

	test = htobe64(test);

	printf("%llx\n", test);

	test = EXTRACT_64BITS(test);

	printf("%llx\n", test);

	return 0;
}
