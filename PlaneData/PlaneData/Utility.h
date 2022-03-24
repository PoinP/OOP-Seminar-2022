#ifndef UTILITY

#define UTILITY

namespace Utility
{
	void strcpy_safe(char* destination, const char* source, const int& size);
	bool doesFileExist(const char* path);
	void createFile(const char* path);
	void toLowercase(char* string);
	bool compare(const char* str1, const char* str2);

	int getRandom(int start, int end);
}

#endif