
INCLUDE_DIRECTORIES(${GDAL_INCLUDE_DIR})


SET(OPENFLUID_CORE_CPP Value.cpp UnstructuredValue.cpp
                       IntegerValue.cpp BooleanValue.cpp DoubleValue.cpp StringValue.cpp NullValue.cpp
                       VectorValue.cpp MatrixValue.cpp MapValue.cpp TreeValue.cpp
                       GeoValue.cpp GeoRasterValue.cpp GeoVectorValue.cpp
                       DateTime.cpp
                       SpatialUnit.cpp UnitsCollection.cpp SpatialGraph.cpp
                       ValuesBuffer.cpp ValuesBufferProperties.cpp
                       Variables.cpp
                       Attributes.cpp
                       Event.cpp EventsCollection.cpp
                       Datastore.cpp DatastoreItem.cpp
                       )

SET(OPENFLUID_CORE_HPP Vector.hpp Matrix.hpp Tree.hpp
                       Value.hpp SimpleValue.hpp CompoundValue.hpp UnstructuredValue.hpp
                       IntegerValue.hpp BooleanValue.hpp DoubleValue.hpp StringValue.hpp NullValue.hpp 
                       VectorValue.hpp MatrixValue.hpp MapValue.hpp TreeValue.hpp
                       GeoValue.hpp GeoRasterValue.hpp GeoVectorValue.hpp
                       IndexedValue.hpp
                       TypeDefs.hpp
                       DateTime.hpp
                       SpatialUnit.hpp UnitsCollection.hpp SpatialGraph.hpp
                       ValuesBuffer.hpp ValuesBufferProperties.hpp
                       Variables.hpp
                       Attributes.hpp          
                       Event.hpp EventsCollection.hpp
                       Datastore.hpp DatastoreItem.hpp
                       )

ADD_LIBRARY(openfluid-core SHARED ${OPENFLUID_CORE_CPP} ${OPENFLUID_CORE_HPP})

SET_TARGET_PROPERTIES(openfluid-core PROPERTIES VERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}" 
                                        SOVERSION "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}.${OPENFLUID_VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        RUNTIME_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${OFBUILD_DIST_LIB_DIR}"
                                        COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                        DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")

TARGET_LINK_LIBRARIES(openfluid-core
                      ${GDAL_LIBRARIES})

INSTALL(TARGETS openfluid-core
        RUNTIME DESTINATION ${OFBUILD_BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${OFBUILD_LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${OFBUILD_LIB_INSTALL_PATH})
   

INSTALL(FILES ${OPENFLUID_CORE_HPP} DESTINATION ${OPENFLUID_INCLUDE_INSTALL_PATH}/core)

IF(OFBUILD_ENABLE_TESTING)
  ADD_SUBDIRECTORY(tests)
ENDIF()  
