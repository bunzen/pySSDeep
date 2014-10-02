from distutils.core import setup, Extension

module1 = Extension('pyssdeep',
                    define_macros = [('MAJOR_VERSION', '0'),
                                     ('MINOR_VERSION', '1')],
                    include_dirs = ['/usr/local/include'],
                    libraries = ['fuzzy'],
                    library_dirs = ['/usr/local/lib'],
                    sources = ['pyssdeepmodule.c'])

setup (name = 'pySSDeep',
       version = '0.1',
       description = 'Wrapper module providing a python interface to the ssdeep library',
       author = 'Geir Skjotskift',
       author_email = 'geir@underworld.no',
       url = 'https://github.com/bunzen/pySSDeep',
       long_description = '''
Wrapper module around the ssdeep library/tool.
http://ssdeep.sourceforge.net/'
''',
       ext_modules = [module1])
