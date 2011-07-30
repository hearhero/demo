#include <stdio.h>

struct date1
{
	short a:4;
	short b:5;
	short c:4;
}DATE1;

struct date2
{
	short a:4;
	short b:5;
	short c:8;
}DATE2;

struct date3
{
	short a:4;
	short b:5;
	int   c:8;
}DATE3;

int main(int argc, char *argv[])
{
	DATE1.a = 15;
	DATE1.b = 31;
	DATE1.c = 14;

	DATE2.a = 15;
	DATE2.b = 31;
	DATE2.c = 254;

	DATE3.a = 15;
	DATE3.b = 31;
	DATE3.c = 254;

	printf(" %d\n %d\n %d\n",sizeof(struct date1), 
			sizeof(struct date2), sizeof(struct date3));

	return 0;
}
