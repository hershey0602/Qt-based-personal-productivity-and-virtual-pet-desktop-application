# D 测试构建登记变更说明（交 A）

请由 A 在公共构建文件中登记以下测试，不由 D 直接修改 `CMakeLists.txt`：

```cmake
add_executable(fg_tests_d tests/d/test_d_services.cpp)
target_link_libraries(fg_tests_d PRIVATE fg_ui Qt6::Test)
add_test(NAME d_services COMMAND fg_tests_d -platform offscreen)
```

验证命令：

```powershell
& 'C:\Qt\Tools\CMake_64\bin\cmake.exe' --build 'D:\qt_project\focusgarden\build-d' --target fg_tests_d
& 'C:\Qt\Tools\CMake_64\bin\ctest.exe' --test-dir 'D:\qt_project\focusgarden\build-d' -R d_services --output-on-failure
```

当前状态：登记待 A 合并；本机配置阶段因缺少 Ninja/MinGW Make 生成器未执行。
