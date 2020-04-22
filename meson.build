project('planck', 'c', version: '0.1.0')

cflags = [  '-g', '-Os', '-std=c11',
            # Turn on all warnings and treat all warnings as errors.
            '-Werror', '-Wall', '-Wextra',
            # Additional warnings
            '-Wunreachable-code', '-Wpointer-arith', '-Wformat=2', '-Wno-format-nonliteral',
            # Not warnings
            '-Wno-missing-field-initializers', '-Wno-unused-parameter', '-Wno-unused-function'
        ]

# lex & yacc
gen_yacc = custom_target('yacc', 
                    output:['y.tab.c', 'y.tab.h'],
                    input:'src/syntax.yacc',
                    command: ['yacc', '-d', '@INPUT@'])
gen_lex = custom_target('lex', 
                    output:'lex.yy.c',
                    input:'src/syntax.lex',
                    command:['lex', '@INPUT@'])

# libraries (shared, static)
incdir = include_directories('src')
src = [ 'src/codegen.c',
        'src/vm.c',
        'src/utils.c',
        'src/errors.c',
        'src/symtab.c',
        'src/planck.c']
libplancks = both_libraries('planck', [src, gen_yacc, gen_lex], 
                        version : meson.project_version(), soversion : '0',
                        c_args:cflags, include_directories:incdir, pic:true)

cc = meson.get_compiler('c')
readline_dep = cc.find_library('readline', required : true)

executable('planck', 'src/main.c', 
            link_with:libplancks.get_shared_lib(),
            c_args:cflags, dependencies:readline_dep)
executable('planck_static', 'src/main.c', 
            link_with:libplancks.get_static_lib(),
            c_args:cflags, dependencies:readline_dep)

# unit test
unit_test_src = ['unittest/testcases/basic_op_test.c',
                'unittest/testcases/error_test.c',
                'unittest/testcases/variable_test.c',
                'unittest/framework/TestMain.c']

test_incdir = include_directories(['src', 'unittest/framework'])

executable('unittest', unit_test_src,
            link_with:libplancks.get_shared_lib(),
            c_args:cflags, include_directories:test_incdir)