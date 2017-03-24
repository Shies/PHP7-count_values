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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_demo.h"
#include "ext/standard/php_var.h"
#include "ext/standard/php_string.h"
#include "ext/standard/php_array.h"
#include "Zend/zend_API.h"


static int le_demo;
zend_class_entry *demo_ce;


char *get_contents(const char *path, const char *mode) {
    FILE *fp;
    fp = fopen(path, mode);
    fseek( fp , 0 , SEEK_END );

    int file_size;
    file_size = ftell( fp );

    char *tmp;
    fseek( fp , 0 , SEEK_SET);
    tmp =  (char *)malloc( file_size * sizeof( char ) );
    fread( tmp , file_size , sizeof(char) , fp);

    return tmp;
}


/** {{{ proto public demo::__construct(void)
*/
PHP_METHOD(demo, __construct)
{
    char *content;
    zval *self,
         *source,
         *target,
         rv;

    self = getThis();
    source = zend_read_property(demo_ce, self, ZEND_STRL("source"), 1, &rv);
    if (Z_TYPE_P(source) == IS_NULL)
    {
        target = zend_read_property(demo_ce, self, ZEND_STRL("target"), 1, &rv);
        content = get_contents(Z_STRVAL_P(target), "r");
        zend_update_property_string(demo_ce, self, ZEND_STRL("source"), content);
        return;
    }

    zend_update_property(demo_ce, self, ZEND_STRL("source"), source);
}


/** {{{ proto public demo::output(void)
*/
PHP_METHOD(demo, output)
{
    long limit = LONG_MAX;
    zval *source, rv, final;
    zend_string *value;
    zend_string *delim = zend_string_init("|", strlen("|"), 1);

    source = zend_read_property(demo_ce, getThis(), ZEND_STRL("source"), 1, &rv);
    value = zend_string_init(Z_STRVAL_P(source), Z_STRLEN_P(source), 1);

    array_init(return_value);
    php_explode(delim, value, return_value, limit);
    if (Z_TYPE_P(return_value) != IS_ARRAY) {
        php_printf("%s", "return_value must for array");
        return;
    }

    zval *item, // row value
         *args, // arguments[]
         retval, // callback return value
         function_name; // callback function name

    array_init(&final);
    ZVAL_STRINGL(&function_name, "handle", strlen("handle"));
    ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(return_value), item) {
        args = safe_emalloc(sizeof(zval), 2, 0);
        ZVAL_COPY_VALUE(&args[0], &final);
        ZVAL_COPY_VALUE(&args[1], item);
        call_user_function(NULL, getThis(), &function_name, &retval, 2, args TSRMLS_CC);

    } ZEND_HASH_FOREACH_END();

    // destory zval
    do {
        zval_ptr_dtor(&function_name);
        zval_ptr_dtor(&retval);
        zval_ptr_dtor(args);
        zval_ptr_dtor(item);
    } while(0);


    RETVAL_ARR(Z_ARRVAL(final));
}


/** {{{ proto public demo::handle(array items, string val)
 */
PHP_METHOD(demo, handle)
{
    long limit = LONG_MAX;
    zend_string *val;
    zval *items;
    zval *__, rv;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "aS", &items, &val) != SUCCESS) {
       return;
    }

    __ = zend_read_property(demo_ce, getThis(), ZEND_STRL("__"), 1, &rv);
    zend_string *delim = zend_string_init(Z_STRVAL_P(__), Z_STRLEN_P(__), 1);

    array_init(return_value);
    php_explode(delim, val, return_value, limit);

    zval *head = zend_hash_index_find(Z_ARRVAL_P(return_value), 0);
    zval *tail = zend_hash_index_find(Z_ARRVAL_P(return_value), 1);
    zend_string *key = zend_string_init(Z_STRVAL_P(head), Z_STRLEN_P(head), 1);

    convert_to_long(tail);
    if (zend_hash_exists(Z_ARRVAL_P(items), key)) {
        zval *one = zend_hash_find(Z_ARRVAL_P(items), key);
        zend_long total = Z_LVAL_P(one) + Z_LVAL_P(tail);
        add_assoc_long_ex(items, Z_STRVAL_P(head), Z_STRLEN_P(head), total);
    } else {
        add_assoc_long_ex(items, Z_STRVAL_P(head), Z_STRLEN_P(head), Z_LVAL_P(tail));
    }

    RETVAL_BOOL(1);
}


/** {{{ proto public demo::valid(void)
 */
PHP_METHOD(demo, valid)
{
    bool retval = 0;
    zval *self, *source, rv;

    self = getThis();
    source = zend_read_property(demo_ce, self, ZEND_STRL("source"), 1, &rv);
    if (Z_TYPE_P(source) != IS_NULL)
    {
        retval = 1;
    }

    RETVAL_BOOL(retval);
}



const zend_function_entry demo_methods[] = {
	PHP_ME(demo, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    PHP_ME(demo, output,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(demo, handle,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(demo, valid,   NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};




PHP_MINIT_FUNCTION(demo)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "demo", demo_methods);

    demo_ce = zend_register_internal_class(&ce TSRMLS_CC);
    // demo_ce->ce_flags |= ZEND_ACC_TRAIT;

    zend_declare_property_null(demo_ce, ZEND_STRL("source"), ZEND_ACC_PRIVATE TSRMLS_CC);
    zend_declare_property_string(demo_ce, ZEND_STRL("__"), ":", ZEND_ACC_PUBLIC TSRMLS_CC);
    zend_declare_property_string(demo_ce, ZEND_STRL("target"), "test.txt", ZEND_ACC_PUBLIC TSRMLS_CC);


	return SUCCESS;
}


PHP_MSHUTDOWN_FUNCTION(demo)
{
	return SUCCESS;
}



PHP_RINIT_FUNCTION(demo)
{
#if defined(COMPILE_DL_DEMO) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}



PHP_RSHUTDOWN_FUNCTION(demo)
{
	return SUCCESS;
}


PHP_MINFO_FUNCTION(demo)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "demo support", "enabled");
	php_info_print_table_end();

}


const zend_function_entry demo_functions[] = {
	PHP_FE_END	/* Must be the last line in demo_functions[] */
};


zend_module_entry demo_module_entry = {
	STANDARD_MODULE_HEADER,
	"demo",
	demo_functions,
	PHP_MINIT(demo),
	PHP_MSHUTDOWN(demo),
	PHP_RINIT(demo),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(demo),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(demo),
	PHP_DEMO_VERSION,
	STANDARD_MODULE_PROPERTIES
};


#ifdef COMPILE_DL_DEMO
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(demo)
#endif

