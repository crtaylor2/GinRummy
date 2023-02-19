add_library(GinRummy GinRummy.cpp GinRummy.h)
add_executable(GinRummyExe GinRummy.cpp GinRummy.h main.cpp)
add_subdirectory(tests)

target_link_libraries(GinRummy
 PRIVATE
  Card)

target_link_libraries(GinRummyExe
 PRIVATE
  Card)

target_include_directories(GinRummy PUBLIC .)
