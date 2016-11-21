cmake_minimum_required(VERSION 2.6)
project(order_8th)



add_executable(order_8th main.cpp equation.cpp)

target_link_libraries(order_8th gsl gslcblas)


install(TARGETS order_8th RUNTIME DESTINATION bin)
