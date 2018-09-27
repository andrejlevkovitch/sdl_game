# find sdl2 and modules

include(FindPackageHandleStandardArgs)

find_path(SDL2_INCLUDE_DIR SDL.h PATH_SUFFIXES SDL2)

if(SDL2_USE_STATIC_LIBS STREQUAL ON)
  exec_program(sdl2-config ARGS --static-libs OUTPUT_VARIABLE SDL2_LIBRARY
    RETURN_VALUE SDL2_CONFIG_RETVAL)
  if(SDL2_CONFIG_RETVAL)
    message(ERROR "Can't be linked static, becose not found sdl2-config")
  else()
  endif()
  find_library(SDL2_IMAGE_LIBRARY libSDL2_image.a)
  find_library(PNG_LIBRARY libpng12.so.0)
  find_library(JPEG_LIBRARY libjpeg.so.8)
  find_library(TIFF_LIBRARY libtiff.so.5)
  find_library(WEBP_LIBRARY libwebp.so.5)
  find_library(Z_LIBRARY libz.so.1)
  find_library(JBIG_LIBRARY libjbig.so.0)
  set(SDL2_IMAGE_LIBRARY "${SDL2_IMAGE_LIBRARY};${PNG_LIBRARY};${JPEG_LIBRARY};${TIFF_LIBRARY};${WEBP_LIBRARY};${Z_LIBRARY};${JBIG_LIBRARY}")
else()
  find_library(SDL2_LIBRARY SDL2)
  find_library(SDL2_IMAGE_LIBRARY SDL2_image)
endif()

find_package_handle_standard_args(SDL2 FOUND_VAR SDL2_FOUND
  REQUIRED_VARS SDL2_LIBRARY SDL2_INCLUDE_DIR)
