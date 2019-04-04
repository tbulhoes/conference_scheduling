cmake_minimum_required (VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMake ${CMAKE_CURRENT_SOURCE_DIR}/CMake/Packages)

include(BapcodApplicationProject)
include(BapcodApplicationPackage)
include(Documentation)
include(Profile)

# disabled as it is not compatible with Mac
#set(XERCES_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Tools/xerces-c/include)
#set(XERCES_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/Tools/xerces-c/lib/libxerces-c.so;${CMAKE_CURRENT_SOURCE_DIR}/Tools/xerces-c/lib/libxerces-c-3.2.so)
#set(XSD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Tools/XSD/libxsd)
#message (STATUS "XSD_INCLUDE_DIR=${XSD_INCLUDE_DIR}")


# Optional: Set other libraries
# disabled as it is not compatible with Mac
#set(USER_LIBRARIES CVRPSEP CliqueSep RheccSep ${JNI_LIBRARIES} ${XERCES_LIBRARY})
#set(USER_INCLUDE_DIR "${CVRPSEP_INCLUDE_DIR}" "${CLQSEP_INCLUDE_DIR}" "${RHECCSEP_INCLUDE_DIR}" "${JNI_INCLUDE_DIRS}" ${XERCES_INCLUDE_DIR} ${XSD_INCLUDE_DIR})
#set(USER_LIBRARIES CVRPSEP CliqueSep RheccSep )
#set(USER_INCLUDE_DIR "${CVRPSEP_INCLUDE_DIR}" "${CLQSEP_INCLUDE_DIR}" "${RHECCSEP_INCLUDE_DIR}" )
# include_directories("${CVRPSEP_INCLUDE_DIR}")

# Execute the build
set_project_version(1 0 0)

# Build Application
try_build_bapcod_application()

# Make Package
try_build_bapcod_application_package()

# Build Documentation
try_build_documentation()

# Make Profile
try_build_profile()
