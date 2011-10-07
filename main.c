/*
 *       Filename:  main.c
 *
 *    Description:  log - a command-line log/journal-keeping program
 *
 *        Version:  1.0
 *        Created:  2011-09-13 22:48
 *       Compiler:  gcc
 *
 *         Author:  Ian D Brunton (ib), iandbrunton@gmail.com
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <glib.h>
#include <unistd.h>

#include "config.h"
#include "log.h"

void parse_config ();
void parse_options (int, char**);
void print_usage ();
void print_version ();
void clean_up ();

int
main (int argc, char *argv[])
{
	if (argc < 2) {
		print_usage ();
	}

	parse_config ();
	parse_options (argc, argv);
	/* compile regexes for date, time, date_diff, command_char */
	/* scan argv for matches to regexes, beginning at input (int) */
	clean_up ();
	return EXIT_SUCCESS;
}	/* ----------  end of function main  ---------- */

void
parse_config ()
{
	/* define config file */
	char *config_file;
	char *xdg_dir = getenv ("XDG_CONFIG_HOME");
	char *filename = "/logrc2";
	size_t len1 = strlen (xdg_dir);
	size_t len2 = strlen (filename);
	config_file = malloc (len1 + len2 + 2);
	memcpy (config_file, xdg_dir, len1);
	memcpy (config_file + len1, filename, len2 + 1);

	/*struct config_t *config;*/
	GKeyFile *keyfile;
	GKeyFileFlags flags;
	GError *error = NULL;
	int i;

	keyfile = g_key_file_new ();
	flags = G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS;

	if (g_key_file_load_from_file (keyfile, config_file, flags, &error) == TRUE) {
		config = g_slice_new (struct config_t);

		config->log_dir = g_key_file_get_string (keyfile, "log", "log_dir", NULL);
		config->snippet_dir = g_key_file_get_string (keyfile, "log", "snippet_dir", NULL);
		config->comment_char = g_key_file_get_string (keyfile, "log", "comment_char", NULL);
		config->editor = g_key_file_get_string (keyfile, "log", "editor", NULL);
		config->alt_editor = g_key_file_get_string (keyfile, "log", "alt_editor", NULL);
		config->comment_tag = g_key_file_get_string (keyfile, "log", "tag:comment", NULL);
		config->date_tag = g_key_file_get_string (keyfile, "log", "tag:date", NULL);
		config->end_tag = g_key_file_get_string (keyfile, "log", "tag:end", NULL);
		config->line_length = g_key_file_get_integer (keyfile, "log", "line_length", NULL);
		config->auto_round = g_key_file_get_boolean (keyfile, "log", "auto_round", NULL);
		config->auto_echo = g_key_file_get_boolean (keyfile, "log", "auto_echo", NULL);

		if (config->log_dir == NULL) {

		}

		if (config->editor == NULL) {
			config->editor = getenv ("EDITOR");
		}

		if (config->end_tag == NULL) {
		}

		if (g_key_file_has_group (keyfile, "tags") == TRUE) {
			tag_names = g_key_file_get_keys (keyfile, "tags", &num_tags, NULL);
			tags = malloc (num_tags * sizeof (struct tag_t));

			for (i = 0; i < num_tags; i++) {
				tags[i].name  = tag_names[i];
				tags[i].value = g_key_file_get_string (keyfile, "tags", tag_names[i], NULL);
			}
		}

		if (g_key_file_has_group (keyfile, "extensions") == TRUE) {
			extension_names = g_key_file_get_keys (keyfile, "extensions", &num_extensions, NULL);
			extensions = malloc (num_extensions * sizeof (struct extension_t));

			for (i = 0; i < num_extensions; i++) {
				extensions[i].name = extension_names[i];
				extensions[i].value = g_key_file_get_string (keyfile, "extensions",
						extension_names[i], NULL);
			}
		}
	} else {
		g_error (error->message);
		exit (1);
	}

	g_key_file_free (keyfile);
	free (config_file);
}	/* ----------  end of function parse_config  ---------- */

void
parse_options (int argc, char **argv)
{
	/*char *standard_opts = "+acehinqrstwx:";*/
	char *opts = malloc (strlen (standard_opts) + 3);
	memcpy (opts, "+", 1);
	memcpy (opts + 1, standard_opts, strlen (standard_opts));
	memcpy (opts + 1 + strlen (standard_opts), ":", 1);

	line_ending = '\n';

	options.print_to_file = TRUE;
	options.prepend_time = TRUE;
	options.indent = FALSE;
	options.append_time = FALSE;
	options.round_time = config->auto_round;
	options.wrap_line = TRUE;
	options.echo_output = config->auto_echo;

	int opt, x;
	while ((opt = getopt (argc, argv, opts)) != -1) {
		switch (opt) {
			case '?':
				print_usage ();
				exit (1);
				break;
			case 'a':
				line_ending = '\0';
				break;
			case 'e':
				options.print_to_file = FALSE;
				break;
			case 'h':
				print_usage ();
				exit (0);
				break;
			case 'i':
				options.prepend_time = FALSE;
				options.indent = TRUE;
				break;
			case 'n':
				options.append_time = TRUE;
				options.prepend_time = FALSE;
				break;
			case 'q':
				break;
			case 'r':
				options.round_time = TRUE;
				break;
			case 's':
				options.echo_output = FALSE;
				break;
			case 't':
				options.prepend_time = FALSE;
				break;
			case 'v':
				print_version ();
				break;
			case 'w':
				options.wrap_line = FALSE;
				break;
			case 'x':
				for (x = 0; x < num_extensions; x++) {
					if (strcmp (extensions[x].name, optarg) == 0) {
						file_extension = extensions[x].value;
					}
				}
				break;
			default:
				fprintf (stderr, "Unknown option `%c'.\n", opt);
				exit (1);
				break;
		}
	}

	input = optind;                             /* where input text begins */
}	/* ----------  end of function parse_options  ---------- */

void
print_usage ()
{
	printf ("usage: %s [-%s] [DATE] [DIFFERENTIAL] TEXT\n", APPNAME, standard_opts);
	exit (0);
}	/* ----------  end of function print_usage  ---------- */

void
print_version ()
{
	printf ("%s %s - a command-line log/journal-keeping program\n", APPNAME, VERSION);
	exit (0);
}	/* ----------  end of function print_version  ---------- */

void
clean_up ()
{
	free (extensions);
	free (tags);
}
