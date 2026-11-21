# 状态与失败处理冻结

FG-API-1.0。数据库SessionStatus和界面FocusState不是同一个枚举；已完成未结算在库里是Completed/Pending，在当前专注界面是RewardPending。不能把结算失败写成专注失败或放弃。

| UI状态 | 开始 | 暂停/继续 | 放弃 | 调任务/时长 | 重试 | 正常关闭 |
|---|---|---|---|---|---|---|
| Idle | 手动可用 | 禁用 | 禁用 | 可用 | 隐藏 | 可 |
| Running | 禁用 | 仅暂停 | 可，须确认 | 禁用 | 隐藏 | 确认且保存Abandoned成功才可 |
| Paused | 禁用 | 仅继续 | 可，须确认 | 禁用 | 隐藏 | 同上 |
| SaveFailed | 禁用 | 禁用 | 禁用 | 禁用 | 只重试原完成记录保存 | 先保存成功，失败保持窗口 |
| RewardPending | 禁用 | 禁用 | 禁用 | 禁用 | 只重试原sessionId结算 | 记录已持久化，可退出并启动恢复 |
| Completed | 下一轮须先prepareNextRound或configure | 禁用 | 禁用 | 可，转Idle | 隐藏 | 可 |

Idle→start保存Running成功→Running；Running→pause成功→Paused；Paused→resume成功→Running。计时独立于页面、最小化和导航。自然达到计划冻结时间→保存成功→RewardPending→结算成功→Completed；记录保存失败→SaveFailed，禁止调奖励。结算成功结果关窗/继续学习只prepareNextRound，不自动start。

Running/Paused确认放弃，持久化成功才转Idle；取消保持原状态。确认框打开期间计时继续，若自然结束已发生，abandon必须识别状态过期，不能覆盖Completed。休息只内存计时，结束提示并Idle，不产生有效专注或金币。

## 设置不会重置当前轮

prepareNextRound读取D.preferences。configure来自明确时长按钮时标ExplicitSelection。D成功保存发preferencesChanged；B只在Idle且DefaultSetting时更新未开始草稿，不改任何活动/失败/已保存待结算轮。下一轮再读新值。默认休息5/10同理；关闭提示音后下次自然完成不播放提供的本地短音效。

## 表单与界面状态

TaskEditDialog新增编辑复用，编辑保留completed。标题trim后1～100码点、备注<=500、due可空可过去。保存失败留窗，取消/X/Esc有改动则二层确认；继续编辑保留输入。删除确认失败留窗，列表和详情不伪报消失。

无任务与筛选无命中分开，后者提供清除筛选。音乐首次空清单，坏文件单条提示；无文件时播放/seek等禁用。金币不足喂食按钮仍可点击但只反馈不足，钱包/经验/心情不变。满级无Lv4与虚假升级进度。

## 启动恢复

先恢复库中遗留Running/Paused为Abandoned，再枚举Completed/Pending交C幂等补偿结算。当前FocusEngine以Idle启动，无需一直停在Focus页才能恢复。歌曲不自动播。数据库打开失败不能让空白假数据冒充正式读库成功。

所有上述异常分支属于前5天必做功能；第6天后测试发现缺陷则修复，不能把原本缺失的持久化、结算、计时或播放称作“后续新开发”。
