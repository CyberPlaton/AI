#pragma once

#include<omp.h>
#include<sstream>
#include<memory>
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<thread>
#include<random>
#include<stdexcept>
#include<stdarg.h> // for custom printf function.



char* _printf_convert(unsigned int num, int base)
{
	static char Representation[] = "0123456789ABCDEF";
	static char buffer[50];
	char* ptr;

	ptr = &buffer[49];
	*ptr = '\0';

	do
	{
		*--ptr = Representation[num % base];
		num /= base;
	} while (num != 0);

	return(ptr);
}

void _printf(char* format, ...)
{
	char* traverse;
	int i;
	char* s;
	std::string output;

	//Module 1: Initializing Myprintf's arguments 
	va_list arg;
	va_start(arg, format);

	for (traverse = format; *traverse != '\0'; traverse++)
	{
		while (*traverse != '%' && *traverse != '\0')
		{
			output += *traverse; // Append char to current string.
			traverse++;
		}

		traverse++;

		//Module 2: Fetching and executing arguments
		switch (*traverse)
		{

		case 'c': i = va_arg(arg, int);		//Fetch char argument

			output += _printf_convert((i - 48), 10); // Variables under %c should be from '1'|...|'9'.
			break;

		case 'd': i = va_arg(arg, int); 		//Fetch Decimal/Integer argument
			if (i < 0)
			{
				i = -i;
				output += '-';
			}
			output += _printf_convert(i, 10);
			break;

		case 'o': i = va_arg(arg, unsigned int); //Fetch Octal representation
			output += _printf_convert(i, 8);
			break;

		case 's': s = va_arg(arg, char*); 		//Fetch string
			output += s;
			break;

		case 'x': i = va_arg(arg, unsigned int); //Fetch Hexadecimal representation
			output += _printf_convert(i, 16);
			break;
		}
	}

	//Module 3: Closing argument list to necessary clean-up
	// Module 3.1: Output the generated string.
	puts(output.c_str());
	va_end(arg);
}
