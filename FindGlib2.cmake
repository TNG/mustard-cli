# - Try to find Imageglib-2.0
# Once done, this will define
#
#  glib-2.0_FOUND - system has glib-2.0
#  glib-2.0_INCLUDE_DIRS - the glib-2.0 include directories
#  glib-2.0_LIBRARIES - link these to use glib-2.0


# Dependencies
find_package(PkgConfig)

# Use pkg-config to get hints about paths
pkg_check_modules(GLIB2 glib-2.0 )
