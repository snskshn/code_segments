#include <sys/types.h>
#include <dirent.h>

int main()
{
	DIR *dir = opendir("temp_ssson_dir");

	if (!dir) {
		mkdir("temp_ssson_dir", 0755);
	}

	return 0;
}
