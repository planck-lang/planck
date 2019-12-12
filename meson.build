project('planck', 'c', version: '0.1.0')

cflags = [  '-g', '-Os', '-std=c11',
            # Turn on all warnings and treat all warnings as errors.
            '-Werror', '-Wall', '-Wextra',
            # Additional warnings
            '-Wunreachable-code', '-Wpointer-arith', '-Wformat=2', '-Wno-format-nonliteral',
            # Not warnings
            '-Wno-missing-field-initializers', '-Wno-unused-parameter', '-Wno-unused-function'
        ]

cc = meson.get_compiler('c')
readline_dep = cc.find_library('readline', required : true)

executable('planck', 'src/main.c', 
            c_args:cflags, dependencies:readline_dep)