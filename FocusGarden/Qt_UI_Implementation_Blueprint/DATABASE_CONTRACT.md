# 数据库与事务契约

FG-API-1.0。B负责Repository及建表/迁移的实现；A维护冻结端口；C负责奖励与喂食业务规则并调用原子端口。SCHEMA_V1.sql是SQL蓝图，不是运行中的SQLite文件。schema中数值枚举与DATA_MODELS完全一致。

## 单一所有权

- Live应用只由Repository创建一个命名QSQLITE连接，所有操作在GUI线程同步串行执行。对象生命周期归AppComposition。C/D/页面不调用addDatabase、transaction或exec；不暴露QSqlDatabase/QSqlQuery给页面。
- Repository持有连接，TaskManager、FocusEngine、StatisticsService、RewardService、PetManager共享非拥有引用。Repository不引用这些服务；因此不存在Repo反调C、C再调B的循环。
- 钱包只来自SELECT COALESCE(SUM(delta),0) FROM coin_ledger。pet_state只存累计EXP/心情，等级推导。PetSnapshot.coins是只读快照；不维护wallet表、Settings金币或页面本地余额。
- appData路径：QStandardPaths::AppDataLocation/focusgarden.sqlite；D设置路径同目录app.ini。路径由A装配并mkdir，用户数据不放代码仓库。A在main中一次设置组织名FocusGardenStudentTeam、应用名FocusGarden；D只接收固定app.ini路径，不再次修改应用标识。

## 表与写入边界

| 表 | 业务发起 | 执行SQL/事务 | 只读使用 |
|---|---|---|---|
| tasks | B TaskManager | B Repository | 首页/任务/统计/Focus新会话校验 |
| sessions | B FocusEngine | B Repository | 统计、C结算、A恢复协调 |
| coin_ledger | C RewardService或PetManager | B Repository原子方法 | C宠物/总览/历史金币 |
| pet_state | C PetManager | B Repository原子方法 | 宠物/总览 |

## 结算事务 creditRewardAtomically

1. BEGIN IMMEDIATE；事务内部重读session，要求status=Completed、mode为Focus或Custom、elapsed=planned。金额expectedCoins必须等于planned_seconds/300*4，不能接受UI给出的金额。
2. 查ux_focus_reward对应流水。已存在则不再插入，读取原delta并返回newlyCredited=false；确认该会话reward_state为Settled。此分支也是重试成功，不是失败。
3. 不存在：插入operation_id='reward:'+sessionId的正收入行；将同session的reward_state=Settled；两步同一事务。
4. COMMIT成功后返回SettlementReceipt与当前PetSnapshot，newlyCredited=true。任一SQL或提交失败ROLLBACK，余额/奖励标记均不改变。
5. FocusEngine只在completeSession已成功后发布completionPersisted；Coordinator才调用C。保存失败时ledger必须无此会话行。唯一索引是最后防线，服务中的if判断不能替代索引。

## 喂食事务 feedPetAtomically

C为一次喂食意图生成OperationId。BEGIN IMMEDIATE后先检查operation_id：已有feed行时返回原事件receipt且newlyApplied=false，避免同操作重试再扣钱。正常新操作读取ledger余额与pet_state；<15返回InsufficientCoins并回滚，三个值完全不变。

余额足够则在同一事务插入-15流水（记录本次EXP/心情前后值），更新pet_state为EXP+10、min(100,mood+15)，COMMIT成功才返回新快照和升级结果。任何失败全部回滚。等级从前后EXP算，Lv3仍可喂食；不发Lv4事件。UI操作处理中禁用重复提交，下一次明确点击是新的喂食意图。

## 专注写入和恢复

insertRunningSession先成功，再开始计时；pause/resume只保存对应会话状态。completeSession第一次将Running转Completed/Pending并固化endedAt/endedLocalDate；重复相同完成写入幂等；若已Abandoned或字段冲突返回StaleSession，不能覆盖。

abandonSession只允许Running/Paused，确认期间若自然完成已发生则不能覆盖Completed。恢复启动只将遗留Running/Paused转Abandoned，elapsed按已保存值处理、统计为0、不补奖；其结束时间用恢复时刻标识异常终止记录，不假装知道崩溃瞬间。Completed/Pending保留，Coordinator枚举并重试C。

休息不进入sessions/coin_ledger；活动退出时休息只停止内存计时。永久失败不得自动把专注记录降为放弃；显示可重试错误。

## 查询与口径

任务排序：due为NULL最后；非空due升序；相同due时createdAtUtc降序，再以id降序打破完全相同时间。查询始终过滤deleted=0。搜索范围已由队长确认仅标题，使用绑定参数与LIKE转义，备注不参与搜索。category/priority/state叠加AND。

今日截止任务用本地日开始/次日开始转换UTC后的半开区间查due，完成项也可出现，limit=3。所有任务完成率是未删除任务中的completed/total，total=0返回TaskCounts(0,0)，UI显示暂无任务。

有效专注只统计status=Completed且非休息的elapsed_seconds，含已完成待结算记录；金币尚未到账不抹去已完成时长。按首次结束的ended_local_date分组，不能用重试日期。history含完成/放弃，降序，放弃金币为0；Pending显示待结算。最近7天固定补0，首页和统计同StatisticsService快照，不另算另一口径。

## 日1与日5边界

日1可用MockRepository让七页编译；B当天优先建真实schema与上述原子端口，让C不等整套任务UI即可对接。日5验收只接受Live真实SQLite与重启持久化；Mock/临时内存库/仅进程内数据不能代替最终实现。后续阶段只做测试、修复、优化，不把本节事务留到后续开发。
