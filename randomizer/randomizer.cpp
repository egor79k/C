#include <iostream>
#include <ctime>

using namespace std;

const char file_name[] = "random.txt";

 
void int_massiv (int from, int to, int num)
{
	FILE *output = fopen (file_name, "w");
	fprintf (output, "::::This numbers were generated with RANDOMIZER.EXE::::\n::::::::::::::::Program by ELKI(c) 2020::::::::::::::::\n\n");
	fprintf (output, "%d random numbers in range from %d to %d:\n\n", num, from, to);
	srand (time (0));
	for (int i = 0; i < num; ++i) fprintf (output, "%d ", from + rand () % (to - from + 1));
	fclose (output);
	return;
}

 
int main()
{
	int from = 0;
	int to = 0;
	int num = 0;

	printf ("Range from: ");
	cin >> from;

	printf ("Range to: ");
	cin >> to;

	printf ("Number of numbers: ");
	cin >> num;

	int_massiv(from, to, num);
}