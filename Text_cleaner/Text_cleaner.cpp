#include <iostream>


char *GetBuffer (const char *file_name)
{
	FILE *in = fopen (file_name, "r");
	if (in == NULL) return NULL;
	fseek (in, 0, SEEK_END);
	int NChars = ftell (in);
	fseek (in, 0, SEEK_SET);
	char *text = (char *) calloc (NChars, sizeof (char));
	fread (text, sizeof (char), NChars, in);
	fclose (in);
	return text;
}


void Clean_spaces (char *buffer, const char *output_file)
{
	FILE *output = fopen (output_file, "w");
	char *tmp = buffer;
	int count = 0;

	while (*buffer != '\0')
	{
		if (*buffer == ' ' || *buffer == '\n' || *buffer == ',' || *buffer == '.' || *buffer == '!' || *buffer == '?' || *buffer == ':' || *buffer == ';')
		{
			count++;
			*buffer = '\0';
			if (*tmp != '\0') fprintf (output, "%s\n", tmp);
			tmp = buffer + 1;
		}
		if (*buffer == '’') *buffer = '\'';
		buffer++;
	}

	fclose (output);

	printf("%d words was finded\n", count);

	return;
}


int main (int argc, const char **argv)
{
	if (argc < 3)
	{
		printf("I need more args: input file and output file\n");
		return 0;
	}

	char *buffer = GetBuffer (argv[1]);
	if (buffer == NULL)
	{
		printf("Wrong file name!\n");
		return 0;
	}
	char *buf_start = buffer;

	Clean_spaces (buffer, argv[2]);

	free (buf_start);

	return 0;
}