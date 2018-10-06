# - Try to find Imagegnome-keyring-1
# Once done, this will define
#
#  gnome-keyring-1_FOUND - system has gnome-keyring-1
#  gnome-keyring-1_INCLUDE_DIRS - the gnome-keyring-1 include directories
#  gnome-keyring-1_LIBRARIES - link these to use gnome-keyring-1


# Dependencies
find_package(PkgConfig)

# Use pkg-config to get hints about paths
pkg_check_modules(LIBSECRET libsecret-1 )
