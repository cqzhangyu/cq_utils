cmake_minimum_required(VERSION 3.7)
project(logger)

# c
aux_source_directory(src/c C_SRC)
add_executable(CTest test/c/CTest.c ${C_SRC})

set(CTargets
	CTest)

foreach(_target ${CTargets})
	target_include_directories(${_target} PRIVATE src/c)
endforeach()

# c++
aux_source_directory(src/cpp CPP_SRC)
add_executable(CppTest test/cpp/CppTest.cc ${CPP_SRC})

set(CppTargets
	CppTest)

foreach(_target ${CppTargets})
	target_include_directories(${_target} PRIVATE src/cpp)
endforeach()
