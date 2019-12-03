get_Filename_Component(SELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(${SELF_DIR}/reflection.cmake)

find_package(concept REQUIRED)
find_package(meta REQUIRED)