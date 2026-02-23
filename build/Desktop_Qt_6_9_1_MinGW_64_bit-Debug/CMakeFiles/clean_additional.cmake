# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\CampusMap_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\CampusMap_autogen.dir\\ParseCache.txt"
  "CampusMap_autogen"
  )
endif()
