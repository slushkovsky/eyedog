import os
import sys
import shutil
from distutils.core import setup, Extension
from distutils.command.build import build as _build
from distutils.dir_util import remove_tree

from utils.path import PROJ_ROOT, butify_path
from utils.assert_cond import is_listed_type
import exc

def find_builded(find_path, module_name): 
    for root, dirs, files in os.walk(find_path): 
        for dirname in dirs:
            if dirname.startswith('temp'):
                continue

            founded = find_builded(os.path.join(root, dirname), module_name)

            if founded is not None: 
                return founded

        for filename in files: 
            if filename.startswith(module_name): 
                return os.path.join(root, filename)


def build(module_basename, module_dir, sources, libs, include_paths=[], libs_paths=[]):
    assert(isinstance(module_basename, str))
    assert(isinstance(module_dir,      str))

    assert(is_listed_type(sources))
    assert(is_listed_type(libs))
    assert(is_listed_type(include_paths))
    assert(is_listed_type(libs_paths))


    DEFAULT_LIB_PATHS = {
        os.path.normpath(os.path.join(PROJ_ROOT, 'pyboost')), 
        '/usr/local/lib'
    }

    DEFAULT_INCLUDE_PATHS = {
        PROJ_ROOT,
        '/usr/local/include'
    }

    ext_params = dict(
        name                =module_basename, 
        extra_compile_args  =['-std=c++11'], 
        sources             =list(sources),
        include_dirs        =list(DEFAULT_INCLUDE_PATHS.union(include_paths)),
        runtime_library_dirs=list(DEFAULT_LIB_PATHS    .union(libs_paths   )),
        libraries           =list(libs),
        language            ='C++'
    )

    cpp_module = Extension(**ext_params)

    if 'PYBOOST_DEBUG' in os.environ:
        print('-' * 32)   
        print('Build module:  {}'.format(ext_params['name']))
        print('Module dir:    {}'.format(butify_path(module_dir)))
        print('Sources:       {}'.format([butify_path(path) for path in ext_params['sources'     ]]))
        print('Libs:          {}'.format([butify_path(path) for path in ext_params['libraries'   ]]))
        print('Libs paths:    {}'.format([butify_path(path) for path in ext_params['runtime_library_dirs']]))
        print('Include paths: {}'.format([butify_path(path) for path in ext_params['include_dirs']]))
        print('-' * 32)


    build_dir = os.path.join(module_dir, '__build__')

    class build(_build):
        def run(self): 
            _build.run(self)

            builded = find_builded(build_dir, module_basename)

            if builded is None: 
                raise exc.PyBoostError('Build result not found')

            shutil.copy(builded, module_dir)
            remove_tree(build_dir)

    sys.argv.append('-q')
    sys.argv.append('build')

    setup(ext_modules=[cpp_module],
          cmdclass={'build': build},
          options={'build': {'build_base': build_dir}})


def import_module(module, sources, libs, include_paths=[], libs_paths=[], rebuild_anyway=False): 
    module_basename = module.split('.')[-1]
    module_dir = os.path.join(PROJ_ROOT, *module.split('.')[:-1])

    sys.path.append(module_dir)

    try: 
        if rebuild_anyway: 
            raise ImportError()

        return __import__(module_basename)
    
    except ImportError as e:
        if not str(e).startswith('No module named'): 
            raise e

        print('Compilation of {!r}\nPlease wait...'.format(module))

        build(module_basename, module_dir, sources, libs, include_paths, libs_paths)
        
        print('Compilation done!')
        
        try: 
            return  __import__(module_basename)
        except ImportError as e:
            if str(e).startswith('No module named'):  
                raise exc.PyBoostError('Couldn\'t import module {!r} after build'.format(module_name))
            else: 
                raise e
