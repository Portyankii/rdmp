#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
print_usage (const char *progname)
{
  fprintf (stderr, "Usage: %s [-b] [input_file]\n", progname);
  fprintf (stderr,
           "       -b : Interpret input as binary instead of hexadecimal.\n");
  exit (EXIT_FAILURE);
}

int
hex_to_byte (char high, char low)
{
  if (!isxdigit (high) || !isxdigit (low))
    {
      fprintf (stderr, "Invalid hex digit: %c%c\n", high, low);
      exit (EXIT_FAILURE);
    }
  int h = isdigit (high) ? high - '0' : tolower (high) - 'a' + 10;
  int l = isdigit (low) ? low - '0' : tolower (low) - 'a' + 10;
  return (h << 4) | l;
}

int
bin_to_byte (const char *bin)
{
  int byte = 0;
  for (int i = 0; i < 8; i++)
    {
      if (bin[i] != '0' && bin[i] != '1')
        {
          fprintf (stderr, "Invalid binary digit: %c\n", bin[i]);
          exit (EXIT_FAILURE);
        }
      byte = (byte << 1) | (bin[i] - '0');
    }
  return byte;
}

void
process_hex (FILE *input)
{
  char buffer[3] = { 0 };
  while (fscanf (input, "%2s", buffer) == 1)
    {
      unsigned char byte = hex_to_byte (buffer[0], buffer[1]);
      fwrite (&byte, 1, 1, stdout);
    }
}

void
process_binary (FILE *input)
{
  char buffer[9] = { 0 };
  while (fscanf (input, "%8s", buffer) == 1)
    {
      unsigned char byte = bin_to_byte (buffer);
      fwrite (&byte, 1, 1, stdout);
    }
}

int
main (int argc, char *argv[])
{
  int binary_mode = 0;
  const char *input_file = NULL;

  // Parse command-line arguments
  for (int i = 1; i < argc; i++)
    {
      if (strcmp (argv[i], "-b") == 0)
        {
          binary_mode = 1;
        }
      else if (argv[i][0] == '-')
        {
          print_usage (argv[0]);
        }
      else
        {
          if (input_file)
            {
              print_usage (argv[0]); // Only one input file allowed
            }
          input_file = argv[i];
        }
    }

  FILE *input = stdin;
  if (input_file)
    {
      input = fopen (input_file, "r");
      if (!input)
        {
          perror ("Error opening input file");
          exit (EXIT_FAILURE);
        }
    }

  if (binary_mode)
    {
      process_binary (input);
    }
  else
    {
      process_hex (input);
    }

  if (input != stdin)
    {
      fclose (input);
    }

  return EXIT_SUCCESS;
}
