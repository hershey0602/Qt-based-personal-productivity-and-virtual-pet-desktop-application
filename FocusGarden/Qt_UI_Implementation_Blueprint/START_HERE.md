# 唯一开工技术入口

本次仅整理开工资料，不创建Qt工程。下一次Codex按仓库中文任务书和FG-API-1.0生成同一个Qt_UI_Starter。读完根目录00指南与队长第1天手册，再按此顺序给AI：

1. docs/SOURCE_PRD.txt（有效修订口径：仅标题搜索、设置归D）
2. DATA_MODELS.md、API_CONTRACT.md、SIGNAL_SLOT_MATRIX.md、UI_BINDING_CONTRACT.md
3. DATABASE_CONTRACT.md、SCHEMA_V1.sql、DEPENDENCY_MAP.md、FILE_OWNERSHIP.json、PROJECT_STRUCTURE.md
4. 既有UI规范、DesignTokens、UI_COMPONENTS、resources.qrc、main.qss、layouts中对应页
5. 自己中文角色目录内的完整任务书、AI输入清单、完整Codex指令和验收表

前5天完成全部真实功能；Mock仅日1支架/测试替身。A最早发布接口和可编译壳，B/C/D增量替换真实类，每天持续提交合并。当前没有CMakeLists/真实接口头文件/可执行程序，准确声明从本目录生成，不能引用旧资料的简写自创接口。
