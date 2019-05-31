CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
PROJECT(planck)

INCLUDE(GNUInstallDirs)
INCLUDE(FindPkgConfig)

SET(VERSION_MAJOR 0)
SET(VERSION_MINOR 1)
SET(VERSION_PATCH 0)
SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

# Common compile options
ADD_COMPILE_OPTIONS(
	-g -Os

	# Turn on all warnings and treat all warnings as errors.
	-Werror -Wall -Wextra

	# Additional warnings
	-Wunreachable-code -Wpointer-arith
	-Wformat=2 -Wno-format-nonliteral

	# Not warnings
	-Wno-missing-field-initializers
	-Wno-unused-parameter)

ADD_DEFINITIONS(-DVERSION=\"${VERSION}\")

INCLUDE_DIRECTORIES(
	${CMAKE_BINARY_DIR}
	${CMAKE_SOURCE_DIR}/src)

# lex & yacc
ADD_CUSTOM_COMMAND(
	OUTPUT ${CMAKE_BINARY_DIR}/y.tab.c ${CMAKE_BINARY_DIR}/y.tab.h
	COMMAND yacc -d ${CMAKE_SOURCE_DIR}/syntax/syntax.yacc
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	DEPENDS syntax/syntax.yacc
	COMMENT "Generate YACC - y.tab.c/.h")

ADD_CUSTOM_COMMAND(
	OUTPUT ${CMAKE_BINARY_DIR}/lex.yy.c
	COMMAND lex ${CMAKE_SOURCE_DIR}/syntax/syntax.lex
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	DEPENDS syntax/syntax.lex
	COMMENT "Generate LEX - lex.yy.c")

# build syntax object without warning options
ADD_LIBRARY(syntax OBJECT
	${CMAKE_BINARY_DIR}/y.tab.c
	${CMAKE_BINARY_DIR}/lex.yy.c)
TARGET_COMPILE_OPTIONS(syntax PRIVATE
	-Wno-unused-function -Wno-unreachable-code
	-Wno-all -Wno-extra -Wno-error)

SET(SRCS
	src/planck.c
	src/code_gen.c
	src/virtual_machine.c)
ADD_EXECUTABLE(planck ${SRCS}
	$<TARGET_OBJECTS:syntax>)
TARGET_COMPILE_OPTIONS(planck PRIVATE -std=c11)
TARGET_LINK_LIBRARIES(planck PRIVATE -lreadline)
INSTALL(TARGETS planck RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

