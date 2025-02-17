# libStarlog || meson.build
# --------------------------------
# Meson build configuration.
#
# Authors: @MaxineToTheStars <https://github.com/MaxineToTheStars>
# ----------------------------------------------------------------

# Project declaration
project(
    'libStarlog',
    'c',
    default_options: {
        'prefix': '/usr',
        'buildtype': 'debug',
        'debug': 'true',
        'default_library': 'shared',
        'errorlogs': 'true',
        'optimization': '0',
        'stdsplit': 'true',
        'c_std': 'c99'
    },
    license: 'GPLv3',
    license_files: ['LICENSE'],
    meson_version: '>= 1.5.0',
    version: '0-1.0-dev'
)

# Run the globber.sh script to collect all source and header files
# NOTE: This is not recommended
# https://github.com/mesonbuild/meson/blob/master/docs/markdown/FAQ.md#but-i-really-want-to-use-wildcards
g_source = run_command('bash', './scripts/globber.sh', '-s', capture: true, check: true)
g_include = run_command('bash', './scripts/globber.sh', '-i', capture: true, check: true)
g_source_files = g_source.stdout().strip().replace('./', '').split('\n')
g_include_files = g_include.stdout().strip().replace('./', '').split('\n')

# Declare dependencies
c_lang = meson.get_compiler('c', native: false)
libvoid = c_lang.find_library('astrl/libvoid/libvoid', required: true)

# Unit test declaration
test(
    'libStarlog - Unit Test',
    executable(
        'libstarlog_unit_test',
        'tests/tests.c',
        dependencies: [libvoid],
        include_directories: [include_directories('include'), 'tests'],
        install: false,
        sources: g_source_files,
    ),
)

# Library files declaration
library(
    'starlog',
    g_source_files,
    dependencies: [libvoid],
    install: true,
    install_dir: 'lib/astrl/libstarlog',
)

# Library headers declaration
install_headers(
    g_include_files,
    follow_symlinks: true,
    install_dir: 'include/astrl/libstarlog',
    preserve_path: false
)
