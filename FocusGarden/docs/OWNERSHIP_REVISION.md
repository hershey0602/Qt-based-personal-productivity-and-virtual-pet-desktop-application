# 设置模块分工修订

本次分工修订：D负责音乐、设置、测试与部署，包括 SettingsPage 和 SettingsService；A负责主窗口、总览、统一UI与整合。分工以本次修订为准，PRD功能规则和UI规范仍沿用原基线。

| 成员 | 最新负责范围 |
|---|---|
| A | 主窗口、Dashboard、统一UI、公共组件、公共接口合并与整合 |
| B | Tasks、Focus、Statistics、数据库、计时；读取D的设置用于下一轮 |
| C | Pet、金币奖励、宠物成长 |
| D | Music、Settings、MusicController、SettingsService、测试、部署 |

D拥有SettingsPage和SettingsService的实现权；A继续维护MainWindow、公共组件、QSS、资源表和AppServices具体转发门面与公共抽象端口，负责导航注册与依赖装配。D通过既定门面与B对接默认时长和提示音配置，不直接重置活动专注。

本修订覆盖原PRD及旧交付中的设置负责人表述；SOURCE_PRD.txt是依据上传原文及本轮明确决定整理的有效PRD；原始上传文件只在本地归档保留追溯。视觉参数与素材保持原基线；前5天完成全部必做真实功能，日6后仅测试修复优化部署答辩。
