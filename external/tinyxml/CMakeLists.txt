#########################################################
# Substance Framework tinyxml
#########################################################

# Tinyxml files
set (TINYXML_SOURCES

    tinystr.cpp
    tinystr.h
    tinyxml.cpp
    tinyxml.h
    tinyxmlerror.cpp
    tinyxmlparser.cpp
)

#autogen_source_groups("${TINYXML_SOURCES}")

add_library(tinyxml STATIC ${TINYXML_SOURCES})