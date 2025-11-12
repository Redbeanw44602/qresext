add_rules('mode.debug', 'mode.release')

add_requires('lief     0.17.1')
add_requires('argparse 3.2')

--- global configs

set_license('MIT')

set_version('1.0.0')

set_languages('c++23', 'c23')
set_warnings('all')

set_configdir('$(builddir)')
add_configfiles('src/config.h.in')

set_pcxxheader('src/pch.h')

if is_mode('release') then
    set_policy('build.optimization.lto', true)
end

--- targets

target('qresext')
    add_rules('qt.console')
    add_files('src/**.cpp')
    add_includedirs('src', '$(builddir)')
    add_packages('lief', 'argparse')
