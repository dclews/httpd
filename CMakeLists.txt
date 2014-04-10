cmake_minimum_required(VERSION 2.8)

SET(LIBPATH $ENV{HOME}/local/lib)

project("httpd")
INCLUDE($ENV{HOME}/local/include/cmake/Clews.cmake)
clews_add_executable()

clews_target_link_libraries("${LIBPATH}/clews/libsock.so")
clews_target_link_libraries("${LIBPATH}/clews/libcore.so")
clews_target_link_libraries("${LIBPATH}/clews/libutility.so")
clews_target_link_libraries("${LIBPATH}/clews/libmime.so")
clews_target_link_libraries("${LIBPATH}/clews/libhttp.so")
