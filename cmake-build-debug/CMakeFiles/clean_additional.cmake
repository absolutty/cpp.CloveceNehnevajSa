# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\CloveceNehnevajSa_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\CloveceNehnevajSa_autogen.dir\\ParseCache.txt"
  "CloveceNehnevajSa_autogen"
  )
endif()
