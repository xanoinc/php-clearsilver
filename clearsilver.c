/*
  +----------------------------------------------------------------------+
  | PHP Version 4                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2003 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.0 of the PHP license,       |
  | that is bundled with this package in the file LICENSE, and is        |
  | available at through the world-wide-web at                           |
  | http://www.php.net/license/3_0.txt.                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Homme Zwaagstra (hrz@geodata.soton.ac.uk)                    |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define refcount refcount__gc
#define is_ref is_ref__gc

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_clearsilver.h"

#include "ClearSilver.h"

/***********************************************************************
 * NOTE FOR FUTURE DEVELOPMENT: 
 * Check out
 * http://cvs.sourceforge.net/viewcvs.py/swishe/swish_php/swishe.c?rev=1.4
 * for example of simple OO interface
 ***********************************************************************/

/* If you declare any globals in php_clearsilver.h uncomment this:
   ZEND_DECLARE_MODULE_GLOBALS(clearsilver)
*/

#define CLEARSILVER_EXTENSION_VERSION "0.3"

/* True global resources - no need for thread safety here */
static int le_clearsilver_hdf;
static void clearsilver_hdf_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	HDF *hdf = (HDF *) rsrc->ptr;
	hdf_destroy(&hdf);
}

static int le_clearsilver_cs;
static void clearsilver_cs_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	CSPARSE *parse = (CSPARSE *) rsrc->ptr;
	cs_destroy(&parse);
}

static NEOERR *render_cb (void *ctx, char *buf)
{
	STRING *str= (STRING *)ctx;

	return nerr_pass(string_append(str, buf));
}

char * neo_error_to_string (NEOERR *err)
{
	STRING str;
	char * err_str = NULL;

	string_init (&str);

	if (nerr_match(err, NERR_PARSE)) {
		nerr_error_string (err, &str);
	} else {
		nerr_error_traceback (err, &str);
	}

	err_str = estrdup(str.buf);
  
	string_clear (&str);

	return err_str;
}


/* {{{ clearsilver_functions[]
 *
 * Every user visible function must have an entry in clearsilver_functions[].
 */
function_entry clearsilver_functions[] = {
	PHP_FE(hdf_init,	NULL)
	PHP_FE(hdf_read_file,	NULL)
	PHP_FE(hdf_write_file,	NULL)
	PHP_FE(hdf_write_string,  NULL)
	PHP_FE(hdf_set_value,	NULL)
	PHP_FE(hdf_get_value,	NULL)
	PHP_FE(hdf_get_node,	NULL)
	PHP_FE(hdf_get_obj,	NULL)
	PHP_FE(hdf_obj_name,	NULL)
	PHP_FE(hdf_obj_value,	NULL)
	PHP_FE(hdf_obj_child,	NULL)
	PHP_FE(hdf_obj_next,	NULL)
	PHP_FE(hdf_get_child,	NULL)
	PHP_FE(hdf_destroy,	NULL)
	PHP_FE(cs_init,	NULL)
	PHP_FE(cs_parse_string,	NULL)
	PHP_FE(cs_parse_file,	NULL)
	PHP_FE(cs_render,	NULL)
	PHP_FE(cs_destroy,	NULL)
	{NULL, NULL, NULL}	/* Must be the last line in clearsilver_functions[] */
};
/* }}} */

/* {{{ clearsilver_module_entry
 */
zend_module_entry clearsilver_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"clearsilver",
	clearsilver_functions,
	PHP_MINIT(clearsilver),
	PHP_MSHUTDOWN(clearsilver),
	NULL,		/* Replace with NULL if there's nothing to do at request start */
	NULL,	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(clearsilver),
#if ZEND_MODULE_API_NO >= 20010901
	CLEARSILVER_EXTENSION_VERSION, /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_CLEARSILVER
