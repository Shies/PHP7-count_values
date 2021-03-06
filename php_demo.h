/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: ___Shies  gukai@bilibili.com                                 |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_DEMO_H
#define PHP_DEMO_H

extern zend_module_entry demo_module_entry;
#define phpext_demo_ptr &demo_module_entry

#define PHP_DEMO_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_DEMO_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_DEMO_API __attribute__ ((visibility("default")))
#else
#	define PHP_DEMO_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif


#define DEMO_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(demo, v)

#if defined(ZTS) && defined(COMPILE_DL_DEMO)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif	/* PHP_DEMO_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
