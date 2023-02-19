add_library(Card Card.cpp Card.h)
add_subdirectory(tests)
target_include_directories(Card PUBLIC .)
