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

#ifndef PHP_CLEARSILVER_H
#define PHP_CLEARSILVER_H

extern zend_module_entry clearsilver_module_entry;
#define phpext_clearsilver_ptr &clearsilver_module_entry

#ifdef PHP_WIN32
#define PHP_CLEARSILVER_API __declspec(dllexport)
#else
#define PHP_CLEARSILVER_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(clearsilver);
PHP_MSHUTDOWN_FUNCTION(clearsilver);
PHP_RINIT_FUNCTION(clearsilver);
PHP_RSHUTDOWN_FUNCTION(clearsilver);
PHP_MINFO_FUNCTION(clearsilver);

PHP_FUNCTION(hdf_init);
PHP_FUNCTION(hdf_read_file);
PHP_FUNCTION(hdf_write_file);
PHP_FUNCTION(hdf_write_string);
PHP_FUNCTION(hdf_set_value);
PHP_FUNCTION(hdf_get_value);
PHP_FUNCTION(hdf_get_node);
PHP_FUNCTION(hdf_get_obj);
PHP_FUNCTION(hdf_obj_name);
PHP_FUNCTION(hdf_obj_value);
PHP_FUNCTION(hdf_obj_child);
PHP_FUNCTION(hdf_obj_next);
PHP_FUNCTION(hdf_get_child);
PHP_FUNCTION(hdf_destroy);
PHP_FUNCTION(cs_init);
PHP_FUNCTION(cs_parse_string);
PHP_FUNCTION(cs_parse_file);
PHP_FUNCTION(cs_render);
PHP_FUNCTION(cs_destroy);

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(clearsilver)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(clearsilver)
*/

/* In every utility function you add that needs to use variables 
   in php_clearsilver_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as CLEARSILVER_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define CLEARSILVER_G(v) TSRMG(clearsilver_globals_id, zend_clearsilver_globals *, v)
#else
#define CLEARSILVER_G(v) (clearsilver_globals.v)
#endif

#endif	/* PHP_CLEARSILVER_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 */
