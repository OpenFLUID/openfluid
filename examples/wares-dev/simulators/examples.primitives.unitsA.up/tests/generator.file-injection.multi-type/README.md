# generator.file-injection.multi-type simulator for custom units class and variable
Template to be converted. 

Example of configuration with CMake:
```cmake
FILE(GLOB_RECURSE TEMPLATE_FILES "${THIS_GENERATOR_PATH}/*")
FOREACH(file ${TEMPLATE_FILES})
    FILE(RELATIVE_PATH rel_path "${THIS_GENERATOR_PATH}" "${file}")
    SET(varname "var10")
    SET(unitsclass "unitsA")
    SET(type "DoubleValue")
    SET(missing_value "NA")
    SET(output "${CMAKE_BINARY_DIR}/${rel_path}")
    CONFIGURE_FILE("${file}" "${output}")
ENDFOREACH()
```