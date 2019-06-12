project('planck', 'c', version: '0.1.0')

cflags = [  '-g', '-Os', '-std=c11',
            # Turn on all warnings and treat all warnings as errors.
            '-Werror', '-Wall', '-Wextra',
            # Additional warnings
            '-Wunreachable-code', '-Wpointer-arith', '-Wformat=2', '-Wno-format-nonliteral',
            # Not warnings
            '-Wno-missing-field-initializers', '-Wno-unused-parameter', '-Wno-unused-function'
            ]

incdir = include_directories('src')

# lex & yacc
gen_yacc = custom_target('yacc', 
                    output:['y.tab.c', 'y.tab.h'],
                    input:'syntax/syntax.yacc',
                    command: ['yacc', '-d', '@INPUT@'])
gen_lex = custom_target('lex', 
                    output:'lex.yy.c',
                    input:'syntax/syntax.lex',
                    command:['lex', '@INPUT@'])

# libraries (shared, static) 
src = [ 'src/planck.c',
        'src/object.c',
        'src/code_gen.c',
        'src/symtab.c',
        'src/virtual_machine.c']
libplancks = both_libraries('planck', [src, gen_yacc, gen_lex], 
                        version : meson.project_version(), soversion : '0',
                        c_args:cflags, include_directories:incdir, pic:true)

# planck executable
cc = meson.get_compiler('c')
readline_dep = cc.find_library('readline', required : true)

executable('planck', 'src/main.c', 
            link_with:libplancks.get_shared_lib(),
            c_args:cflags, dependencies:readline_dep)
executable('planck_static', 'src/main.c', 
            link_with:libplancks.get_static_lib(),
            c_args:cflags, dependencies:readline_dep)

