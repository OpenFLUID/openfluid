
SET(OF_SYSTEM_ARCH "")
SET(OF_DISTRO_ID "")
SET(OF_DISTRO_CODE "")
SET(OF_DISTRO_VERSION "")
SET(OF_SYSTEM_IS_64BITS NO)
SET(OF_SYSTEM_IS_WINDOWS NO)
SET(OF_SYSTEM_IS_LINUX NO)
SET(OF_SYSTEM_IS_APPLE NO)
SET(OF_DISTRO_IS_UBUNTU NO)
SET(OF_DISTRO_IS_FEDORA NO)
SET(OF_DISTRO_IS_CENTOS NO)
SET(OF_LIBDIR_SUFFIX "")


IF("${CMAKE_SIZEOF_VOID_P}" EQUAL "8" )
  SET(OF_SYSTEM_IS_64BITS YES)
ENDIF("${CMAKE_SIZEOF_VOID_P}" EQUAL "8" )


IF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  SET(OF_SYSTEM_IS_WINDOWS YES)
  IF(OF_SYSTEM_IS_64BITS)
    SET(OF_SYSTEM_ARCH "win64")
  ELSE(OF_SYSTEM_IS_64BITS)
    SET(OF_SYSTEM_ARCH "win32")
  ENDIF(OF_SYSTEM_IS_64BITS)
ENDIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")


IF(UNIX AND NOT CYGWIN)
  IF(NOT APPLE) 

    FIND_PROGRAM(LSB_RELEASE_PROG "lsb_release")
    IF(LSB_RELEASE_PROG-NOTFOUND)
      SET(OF_SYSTEM_ARCH "noarch")
      SET(OF_DISTRO_ID "nodistro")
    ELSE(LSB_RELEASE_PROG-NOTFOUND)

      EXECUTE_PROCESS(COMMAND "${LSB_RELEASE_PROG}" "-is" OUTPUT_VARIABLE OF_DISTRO_ID OUTPUT_STRIP_TRAILING_WHITESPACE)
      EXECUTE_PROCESS(COMMAND "${LSB_RELEASE_PROG}" "-cs" OUTPUT_VARIABLE OF_DISTRO_CODE OUTPUT_STRIP_TRAILING_WHITESPACE)
      EXECUTE_PROCESS(COMMAND "${LSB_RELEASE_PROG}" "-rs" OUTPUT_VARIABLE OF_DISTRO_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
    
      IF(OF_DISTRO_ID STREQUAL "Ubuntu")
        SET(OF_SYSTEM_IS_LINUX YES)
        SET(OF_DISTRO_IS_UBUNTU YES)
        IF(OF_SYSTEM_IS_64BITS)
          SET(OF_SYSTEM_ARCH "amd64")
        ELSE(OF_SYSTEM_IS_64BITS)
          SET(OF_SYSTEM_ARCH "i386")
        ENDIF(OF_SYSTEM_IS_64BITS)
      ENDIF(OF_DISTRO_ID STREQUAL "Ubuntu")
      
      IF(OF_DISTRO_ID STREQUAL "CentOS")
        SET(OF_SYSTEM_IS_LINUX YES)        
        SET(OF_DISTRO_IS_CENTOS YES)        
        IF(OF_SYSTEM_IS_64BITS)
          SET(OF_SYSTEM_ARCH "x86_64")
          SET(OF_LIBDIR_SUFFIX "64")
        ELSE(OF_SYSTEM_IS_64BITS)
          SET(OF_SYSTEM_ARCH "i686")
        ENDIF(OF_SYSTEM_IS_64BITS)
      ENDIF(OF_DISTRO_ID STREQUAL "CentOS")
      
    ENDIF(LSB_RELEASE_PROG-NOTFOUND)
      
      
  ELSE(NOT APPLE)
    SET(OF_SYSTEM_IS_APPLE YES)
  ENDIF(NOT APPLE)
  
ENDIF(UNIX AND NOT CYGWIN)