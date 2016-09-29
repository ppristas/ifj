#include <stdio.h>

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		return 99;
	}
	if((subor_docasny_nazov = fopen(argv[1], "r")) == NULL )
	{
		return 99;
	}
}