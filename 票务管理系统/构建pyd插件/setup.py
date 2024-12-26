from setuptools import setup, Extension
from Cython.Build import cythonize

setup(
    name='py_ollama',  
    ext_modules=cythonize("py_ollama.pyx", language_level=3)
)
