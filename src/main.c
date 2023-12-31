#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <argp.h>
#include <openssl/rand.h>

#define MAX_PASSWORDS 2048
#define MIN_PASSWORDS 1

void clamp(int *value, int min, int max)
{
	if (*value < min) {
		*value = min;
	} else if (*value > max) {
		*value = max;
	}
}

int get_random_int(void)
{
	int random_integer;

	/* Generate a random integer using OpenSSL's RAND_bytes */
	if (RAND_bytes((unsigned char*)&random_integer, sizeof(random_integer)) != 1) {
		perror("Error generating random bytes");
		exit(EXIT_FAILURE);
	}

	return abs(random_integer);
}

/* Generate a random password */
char *generate_password()
{
	char lowercase_letters[] = "abcdefghijklmnopqrstuvwxyz";
	char uppercase_letter = (char)('A' + get_random_int() % 26);
	char number = (char)('0' + get_random_int() % 10);
	char letters[19];

	int i = 0;
	/* Shuffle lowercase letters array */
	for (i = 25; i > 0; i--) {
		int j = get_random_int() % (i + 1);
		char temp = lowercase_letters[i];
		lowercase_letters[i] = lowercase_letters[j];
		lowercase_letters[j] = temp;
	}

	/* Populate 'letters' array with shuffled lowercase letters */
	for (i = 0; i < 6; i++) {
		letters[i] = lowercase_letters[i];
		letters[i + 6] = lowercase_letters[i + 6];
		letters[i + 12] = lowercase_letters[i + 12];
	}

	/* Place uppercase letter and number in 'letters' array */
	letters[get_random_int() % 6] = uppercase_letter;
	letters[6 + get_random_int() % 6] = uppercase_letter;
	letters[12 + get_random_int() % 6] = number;
	letters[18] = '\0';

	/* Allocate memory for password and format it */
	char *password = (char *)malloc(22);
	if (password != NULL) {
		snprintf(password, 22, "%.*s-%.*s-%.*s", 6, letters, 6, letters + 6, 6, letters + 12);
	} else {
		perror("Error while generating the password");
		exit(EXIT_FAILURE);
	}
	return password;
}

const char *argp_program_version = "cpasswordgenerator 0.2.1";
const char *argp_program_bug_address = "<walker84837@gmail.com>";

static char doc[] = "Generate random passwords";
static char args_doc[] = "[OPTION]...";

struct arguments
{
	int num_passwords;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	struct arguments *arguments = (struct arguments *)state->input;

	switch (key) {
		case 'n':
			arguments->num_passwords = atoi(arg);
			if (arguments->num_passwords <= 0) {
				fputs("Invalid number of passwords\n", stderr);
				exit(EXIT_FAILURE);
			}
			break;
		case ARGP_KEY_ARG:
			break;
		case ARGP_KEY_END:
			clamp(&arguments->num_passwords, MIN_PASSWORDS, MAX_PASSWORDS);
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp_option options[] = {
	{"num_passwords", 'n', "NUM_PASSWORDS", 0, "Number of passwords to generate", 0},
	{0}
};

static struct argp argp = {options, parse_opt, args_doc, doc};

int main(int argc, char *argv[])
{
	struct arguments args;
	args.num_passwords = 1;

	/* Parse command-line arguments */
	argp_parse(&argp, argc, argv, 0, 0, &args);

	/* Generate and print passwords */
	int i = 0;
	for (i = 0; i < args.num_passwords; i++) {
		char *password = generate_password();
		if (password != NULL) {
			puts(password);
			free(password);
		} else {
			fputs("Failed to allocate memory for password.\n", stderr);
			return 1;
		}
	}

	return 0;
}

