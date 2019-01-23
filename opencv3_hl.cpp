/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2018 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_opencv3_hl.h"

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <string.h>

/* If you declare any globals in php_opencv3_hl.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(opencv3_hl)
*/

/* True global resources - no need for thread safety here */
static int le_opencv3_hl;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("opencv3_hl.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_opencv3_hl_globals, opencv3_hl_globals)
    STD_PHP_INI_ENTRY("opencv3_hl.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_opencv3_hl_globals, opencv3_hl_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/**
 * 取出目标路径的所属目录，例如： /a/b/c/d.jpg，dir会赋值为 /a/b/c
 * path        目标路径
 * separate    分隔符
 * dir         所要取出的路径
*/
void splitpath(const char* path, char separate, char* dir) {
    size_t path_len = strlen(path);
    int separate_node = 0;
    //std::cout << "full: " << path << "len:" << path_len << std::endl;
    // 从字符串末尾开始查找分隔符
    for (int i = (path_len - 1); i >= 0; i--) {
        // std::cout << "check2: " << path[i] <<std::endl;
        // 找到分隔符时停止
        if (path[i] == separate) {
            separate_node = i;
            break;
        }
    }
    // 把路径复制入dir
    strncpy(dir, path, separate_node + 1);
    // 添加结束符
    dir[separate_node + 1] = '\0';
    // std::cout << "dir: " << dir << std::endl;
}

/**
 * 建立多级目录
 * path    目标目录
*/
int creatpath(char* path) {
    // 文件状态
    struct stat f_buf;
    int max_len = strlen(path);
    // 分隔符
    const char* d = "/";
    // 分割字符串结果
    char* res;
    // 临时目录路径
    char buf_path[max_len];
    strcpy(buf_path, "/");
    // std::cout << "buf_path: " << buf_path << std::endl;
    res = strtok(path, d);
    // std::cout << "res: " << res << std::endl;
    while(res) {
        strcat(buf_path, res);
        // 判断目录是否存在
        if (stat(buf_path, &f_buf) < 0) {
        // 如果文件不存在，创建目录
            mkdir(buf_path, 0755);
            // std::cout << "buf_path: " << buf_path << " - not exist" << std::endl;
        } else {
            // std::cout << "buf_path: " << buf_path << " - is exist" << std::endl;
        }
        // 继续分割路径
        res = strtok(NULL, d);
        // std::cout << "res: " << res << std::endl;
        if (res) {
            strcat(buf_path, "/");
        }
    }
    return 1;
}

PHP_FUNCTION(opencv3_hl_capture_video_image)
{
	// 视频绝对路径
    char* video_src;
    size_t video_src_len;

	// 存储图片目标路径
    char* img_dest;
    size_t img_dest_len;

	// 要截取画面的秒数
	long desc_sec;

    ZEND_PARSE_PARAMETERS_START(3, 3)
        Z_PARAM_STRING(video_src, video_src_len)
        Z_PARAM_STRING(img_dest, img_dest_len)
		Z_PARAM_LONG(desc_sec)
		// Z_PARAM_OPTIONAL		// 可选参数分隔
    ZEND_PARSE_PARAMETERS_END();
	
	// 测试用例
    // size_t str_len;
    // str_len = video_src_len + img_dest_len;
    // char f[str_len];
    // strcpy(f, video_src);
    // strcat(f, img_dest);

	// 函数返回值
    int func_result = 0;
	// 判断输入为空则返回 0
    if (video_src != NULL || img_dest != NULL) {
        if (strlen(video_src) == 0 || strlen(img_dest) == 0) {
            RETURN_LONG(func_result);
        }
    }

	// 自动建立目标路径文件目录
	// 目标路径长度
    size_t path_len = strlen(img_dest);
    // 目标文件目录
    char dir[path_len];
    // 文件辨识符
    char file_sepa[] = ".";
    // 文件状态结构体
    struct stat f_buf;
    // 判断目标路径是个文件还是目录
    if (strstr(img_dest, file_sepa)) {
        // 包含 '.' 字符，默认为一个文件
        // 取出文件的上层目录
        splitpath(img_dest, '/', dir);
    } else {
        // 不包含 '.' 字符，默认为一个目录
        strcpy(dir, img_dest);
    }
    // 判断dir是否存在
    if (stat(dir, &f_buf) < 0) {
        // 如果目录不存在，创建目录
        creatpath(dir);
    }

	// 目标毫秒
	long desc_millisec = 1000 * desc_sec;

    using namespace cv;
    std::string file = video_src;
    VideoCapture capture(file);

    // 设置从 desc_millisec 秒开始
	capture.set(CV_CAP_PROP_POS_MSEC, desc_millisec);
	Mat frame;
    capture.read(frame);
    // 写入图片
    bool imwrite_result = imwrite(img_dest, frame);

    if (imwrite_result) {
        func_result = 1;
    } else {
        func_result = 0;
    }

    // 返回 0 or 1
	RETURN_LONG(func_result);
    
    //std::string str = video_src;
    //RETURN_STRING(img_dest);
    
}

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_opencv3_hl_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_opencv3_hl_compiled)
{
	char *arg = NULL;
	size_t arg_len, len;
	zend_string *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	strg = strpprintf(0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "opencv3_hl", arg);

	RETURN_STR(strg);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/


/* {{{ php_opencv3_hl_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_opencv3_hl_init_globals(zend_opencv3_hl_globals *opencv3_hl_globals)
{
	opencv3_hl_globals->global_value = 0;
	opencv3_hl_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(opencv3_hl)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(opencv3_hl)
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
PHP_RINIT_FUNCTION(opencv3_hl)
{
#if defined(COMPILE_DL_OPENCV3_HL) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(opencv3_hl)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(opencv3_hl)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "opencv3_hl support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ opencv3_hl_functions[]
 *
 * Every user visible function must have an entry in opencv3_hl_functions[].
 */
const zend_function_entry opencv3_hl_functions[] = {
	PHP_FE(opencv3_hl_capture_video_image, NULL)
	PHP_FE(confirm_opencv3_hl_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in opencv3_hl_functions[] */
};
/* }}} */

/* {{{ opencv3_hl_module_entry
 */
zend_module_entry opencv3_hl_module_entry = {
	STANDARD_MODULE_HEADER,
	"opencv3_hl",
	opencv3_hl_functions,
	PHP_MINIT(opencv3_hl),
	PHP_MSHUTDOWN(opencv3_hl),
	PHP_RINIT(opencv3_hl),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(opencv3_hl),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(opencv3_hl),
	PHP_OPENCV3_HL_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_OPENCV3_HL
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(opencv3_hl)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
