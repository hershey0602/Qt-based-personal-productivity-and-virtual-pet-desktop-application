add_executable(fg_tests_b test_repository.cpp)
target_link_libraries(fg_tests_b PRIVATE fg_ui Qt6::Test)
add_test(NAME b_repository COMMAND fg_tests_b -platform offscreen)
set_tests_properties(b_repository PROPERTIES
    ENVIRONMENT_MODIFICATION
        "PATH=path_list_prepend:$<TARGET_FILE_DIR:fg_ui>;PATH=path_list_prepend:D:/qt/6.11.2/mingw_64/bin;PATH=path_list_prepend:D:/qt/Tools/mingw1310_64/bin")
set_tests_properties(b_repository PROPERTIES
    ENVIRONMENT "QT_PLUGIN_PATH=D:/qt/6.11.2/mingw_64/plugins")
