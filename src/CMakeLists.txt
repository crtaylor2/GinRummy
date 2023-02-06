add_library(GinRummy GinRummy.cpp GinRummy.h Card.cpp Card.h main.cpp)
add_executable(GinRummyExe GinRummy.cpp GinRummy.h Card.cpp Card.h main.cpp)

target_include_directories(GinRummy PUBLIC .)
