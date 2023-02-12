# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\thermalfl_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\thermalfl_autogen.dir\\ParseCache.txt"
  "thermalfl_autogen"
  )
endif()
