#!/usr/bin/env python

import sys

def options(opt):
        opt.load('compiler_cxx')

def configure(conf):
        conf.load('compiler_cxx')
        conf.env.CXXFLAGS_VRPD = ['-DFLEXT_SYS=FLEXT_SYS_PD', '-O6', '-fno-rtti' ,'-Wall']
        conf.env.STLIB_VRPD = ['m', 'vrpn', 'vrpnserver', 'flext-pd_sd']
        conf.env.STLIBPATH_VRPD = ['/usr/lib','/usr/local/lib']
        if sys.platform == 'darwin':
            conf.env.INCLUDES_OS = ['/Applications/Pd-extended.app/Contents/Resources/include', '/usr/local/include/flext']
            conf.env.CXXFLAGS_OS = ['-m32']
            conf.env.LINKFLAGS_OS = ['-m32','-flat_namespace']
            conf.env.cxxshlib_PATTERN = '%s.pd_darwin'


def build(bld):
    #TRACKER
    bld.shlib(
     source       = 'vrpd_tracker/vrpd_tracker.cpp' ,
     use          = ['OS','VRPD'],
     target       = 'vrpd_tracker',
     )
    #BUTTON
    bld.shlib(
     source       = 'vrpd_button/vrpd_button.cpp' ,
     use          = ['OS','VRPD'],
     target       = 'vrpd_button',
     )
    #ANALOG
    bld.shlib(
     source       = 'vrpd_analog/vrpd_analog.cpp' ,
     use          = ['OS','VRPD'],
     target       = 'vrpd_analog',
     )




