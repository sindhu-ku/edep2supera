set(name edep2supera)

# Get all the source/header files:
file(GLOB SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cxx)
file(GLOB HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)
list(REMOVE_ITEM HEADERS "LinkDef.h")

# Generate dictionary
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
ROOT_GENERATE_DICTIONARY(G__edep2supera
  ${HEADERS}
  #OPTIONS -inlineInputHeader
  LINKDEF LinkDef.h)

# Build the library.
add_library(edep2supera SHARED ${SOURCES} G__edep2supera.cxx)

# Link libraries
target_link_libraries(edep2supera
    ${PYTHON_LIBRARIES}
    ${ROOT_LIBRARIES}
    -L${EDepSim_DIR}/lib -ledepsim_io
    -L${SUPERA_LIB_DIR} -lsupera
)

# Install
install(TARGETS edep2supera
    LIBRARY DESTINATION ${CMAKE_PACKAGE_DIR}/lib
    ARCHIVE DESTINATION ${CMAKE_PACKAGE_DIR}/lib
    RUNTIME DESTINATION ${CMAKE_PACKAGE_DIR}/bin
    INCLUDES DESTINATION ${CMAKE_PACKAGE_DIR}/include
    )

install (FILES ${HEADERS}
    DESTINATION ${CMAKE_PACKAGE_DIR}/include/edep2supera/${name})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libedep2supera_rdict.pcm DESTINATION ${CMAKE_PACKAGE_DIR}/lib)