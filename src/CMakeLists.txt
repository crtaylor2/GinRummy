add_library(GinRummy GinRummy.cpp GinRummy.h GinRummyRestServer.cpp GinRummyRestServer.h)
add_executable(GinRummyExe GinRummy.cpp GinRummy.h GinRummyRestServer.cpp GinRummyRestServer.h main.cpp)
add_subdirectory(tests)

find_package(cpprestsdk REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

target_link_libraries(GinRummy
 PRIVATE
  Card
  cpprestsdk::cpprest
  ${JSONCPP_LIBRARIES})

target_link_libraries(GinRummyExe
 PRIVATE
  Card
  cpprestsdk::cpprest
  ${JSONCPP_LIBRARIES})

target_include_directories(GinRummy PUBLIC .)
