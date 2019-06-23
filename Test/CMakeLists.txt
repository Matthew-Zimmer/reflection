Target_Include_Directories(Reflection PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../Include>
	$<BUILD_INTERFACE:/usr/local/include/Concept>
	$<BUILD_INTERFACE:/usr/local/include/Meta>)

Target_Compile_Features(Reflection 
    PUBLIC cxx_std_17)

Set(CMAKE_CXX_STANDARD "libc++")