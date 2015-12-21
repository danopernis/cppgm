add_test(
    NAME pa1
    COMMAND
        ${CMAKE_CURRENT_SOURCE_DIR}/runtest.sh
        ${CMAKE_SOURCE_DIR}/pa/pa1/tests
        ${CMAKE_BINARY_DIR}/pptoken
)
