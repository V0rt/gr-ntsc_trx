find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_NTSC_TRX gnuradio-ntsc_trx)

FIND_PATH(
    GR_NTSC_TRX_INCLUDE_DIRS
    NAMES gnuradio/ntsc_trx/api.h
    HINTS $ENV{NTSC_TRX_DIR}/include
        ${PC_NTSC_TRX_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_NTSC_TRX_LIBRARIES
    NAMES gnuradio-ntsc_trx
    HINTS $ENV{NTSC_TRX_DIR}/lib
        ${PC_NTSC_TRX_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-ntsc_trxTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_NTSC_TRX DEFAULT_MSG GR_NTSC_TRX_LIBRARIES GR_NTSC_TRX_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_NTSC_TRX_LIBRARIES GR_NTSC_TRX_INCLUDE_DIRS)
