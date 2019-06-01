#!/usr/bin/env python3
# encoding: utf-8

from distutils.core import setup, Extension

planck_module = Extension('planck', sources = ['planck_module.c'],
                            include_dirs = ['../../src'],
                            extra_objects = ['../../build/libplanck.a'])

setup(name='planck',
      version='0.1.0',
      description='planck module written in C',
ext_modules=[planck_module])
