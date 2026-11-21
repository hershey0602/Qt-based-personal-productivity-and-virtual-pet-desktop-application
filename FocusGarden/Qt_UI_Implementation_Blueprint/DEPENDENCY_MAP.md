# 依赖方向与禁止事项

构造引用依赖图由API_REGISTRY.json生成并在本次做静态无环核对；没有声称已运行C++。

| 具体类 | 依赖端口 | 责任 |
|---|---|---|
| Repository B | QtSql/文件路径 | 唯一SQLite连接与事务执行 |
| SettingsService D | QSettings/固定路径 | 唯一配置写入 |
| TaskManager B / StatisticsService B | IRepository | 任务校验/统计读写 |
| FocusEngine B | IRepository、ISettingsService | 计时、记录、读取下一轮默认 |
| RewardService C / PetManager C | IRepository | 金币/喂食政策，调用原子事务 |
| MusicController D / CompletionSound D | ISettingsService | 唯一播放器、提示音 |
| CompletionCoordinator A | IFocusEngine、IRewardService、IRepository | 保存成功后的结算协调及恢复 |
| AppServices A | 各业务端口及Coordinator | 一个转发门面，统一信号 |
| MainWindow/七页 | AppServices | UI读写请求与展示 |

Repository不能引用上层服务。C不持有B具体FocusEngine，B不持有C具体RewardService；通过A协调器与共享值类型沟通。信号通知可以返回UI，但不形成构造/拥有依赖。一个QObject只由AppComposition或页面父对象销毁，不再套另一份拥有指针。

禁止：C再开数据库；D用QSettings存金币；页面缓存余额后自行加减；导航时创建另一个FocusEngine/QMediaPlayer；四人各定义一个Task/Result；刷新槽反向写数据库；B/C同时各发一次完成金币；为解决编译而修改别人公共接口。
