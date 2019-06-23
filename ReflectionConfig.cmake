Get_Filename_Component(SELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
Include(${SELF_DIR}/Reflection.cmake)

Find_Package(Concept REQUIRED)
Find_Package(Meta REQUIRED)