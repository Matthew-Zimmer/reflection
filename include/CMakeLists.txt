target_include_directories(reflection INTERFACE 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}> 
	$<INSTALL_INTERFACE:include/reflection>)

target_include_directories(reflection_test PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)

set(headers 
	initializable.hpp
	operator_helpers.hpp
	reflection.hpp
	variables.hpp)

install(FILES ${headers} DESTINATION include/reflection)