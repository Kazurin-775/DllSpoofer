project('high_dpi_hook', 'cpp',
    version : '0.1',
    default_options : ['warning_level=3', 'cpp_std=c++14'])

### configuration part

dllname = 'winmm'
system32 = 'C:\\Windows\\System32'

conf_data = configuration_data()
conf_data.set('dllname', dllname)
configure_file(input : 'dllname.h.in',
               output : 'dllname.h',
               configuration : conf_data)

dllpath = system32 + '\\' + dllname + '.dll'

compiler_id = meson.get_compiler('cpp').get_id()

### export table generation part

generate_command = ['python', files('extract_exports.py'), dllpath]
if compiler_id == 'msvc'
    generate_command += '--use-noname'
endif

gen_files = custom_target('gen_files',
    input : ['extract_exports.py', dllpath],
    command : generate_command,
    output : ['func_defs.inc', 'exports.def'])

### compiler part

if compiler_id == 'msvc'
    link_args = ['shcore.lib', '/def:exports.def']      # shcore.dll is only needed by hidpi_hook
else
    link_args = [system32 + '\\shcore.dll', 'exports.def']
endif

libhook = shared_library('hook',
    'dll_hook.cc',
    'hidpi_hook.cc',
    gen_files,
    link_args : link_args)
