CMake_Minimum_Required(VERSION 3.13)

Project(Reflection)

If(DEFINED TESTING)
	If(${TESTING} STREQUAL true)
		Add_Executable(Reflection "Test/Main.cpp")
		Target_Compile_Definitions(Reflection PRIVATE TESTING)
		Add_Subdirectory(Test)
	Else()
		Add_Library(Reflection INTERFACE)
	EndIf()
	Unset(TESTING CACHE)
ELSE()
	Add_Library(Reflection INTERFACE)
EndIf()

Add_Subdirectory(Source)
Add_Subdirectory(Include)
Add_Subdirectory(Imp)

Target_Compile_Features(Reflection 
    INTERFACE cxx_std_17)

Set(CMAKE_CXX_STANDARD "libc++")
#Set(CMAKE_CXX_FLAGS "-Wall" "-Wextra")

Install(TARGETS Reflection EXPORT Reflection DESTINATION lib/Reflection)
Install(EXPORT Reflection DESTINATION lib/Reflection)
Install(FILES ReflectionConfig.cmake DESTINATION lib/Reflection)

Find_Package(Concept REQUIRED)
Find_Package(Meta REQUIRED)
Target_Link_Libraries(Reflection INTERFACE Concept Meta)