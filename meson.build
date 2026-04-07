project('tdev', 'c')

srcs = [
    'src/netlink.c',
    'src/tdev.c',
    'src/plugin.c',
]

add_project_arguments('-I' + meson.current_source_dir()+'/src',  language: 'c')
add_project_arguments('-fvisibility=hidden', '-Dvisible=__attribute__((visibility("default")))',  language: 'c')
add_project_arguments('-DPLUGINDIR="'+get_option('plugindir')+'"', language: 'c')


if get_option('debug')
    add_project_arguments('-Ddebug=printf', '-DDEBUG', language: 'c')
else
    add_project_arguments('-Ddebug=', language: 'c')
endif



# library
libtdev = library('tdev', srcs)

# plugins
foreach src : run_command('find', 'plugins', '-type', 'f', '-iname', '*.c',check:true).stdout().strip().split('\n')
        name = src.replace('.c', '').split('/')[-1]
        library('tdev_'+name, [src], link_with: libtdev)
endforeach

executable('tdev-cli', ['src/main.c'], link_with: libtdev)