ZEND_GET_MODULE(clearsilver)
#endif

	 /* {{{ PHP_INI
	  */
	 /* Remove comments and fill if you need to have entries in php.ini
		PHP_INI_BEGIN()
		STD_PHP_INI_ENTRY("clearsilver.global_value",      "42", PHP_INI_ALL, OnUpdateInt, global_value, zend_clearsilver_globals, clearsilver_globals)
		STD_PHP_INI_ENTRY("clearsilver.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_clearsilver_globals, clearsilver_globals)
		PHP_INI_END()
	 */
	 /* }}} */

	 /* {{{ php_clearsilver_init_globals
	  */
	 /* Uncomment this function if you have INI entries
		static void php_clearsilver_init_globals(zend_clearsilver_globals *clearsilver_globals)
		{
		clearsilver_globals->global_value = 0;
		clearsilver_globals->global_string = NULL;
		}
	 */
	 /* }}} */

	 /* {{{ PHP_MINIT_FUNCTION
	  */
	 PHP_MINIT_FUNCTION(clearsilver)
{
	/* If you have INI entries, uncomment these lines 
	   ZEND_INIT_MODULE_GLOBALS(clearsilver, php_clearsilver_init_globals, NULL);
	   REGISTER_INI_ENTRIES();
	*/
	le_clearsilver_hdf = zend_register_list_destructors_ex(clearsilver_hdf_dtor,
														   NULL,
														   "ClearSilver HDF",
														   module_number);

	le_clearsilver_cs = zend_register_list_destructors_ex(clearsilver_cs_dtor,
														  NULL,
														  "ClearSilver CS",
														  module_number);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(clearsilver)
{
	/* uncomment this line if you have INI entries
	   UNREGISTER_INI_ENTRIES();
	*/

	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(clearsilver)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(clearsilver)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(clearsilver)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "ClearSilver support", "enabled");
	php_info_print_table_header(2, "Extension Version", CLEARSILVER_EXTENSION_VERSION);
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	   DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ proto resource hdf_init()
   Initialize an HDF data set */
PHP_FUNCTION(hdf_init)
{
	HDF *hdf;
	NEOERR *err;

	if (ZEND_NUM_ARGS() != 0) {
		WRONG_PARAM_COUNT;
	}

	err = hdf_init(&hdf);
	
	if (err != STATUS_OK) {
		php_error(E_WARNING, neo_error_to_string(err));
		RETURN_NULL();
	}

	ZEND_REGISTER_RESOURCE(return_value, hdf, le_clearsilver_hdf);
}
/* }}} */

/* {{{ proto bool hdf_read_file(resource hdf, string path)
   Read an HDF data file */
PHP_FUNCTION(hdf_read_file)
{
	zval *zhdf = NULL;
	char *path = NULL;
	int argc = ZEND_NUM_ARGS();
	int path_len;
	HDF *hdf = NULL;
	NEOERR *err = NULL;

	if (zend_parse_parameters(argc TSRMLS_CC, "rs", &zhdf, &path, &path_len) == FAILURE) 
		return;

	ZEND_FETCH_RESOURCE(hdf, HDF *, &zhdf, -1, "ClearSilver HDF", le_clearsilver_hdf);

	if (!hdf) {
		php_error(E_WARNING, "failed to retrieve HDF resource");
		RETURN_FALSE;
	}
	
	err = hdf_read_file(hdf, path);

	if (err != STATUS_OK) {
		php_error(E_WARNING, neo_error_to_string(err));
		RETURN_FALSE;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool hdf_write_file(resource hdf, string path)
   Write an HDF data set to disk */
PHP_FUNCTION(hdf_write_file)
{
	zval *zhdf = NULL;
	char *path = NULL;
	int argc = ZEND_NUM_ARGS();
	int path_len;
	HDF *hdf = NULL;
	NEOERR *err = NULL;

	if (zend_parse_parameters(argc TSRMLS_CC, "rs", &zhdf, &path, &path_len) == FAILURE) 
		return;

	ZEND_FETCH_RESOURCE(hdf, HDF *, &zhdf, -1, "ClearSilver HDF", le_clearsilver_hdf);

	if (!hdf) {
		php_error(E_WARNING, "failed to retrieve HDF resource");
		RETURN_FALSE;
	}
	
	err = hdf_write_file(hdf, path);

	if (err != STATUS_OK) {
		php_error(E_WARNING, neo_error_to_string(err));
		RETURN_FALSE;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto string hdf_write_string(resource hdf)
   Serialize an HDF dataset to a string */
PHP_FUNCTION(hdf_write_string)
{
	zval *zhdf = NULL;
	int argc = ZEND_NUM_ARGS();
	HDF *hdf = NULL;
	NEOERR *err = NULL;
	char *str = NULL;

	if (zend_parse_parameters(argc TSRMLS_CC, "r", &zhdf) == FAILURE) 
		return;

	ZEND_FETCH_RESOURCE(hdf, HDF *, &zhdf, -1, "ClearSilver HDF", le_clearsilver_hdf);

	if (!hdf) {
		php_error(E_WARNING, "failed to retrieve HDF resource");
		RETURN_FALSE;
	}
	
	err = hdf_write_string(hdf, &str);

	if (err != STATUS_OK) {
		php_error(E_WARNING, neo_error_to_string(err));
		RETURN_NULL();
	}

	RETURN_STRING(str, 1);
}
/* }}} */

/*  code modified from php_array_merge() in ext/standard/array.c in PHP */
int hdf_set_array(HDF *hdf, HashTable *src TSRMLS_DC)
{
	zval	  **src_entry;
	char	   *string_key;
	uint		string_key_len;
	ulong		num_key;
	HashPosition pos;
	NEOERR *err = NULL;
	HDF *node = NULL;			/* a potential child node */
	char buf[100];
	
	zend_hash_internal_pointer_reset_ex(src, &pos);
	while (zend_hash_get_current_data_ex(src, (void **)&src_entry, &pos) == SUCCESS) {
		switch (zend_hash_get_current_key_ex(src, &string_key, &string_key_len, &num_key, 0, &pos)) {
		case HASH_KEY_IS_STRING:
			if ((*src_entry)->refcount > 1) {
				php_error(E_WARNING, "%s(): recursion detected", get_active_function_name(TSRMLS_C));
				return 0;
			}
			
			SEPARATE_ZVAL(src_entry);

			if (Z_TYPE_PP(src_entry) != IS_ARRAY) {
				convert_to_string_ex(src_entry);
				//php_printf("key='%s' value='%s'\n", string_key, Z_STRVAL_PP(src_entry));

				err = hdf_set_value(hdf, string_key, Z_STRVAL_PP(src_entry));
				if (err != STATUS_OK) {
					php_error(E_WARNING, neo_error_to_string(err));
					return 0;
				}
			} else {
				convert_to_array_ex(src_entry);

				err = hdf_get_node(hdf, string_key, &node);
				if (err != STATUS_OK) {
					php_error(E_WARNING, neo_error_to_string(err));
					return 0;
				}
						
				if (!hdf_set_array(node, Z_ARRVAL_PP(src_entry) TSRMLS_CC))
					return 0;
			}
					
			break;

		case HASH_KEY_IS_LONG:
			(*src_entry)->refcount++;
				
			//php_printf("key='%d' value='%s'\n", num_key, Z_STRVAL_PP(src_entry));

			snprintf(buf, sizeof(buf), "%d", num_key);
				
			if (Z_TYPE_PP(src_entry) != IS_ARRAY) {
				convert_to_string_ex(src_entry);

				err = hdf_set_value(hdf, strdup(buf), Z_STRVAL_PP(src_entry));
				if (err != STATUS_OK) {
					php_error(E_WARNING, neo_error_to_string(err));
					return 0;
				}
			} else {
				convert_to_array_ex(src_entry);

				err = hdf_get_node(hdf, strdup(buf), &node);
				if (err != STATUS_OK) {
					php_error(E_WARNING, neo_error_to_string(err));
					return 0;
				}
						
				if (!hdf_set_array(node, Z_ARRVAL_PP(src_entry) TSRMLS_CC))
					return 0;
			}

			break;
		}

		zend_hash_move_forward_ex(src, &pos);
	}

	return 1;
}

/* {{{ proto bool hdf_set_value(resource hdf, string name, mixed value)
   Set the value of a named node */
PHP_FUNCTION(hdf_set_value)
{
	zval *zhdf = NULL;
	zval *value = NULL;
	char *name = NULL;
	int argc = ZEND_NUM_ARGS();
	int name_len;
	HDF *hdf = NULL;
	HDF *node = NULL;			/* potential child node */
	NEOERR *err = NULL;

	if (zend_parse_parameters(argc TSRMLS_CC, "rsz", &zhdf, &name, &name_len, &value) == FAILURE) 
		return;

	ZEND_FETCH_RESOURCE(hdf, HDF *, &zhdf, -1, "ClearSilver HDF", le_clearsilver_hdf);

	if (!hdf) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to retrieve HDF resource");
			RETURN_FALSE;
	}

	if ((name_len == 0) && (hdf == hdf_obj_top(hdf))) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "the top HDF node cannot have a value");
			RETURN_FALSE;
	}

	/* start buffering */
	php_start_ob_buffer (NULL, 0, 1 TSRMLS_CC);

	/* print zval */
	zend_print_variable(value);

	/* retrieve output buffer */
	php_ob_get_buffer (return_value TSRMLS_CC);
	php_end_ob_buffer (0, 0 TSRMLS_CC);
	
	err = hdf_set_value(hdf, name, Z_STRVAL_P(return_value));
	if (err != STATUS_OK) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, neo_error_to_string(err));
			RETURN_FALSE;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto string hdf_get_value(resource hdf, string name, string defval)
   Return the value of a node in the data set */
PHP_FUNCTION(hdf_get_value)
{
	zval *zhdf = NULL;
	char *name = NULL;
	char *defval = NULL;
	char *value = NULL;
	int argc = ZEND_NUM_ARGS();
	int name_len;
	int defval_len;
	HDF *hdf = NULL;

	if (zend_parse_parameters(argc TSRMLS_CC, "rss", &zhdf, &name, &name_len, &defval, &defval_len) == FAILURE) 
		return;

	ZEND_FETCH_RESOURCE(hdf, HDF *, &zhdf, -1, "ClearSilver HDF", le_clearsilver_hdf);

	if (!hdf) {
		php_error(E_WARNING, "failed to retrieve HDF resource");
		RETURN_NULL();
	}

	value = hdf_get_value(hdf, name, defval);

	RETURN_STRING(value, 1);
}
/* }}} */

/* {{{ proto resource hdf_get_node(resource hdf, string name)
   Return the HDF data set node at a named location, creating it if it does not exist */
PHP_FUNCTION(hdf_get_node)
{
	zval *zhdf = NULL;
	char *name = NULL;
	int argc = ZEND_NUM_ARGS();
	int name_len;
	NEOERR *err;
	HDF *hdf = NULL;
	HDF *node = NULL;

	if (zend_parse_parameters(argc TSRMLS_CC, "rs", &zhdf, &name, &name_len) == FAILURE) 
		return;

	ZEND_FETCH_RESOURCE(hdf, HDF *, &zhdf, -1, "ClearSilver HDF", le_clearsilver_hdf);

	if (!hdf) {
		php_error(E_WARNING, "failed to retrieve HDF resource");
		RETURN_NULL();
	}

	err = hdf_get_node(hdf, name, &node);

	if (err != STATUS_OK) {
		php_error(E_WARNING, neo_error_to_string(err));
		RETURN_NULL();
	}
	
	if (!node) {
		RETURN_NULL();
	}

	ZEND_REGISTER_RESOURCE(return_value, node, le_clearsilver_hdf);
}
/* }}} */

/* {{{ proto resource hdf_get_obj(resource hdf, string name)
   Return the HDF data set node at a named location */
PHP_FUNCTION(hdf_get_obj)
{
	zval *zhdf = NULL;
	char *name = NULL;
	int argc = ZEND_NUM_ARGS();
	int name_len;
	HDF *hdf = NULL;
	HDF *obj = NULL;

	if (zend_parse_parameters(argc TSRMLS_CC, "rs", &zhdf, &name, &name_len) == FAILURE) 
		return;

	ZEND_FETCH_RESOURCE(hdf, HDF *, &zhdf, -1, "ClearSilver HDF", le_clearsilver_hdf);

	if (!hdf) {
		php_error(E_WARNING, "failed to retrieve HDF resource");
		RETURN_NULL();
	}

	obj = hdf_get_obj(hdf, name);

	if (!obj) {
		RETURN_NULL();
	}

	ZEND_REGISTER_RESOURCE(return_value, obj, le_clearsilver_hdf);
}
/* }}} */

/* {{{ proto string hdf_obj_name(resource hdf)
   Return the name of a node */
PHP_FUNCTION(hdf_obj_name)
{
	zval *zhdf = NULL;
	int argc = ZEND_NUM_ARGS();
	char *name = NULL;
	HDF *hdf = NULL;

	if (zend_parse_parameters(argc TSRMLS_CC, "r", &zhdf) == FAILURE) 
		return;

	ZEND_FETCH_RESOURCE(hdf, HDF *, &zhdf, -1, "ClearSilver HDF", le_clearsilver_hdf);

	if (!hdf) {
		php_error(E_WARNING, "failed to retrieve HDF resource");
		RETURN_NULL();
	}

	name = hdf_obj_name(hdf);

	if (!name) {
		RETURN_NULL();
	}

	RETURN_STRING(name, 1);
}
/* }}} */

/* {{{ proto string hdf_obj_value(resource hdf)
   Return the value of a node */
PHP_FUNCTION(hdf_obj_value)
{
	zval *zhdf = NULL;
	int argc = ZEND_NUM_ARGS();
	char *value = NULL;
	HDF *hdf = NULL;

	if (zend_parse_parameters(argc TSRMLS_CC, "r", &zhdf) == FAILURE) 
		return;

	ZEND_FETCH_RESOURCE(hdf, HDF *, &zhdf, -1, "ClearSilver HDF", le_clearsilver_hdf);

	if (!hdf) {
		php_error(E_WARNING, "failed to retrieve HDF resource");
		RETURN_NULL();
	}

	value = hdf_obj_value(hdf);

	if (!value) {
		RETURN_EMPTY_STRING();
	}

	RETURN_STRING(value, 1);
}
/* }}} */

/* {{{ proto resource hdf_obj_child(resource hdf)
   Return the first child of a dataset node */
PHP_FUNCTION(hdf_obj_child)
{
	zval *zhdf = NULL;
	int argc = ZEND_NUM_ARGS();
	HDF *hdf = NULL;
	HDF *child = NULL;

	if (zend_parse_parameters(argc TSRMLS_CC, "r", &zhdf) == FAILURE) 
		return;

	ZEND_FETCH_RESOURCE(hdf, HDF *, &zhdf, -1, "ClearSilver HDF", le_clearsilver_hdf);

	if (!hdf) {
		php_error(E_WARNING, "failed to retrieve HDF resource");
		RETURN_NULL();
	}

	child = hdf_obj_child(hdf);

	if (!child) {
		RETURN_NULL();
	}

	ZEND_REGISTER_RESOURCE(return_value, child, le_clearsilver_hdf);
}
/* }}} */

/* {{{ proto resource hdf_get_child(resource hdf, string name)
   Return the first child of the named node */
PHP_FUNCTION(hdf_get_child)
{
	zval *zhdf = NULL;
	int argc = ZEND_NUM_ARGS();
	char *name = NULL;
	int name_len;
	HDF *hdf = NULL;
	HDF *child = NULL;

	if (zend_parse_parameters(argc TSRMLS_CC, "rs", &zhdf, &name, &name_len) == FAILURE) 
		return;

	ZEND_FETCH_RESOURCE(hdf, HDF *, &zhdf, -1, "ClearSilver HDF", le_clearsilver_hdf);

	if (!hdf) {
		php_error(E_WARNING, "failed to retrieve HDF resource");
		RETURN_NULL();
	}

	child = hdf_get_child(hdf, name);

	if (!child) {
		RETURN_NULL();
	}

	ZEND_REGISTER_RESOURCE(return_value, child, le_clearsilver_hdf);
}
/* }}} */


/* {{{ proto resource hdf_obj_next(resource hdf)
   Return the next node of a dataset level */
PHP_FUNCTION(hdf_obj_next)
{
	zval *zhdf = NULL;
	int argc = ZEND_NUM_ARGS();
	HDF *hdf = NULL;
	HDF *next = NULL;

	if (zend_parse_parameters(argc TSRMLS_CC, "r", &zhdf) == FAILURE) 
		return;

	ZEND_FETCH_RESOURCE(hdf, HDF *, &zhdf, -1, "ClearSilver HDF", le_clearsilver_hdf);

	if (!hdf) {
		php_error(E_WARNING, "failed to retrieve HDF resource");
		RETURN_NULL();
	}

	next = hdf_obj_next(hdf);

	if (!next) {
		RETURN_NULL();
	}

	ZEND_REGISTER_RESOURCE(return_value, next, le_clearsilver_hdf);
}
/* }}} */

/* {{{ proto void hdf_destroy(resource hdf)
   Deallocate an HDF data set */
PHP_FUNCTION(hdf_destroy)
{
	zval *zhdf = NULL;
	int argc = ZEND_NUM_ARGS();
	HDF *hdf = NULL;

	if (zend_parse_parameters(argc TSRMLS_CC, "r", &zhdf) == FAILURE) 
		return;

	ZEND_FETCH_RESOURCE(hdf, HDF *, &zhdf, -1, "ClearSilver HDF", le_clearsilver_hdf);

	zend_list_delete(Z_LVAL_P(zhdf));
}
/* }}} */


/* {{{ proto resource cs_init(resource hdf)
   Create and initialize a CS context */
PHP_FUNCTION(cs_init)
{
	zval *zhdf = NULL;
	CSPARSE *parse;
	NEOERR *err;
	int argc = ZEND_NUM_ARGS();
	HDF *hdf = NULL;

	if (zend_parse_parameters(argc TSRMLS_CC, "r", &zhdf) == FAILURE) 
		return;

	ZEND_FETCH_RESOURCE(hdf, HDF *, &zhdf, -1, "ClearSilver HDF", le_clearsilver_hdf);

	if (!hdf) {
		php_error(E_WARNING, "failed to retrieve HDF resource");
		RETURN_NULL();
	}

	err = cs_init(&parse, hdf);
	
	if (err != STATUS_OK) {
		php_error(E_WARNING, neo_error_to_string(err));
		RETURN_NULL();
	}

	// add support for filters (html_escape, url_escape, etc)
	err = cgi_register_strfuncs(parse);
	if (err != STATUS_OK) {
		cs_destroy(parse);
		php_error(E_WARNING, neo_error_to_string(err));
		RETURN_NULL();
	}

	ZEND_REGISTER_RESOURCE(return_value, parse, le_clearsilver_cs);
}
/* }}} */


/* {{{ proto bool cs_parse_string(resource parse, string buf)
   parse a CS template string */
PHP_FUNCTION(cs_parse_string)
{
	zval *zparse = NULL;
	CSPARSE *parse = NULL;
	NEOERR *err = NULL;
	char *template = NULL;
	char *buf = NULL;
	int template_len;
	int argc = ZEND_NUM_ARGS();

	if (zend_parse_parameters(argc TSRMLS_CC, "rs", &zparse, &template, &template_len) == FAILURE) 
		return;

	ZEND_FETCH_RESOURCE(parse, CSPARSE *, &zparse, -1, "ClearSilver CS", le_clearsilver_cs);

	if (!parse) {
		php_error(E_WARNING, "failed to retrieve CS resource");
		RETURN_FALSE;
	}

	buf = strdup(template);

	if (!buf) {
		php_error(E_WARNING, "failed to duplicate string");
		RETURN_FALSE;
	}

	err = cs_parse_string(parse, buf, template_len);
	
	if (err != STATUS_OK) {
		php_error(E_WARNING, neo_error_to_string(err));
		RETURN_FALSE;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto void cs_parse_file(resource parse, string path)
   parse a CS template file */
PHP_FUNCTION(cs_parse_file)
{
	zval *zparse = NULL;
	CSPARSE *parse = NULL;
	NEOERR *err = NULL;
	char *path = NULL;
	int path_len;
	int argc = ZEND_NUM_ARGS();

	if (zend_parse_parameters(argc TSRMLS_CC, "rs", &zparse, &path, &path_len) == FAILURE) 
		return;

	ZEND_FETCH_RESOURCE(parse, CSPARSE *, &zparse, -1, "ClearSilver CS", le_clearsilver_cs);

	if (!parse) {
		php_error(E_WARNING, "failed to retrieve CS resource");
		RETURN_FALSE;
	}

	err = cs_parse_file(parse, path);
	
	if (err != STATUS_OK) {
		php_error(E_WARNING, neo_error_to_string(err));
		RETURN_FALSE;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto string cs_render(resource cs)
   Render a CS parse tree to a string */
PHP_FUNCTION(cs_render)
{
	zval *zparse = NULL;
	CSPARSE *parse = NULL;
	NEOERR *err = NULL;
	STRING str;
	int argc = ZEND_NUM_ARGS();

	if (zend_parse_parameters(argc TSRMLS_CC, "r", &zparse) == FAILURE) 
		return;

	ZEND_FETCH_RESOURCE(parse, CSPARSE *, &zparse, -1, "ClearSilver CS", le_clearsilver_cs);

	if (!parse) {
		php_error(E_WARNING, "failed to retrieve CS resource");
		RETURN_NULL();
	}
	
	string_init(&str);
	err = cs_render(parse, &str, render_cb);
	
	if (err != STATUS_OK) {
		php_error(E_WARNING, neo_error_to_string(err));
		RETURN_NULL();
	}

	RETVAL_STRING(str.buf, 1);

	string_clear (&str);
}
/* }}} */

/* {{{ proto void cs_destroy(resource parse)
   Clean up and deallocate a CS parse tree */
PHP_FUNCTION(cs_destroy)
{
	zval *zparse = NULL;
	CSPARSE *parse = NULL;
	int argc = ZEND_NUM_ARGS();

	if (zend_parse_parameters(argc TSRMLS_CC, "r", &zparse) == FAILURE) 
		return;

	ZEND_FETCH_RESOURCE(parse, CSPARSE *, &zparse, -1, "ClearSilver CS", le_clearsilver_cs);

	zend_list_delete(Z_LVAL_P(zparse));
}
/* }}} */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
