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
    version: '0.1.0-dev'
)

# Project variables
PROJECT_VERSION = '0.1.0-dev'

# Run the globber.sh script to collect all source and header files
# NOTE: This is not recommended
# https://github.com/mesonbuild/meson/blob/master/docs/markdown/FAQ.md#but-i-really-want-to-use-wildcards
g_source = run_command('bash', './scripts/globber.sh', '-s', capture: true, check: true)
g_include = run_command('bash', './scripts/globber.sh', '-i', capture: true, check: true)
g_source_files = g_source.stdout().strip().replace('./', '').split('\n')
g_include_files = g_include.stdout().strip().replace('./', '').split('\n')

# Declare dependencies
libVoid = dependency('libVoid', method: 'pkg-config', required: true, version: '>=0.1.0-dev')

# Unit test declaration
test(
    'libStarlog - Unit Test',
    executable(
        'libstarlog_unit_test',
        'tests/tests.c',
        dependencies: [libVoid],
        include_directories: [include_directories('include'), 'tests'],
        install: false,
        sources: g_source_files,
    ),
)

# Library files declaration
lsl = library(
    'starlog',
    g_source_files,
    dependencies: [libVoid],
    install: true,
    install_dir: 'lib/astrl/libstarlog',
)

# Library headers declaration
lslh = install_headers(
    g_include_files,
    follow_symlinks: true,
    install_dir: 'include/astrl/libstarlog',
    preserve_path: false
)

# Generate pkg-config
pkg = import('pkgconfig')
pkg.generate(
    lsl,
    description: 'A simple to use logging utility - Provided by Astrl',
    install_dir: '/usr/lib/pkgconfig',
    name: 'libStarlog',
    requires: [libVoid],
    url: 'https://github.com/The-Astrl-Project/libstarlog',
    version: PROJECT_VERSION,
)
