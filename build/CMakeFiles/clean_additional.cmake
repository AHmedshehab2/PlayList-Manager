# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "CMakeFiles\\MusicPlaylistManager_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\MusicPlaylistManager_autogen.dir\\ParseCache.txt"
  "MusicPlaylistManager_autogen"
  )
endif()
