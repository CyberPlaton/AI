#include "test_suite_alpha_one.h"

int main()
{
	bool success = false;

	success = test_suite_alpha_one();
	success &= test_suite_alpha_two();

	return success;
}