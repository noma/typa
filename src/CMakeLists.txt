# Copyright (c) 2017 Matthias Noack <ma.noack.pr@gmail.com>
#
# See accompanying file LICENSE and README for further information.

# do not put executable into subdir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# test application
option(NOMA_TYPE_TESTS "Build tests.")

if(${NOMA_TYPE_TESTS})
	add_executable(test_parser test_parser.cpp)
	target_link_libraries(test_parser noma_typa)
	set_target_properties(test_parser PROPERTIES
		CXX_STANDARD 11
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
	)
endif()

