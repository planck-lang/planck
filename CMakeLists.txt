CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
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

IF (NOT NO-RPATH)
LINK_LIBRARIES(-Wl,--rpath=${CMAKE_BINARY_DIR})
ENDIF()

# Add cppcheck for target
FUNCTION(add_fake_cppcheck _target)
	# Get include dirs to add -I{..} parameters
	GET_TARGET_PROPERTY(_check_includes "${_target}" INCLUDE_DIRECTORIES)
	SET(_incopt)
	FOREACH(_inc ${_check_includes})
		SET(_incopt "${_incopt} -I${_inc}")
	ENDFOREACH()

	# Get all source files in the target
	GET_TARGET_PROPERTY(_check_sources "${_target}" SOURCES)

	# Create fake target for all sources
	FOREACH(_source ${_check_sources})
		# Get full path
		GET_SOURCE_FILE_PROPERTY(_path "${_source}" LOCATION)

		# Replace '/' to '_' ('/' is invalid in target name)
		STRING(REGEX REPLACE "\/" "_" _tname ${_source})

		SET(_fake_target "FakeCppcheckTarget_${_tname}")
		SET(_fake_path "${CMAKE_BINARY_DIR}/cppcheck")
		MESSAGE("-- Add fake-target for cppcheck: ${_fake_target}")

		# Run cppcheck and generate dummy fake target
		ADD_CUSTOM_COMMAND(
			OUTPUT ${_fake_path}/${_fake_target}
			COMMAND cppcheck
				--enable=all
				--std=c11
				--verbose
				--quiet
				${_incopt}
				${_path}
			COMMAND mkdir -p ${_fake_path}
			COMMAND touch ${_fake_path}/${_fake_target}
			DEPENDS ${_source})

		# Dummy fake target
		ADD_CUSTOM_TARGET(${_fake_target} ALL
			DEPENDS ${_fake_path}/${_fake_target})

		# Add to real target
		ADD_DEPENDENCIES(${_target} ${_fake_target})
	ENDFOREACH()
ENDFUNCTION()

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
	-Wno-all -Wno-extra -Wno-error
	-fPIC)

# library sources
SET(SRCS
	src/planck.c
    src/object.c
	src/code_gen.c
	src/symtab.c
	src/expr.c
	src/ported_lib.c
	src/virtual_machine.c)
ADD_LIBRARY(modplanck OBJECT ${SRCS})
TARGET_COMPILE_OPTIONS(modplanck PRIVATE
	-std=c11 -fPIC)

add_fake_cppcheck(modplanck)

# libplanck.so, libplanck.a
ADD_LIBRARY(libplanck_shared SHARED
	$<TARGET_OBJECTS:syntax>
	$<TARGET_OBJECTS:modplanck>)
SET_TARGET_PROPERTIES(libplanck_shared
	PROPERTIES VERSION ${VERSION} SOVERSION ${VERSION_MAJOR} OUTPUT_NAME planck)
INSTALL(TARGETS libplanck_shared LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

ADD_LIBRARY(libplanck_static STATIC
	$<TARGET_OBJECTS:syntax>
	$<TARGET_OBJECTS:modplanck>)
SET_TARGET_PROPERTIES(libplanck_static PROPERTIES
	OUTPUT_NAME planck)
INSTALL(TARGETS libplanck_static ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

# planck exeutable with shared and static library
ADD_EXECUTABLE(planck src/main.c)
TARGET_COMPILE_OPTIONS(planck PRIVATE -std=gnu11)
TARGET_LINK_LIBRARIES(planck -L${CMAKE_BINARY_DIR} -lreadline -lplanck )
ADD_DEPENDENCIES(planck libplanck_shared)
INSTALL(TARGETS planck RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

ADD_EXECUTABLE(planck_static src/main.c)
TARGET_COMPILE_OPTIONS(planck_static PRIVATE -std=gnu11)
TARGET_LINK_LIBRARIES(planck_static ${CMAKE_BINARY_DIR}/libplanck.a -lreadline)
ADD_DEPENDENCIES(planck_static libplanck_static)
INSTALL(TARGETS planck_static RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

add_fake_cppcheck(planck)

# header files
INSTALL(FILES src/planck.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/planck/)

# pkgconfig
CONFIGURE_FILE(planck.pc.in ${CMAKE_BINARY_DIR}/planck.pc @ONLY)
INSTALL(FILES ${CMAKE_BINARY_DIR}/planck.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig/)
