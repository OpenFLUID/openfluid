
ADD_LIBRARY(thirdparty-tinyxml2 STATIC tinyxml2.cpp)

SET_TARGET_PROPERTIES(thirdparty-tinyxml2 
                      PROPERTIES POSITION_INDEPENDENT_CODE True)