/*
 *       Filename:  config.h
 *
 *    Description:  configuration options
 *
 *        Version:  1.0
 *        Created:  2011-09-13 18:29
 *       Compiler:  gcc
 *
 *         Author:  Ian D Brunton (ib), iandbrunton@gmail.com
 *
 */
#ifndef CONFIG_H
#define CONFIG_H


#include <glib.h>

typedef enum {
	False,
	True
} cfg_bool;

struct config_t {
	char *log_dir;
	char *snippet_dir;
	char *comment_char;
	char *editor;
	char *alt_editor;
	char *comment_tag;
	char *date_tag;
	char *end_tag;
	gboolean auto_round;
	gboolean auto_echo;
	int line_length;
};

struct config_t *config;

struct tag_t {
	gchar *name;
	gchar *value;
};

struct tag_t *tags;
gsize num_tags;
gchar **tag_names;
gchar **tag_values;

struct extension_t {
	gchar *name;
	gchar *value;
};

struct extension_t *extensions;
gsize num_extensions;
gchar **extension_names;
gchar **extension_values;

int input;

/* command-line options: */
char *standard_opts = "acehinqrstvwx";
char *file_extension;
char line_ending;

struct option_t {
	gboolean print_to_file;
	gboolean prepend_time;
	gboolean indent;
	gboolean append_time;
	gboolean round_time;
	gboolean wrap_line;
	gboolean echo_output;
};

struct option_t options;

#endif
