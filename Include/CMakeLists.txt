Target_Include_Directories(Reflection INTERFACE 
	$<BUILD_INTERFACE:.> 
	$<INSTALL_INTERFACE:include/Reflection>)

Set(Headers 
	Initializable.hpp
	Operator_Helpers.hpp
	Reflection.hpp
	Variables.hpp)

Install(FILES ${Headers} DESTINATION include/Reflection)