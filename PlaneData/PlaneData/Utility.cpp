#include "Utility.h"
#include <fstream>

void Utility::strcpy_safe(char* destination, const char* source, const int& size)
{
	int i;
	for (i = 0; i < size - 1 && source[i] != '\0'; i++)
	{
		destination[i] = source[i];
	}

	destination[i] = '\0';
}

bool Utility::doesFileExist(const char* path)
{
	std::ifstream input(path);
	bool status = input.good();
	input.close();

	return status;
}

void Utility::createFile(const char* path)
{
	std::ofstream output(path);
	output.close();
}

bool Utility::compare(const char* str1, const char* str2)
{
	for (int i = 0; str1[i] != '\0' || str2[i] != '\0'; i++)
	{
		if (str1[i] != str2[i])
			return false;
	}

	return true;
}

void Utility::toLowercase(char* string)
{
	while (*string != '\0')
	{
		if (*string >= 'A' && *string <= 'Z')
			*string += 'a' - 'A';

		string++;
	}
}

int Utility::getRandom(int start, int end)
{
	return rand() % end + start;
}