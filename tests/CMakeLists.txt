find_package(Qt6 REQUIRED COMPONENTS Test)

enable_testing()

add_executable(tst_document
    tst_document.cpp
)
target_link_libraries(tst_document PRIVATE qdraw_core Qt6::Test)
add_test(NAME tst_document COMMAND tst_document)

add_executable(tst_hittesting
    tst_hittesting.cpp
)
target_link_libraries(tst_hittesting PRIVATE qdraw_core Qt6::Test)
add_test(NAME tst_hittesting COMMAND tst_hittesting)

add_executable(tst_history
    tst_history.cpp
)
target_link_libraries(tst_history PRIVATE qdraw_core Qt6::Test)
add_test(NAME tst_history COMMAND tst_history)

add_executable(tst_math
    tst_math.cpp
)
target_link_libraries(tst_math PRIVATE qdraw_core Qt6::Test)
add_test(NAME tst_math COMMAND tst_math)
