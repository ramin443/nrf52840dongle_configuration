# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/Users/udash/nordic/MyApps/blinky_1")
  file(MAKE_DIRECTORY "C:/Users/udash/nordic/MyApps/blinky_1")
endif()
file(MAKE_DIRECTORY
  "C:/Users/udash/nordic/MyApps/build/blinky_1"
  "C:/Users/udash/nordic/MyApps/build/_sysbuild/sysbuild/images/blinky_1-prefix"
  "C:/Users/udash/nordic/MyApps/build/_sysbuild/sysbuild/images/blinky_1-prefix/tmp"
  "C:/Users/udash/nordic/MyApps/build/_sysbuild/sysbuild/images/blinky_1-prefix/src/blinky_1-stamp"
  "C:/Users/udash/nordic/MyApps/build/_sysbuild/sysbuild/images/blinky_1-prefix/src"
  "C:/Users/udash/nordic/MyApps/build/_sysbuild/sysbuild/images/blinky_1-prefix/src/blinky_1-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/udash/nordic/MyApps/build/_sysbuild/sysbuild/images/blinky_1-prefix/src/blinky_1-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/udash/nordic/MyApps/build/_sysbuild/sysbuild/images/blinky_1-prefix/src/blinky_1-stamp${cfgdir}") # cfgdir has leading slash
endif()
