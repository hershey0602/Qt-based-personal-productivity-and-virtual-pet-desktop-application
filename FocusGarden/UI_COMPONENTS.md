# Focus Garden 统一组件规范

视觉依据为 UI V2.1，业务依据为 PRD V2.0。本文件、Tokens、QSS 与组件参数表为四人共同基线。几何使用 Layout 实现，坐标只用于截图审查。组件 ID 点号改为下划线即 ObjectName；禁止依赖中文文本查找控件。

## 组件目录

| 组件 | Qt 类型与结构 | 标准样式与复用 |
|---|---|---|
| Sidebar / NavButton | QFrame + QVBoxLayout + 7 QPushButton | 宽218/188，按钮48/43高，图标22，圆角13；MainWindow唯一实例 |
| TopHeader | QFrame + QHBoxLayout | 高74，边距24/18；品牌、演示标识、本地运行；无头像/通知 |
| BaseCard | QFrame + QVBoxLayout | 白色、边1 #E5EBDD、radius20、padding19/15、gap16/12；所有页复用 |
| MetricCard | BaseCard + QHBoxLayout | 高105；49×49图标槽、12px标签、28px/800数值；数据从服务取 |
| PrimaryButton | QPushButton role=primary | 高40、radius12、水平padding13；主绿/白字；禁止各页自写颜色 |
| OutlineButton | QPushButton role=outline | 高40、白底绿灰边；次操作 |
| DangerButton | QPushButton role=danger | 高40、淡红底，危险动作二次确认 |
| IconButton | QPushButton + QIcon | 35×35或父页指定，图标18；必须tooltip/accessibility name |
| SmallButton | QPushButton | 高30、radius9、12px/600、padding10；任务行编辑/删除/专注 |
| Input / Search | QLineEdit | 高40、radius12、padding12，标题搜索；focus边2 #89B68C |
| Select | QComboBox | 高40、同输入框；Qt popup复用浅绿选中，限定枚举 |
| NoteInput | QTextEdit | 高86，padding10/12，纯文本，最多500字 |
| DateTimeInput | QDateTimeEdit + QCheckBox | 高40；不勾选时禁用；可过去日期 |
| StatusTag | QLabel | 11px/600、padding4/10、radius10；绿/黄/灰/错误按状态 |
| TaskRow | QFrame + QHBoxLayout | min73高、radius14；复选框、标题/标签、三操作；selected与done不同样式 |
| LinearProgress | QProgressBar | 高11、radius5、无文本；累计EXP；Lv3隐藏并显示满级 |
| TimerRing | QWidget + QPainter | 265×265，环19；中心54px/800，说明12、脚注11；不接受点击 |
| CompletionRing | QWidget + QPainter | 148×148，环17，数字26；任务数0时显示暂无任务，比例0 |
| WeekBarChart | QWidget + QPainter | 7列，柱宽不超过36，标签11px；刻度下限60分钟；零值2px基线 |
| TaskEditDialog | QDialog + 头/体/脚布局 | 宽530，padding24；新增/编辑同类；标题/备注/分类/优先级/可选截止 |
| ConfirmDialog | QDialog | 宽450；删除/放弃/退出/未保存共用；commit成功才accept |
| ResultDialog | QDialog | 宽450；奖励/升级共用；展示服务返回结果，不自行发币 |
| StateView | QFrame + SVG + QLabel + 可选按钮 | empty_tasks/empty_music/empty_history；图64；无数据不填假值 |
| InlineError | QLabel / QFrame | #C15B54 + 清楚文案与重试；不能仅靠颜色 |
| Toast | QFrame + QLabel | 390目标宽、padding12/16、radius12；左边4绿/红，3600ms；不遮挡关键操作 |
| ModalOverlay | QFrame + QDialog | rgba(29,50,31,.44)，覆盖客户区；任务未保存二层确认覆盖第一层 |

## 状态样式

以下 Profile 名与 Excel 的状态Profile列逐行关联。未列出的静态组件不产生悬停、按下或选中效果。

| Profile | 默认 | Hover | Pressed | Selected | Disabled |
|---|---|---|---|---|---|
|Static|只读，无悬停/按下/选中变化|不变|不变|不适用|随父控件禁用，文本不接受点击|
|primary|bg #5F9F64 / fg #FFFFFF / border #5F9F64|bg #4D8D53|bg #37773F|不适用|bg #EFF1E9 / fg #A3ADA0 / border #E5EBDD|
|outline|bg #FFFFFF / fg #3F5041 / border #D9E5D5|bg #EDF6E8|bg #DDEED5 / border #89B68C|不适用|bg #EFF1E9 / fg #A3ADA0 / border #E5EBDD|
|danger|bg #FFF3F0 / fg #C15B54 / border #F4D7D2|bg #FFE6E1|bg #F8D6CF|不适用|bg #EFF1E9 / fg #A3ADA0 / border #E5EBDD|
|soft|bg #EDF6E8 / fg #397946|bg #DDEED5|bg #DDEED5 / border #89B68C|不适用|bg #EFF1E9 / fg #A3ADA0|
|yellow|bg #F6DD8C / fg #263E2B|bg #EECF70|bg #E0BF5E|不适用|bg #EFF1E9 / fg #A3ADA0|
|quiet|透明 / fg #5F9F64|bg #EDF6E8|bg #DDEED5|不适用|bg #EFF1E9 / fg #A3ADA0|
|nav|透明 / fg #51664F|bg #F3F3D6|bg #DDEED5|bg #F6DD8C / fg #375134 / border #EFD171 / weight700|不禁用主导航；模态时由遮罩阻止交互|
|pill|bg #FFFFFF / fg #677C62 / border #D9E5D5|bg #EDF6E8|bg #DDEED5|bg #FFF5D6 / fg #826923 / border #E6C96D|bg #EFF1E9 / fg #A3ADA0 / 模式锁定|
|Input|bg #FFFFFF / border #DDE6D5 / fg #3F5041|同默认|焦点 border2 #89B68C|文字 selection bg #D5EDCF|bg #F2F4ED / fg #A3ADA0|
|TaskRow|bg #FFFFFF / border #E5EBDD|bg #FBFDF7|选择该任务并显示只读详情|bg #FFFEF3 / border #D8CB76|完成后专注按钮禁用；已完成背景 #F7FAF4|
|Check|16×16 / white / border #A6B59F / radius4|border #89B68C|切换完成/恢复|checked bg #5F9F64 + ic_check_white.svg|bg #E5EBDD|
|Slider|轨道 #E5EDD9 / 已选段及滑块 #5F9F64|滑块保持清楚|拖动显示实时值|当前值|滑块 #BECAB6；音乐无文件时禁用|
|Card|white / border1 #E5EBDD / radius20 / shadow 0 8 26 rgba(55,83,45,.055)|不变|不变|不适用|只禁用内部对应操作|
|Dialog|白色 / radius20 / 模态 / 遮罩 rgba(29,50,31,.44)|仅操作按钮变化|确认写入成功才关闭|不适用|父窗口不可交互|
|Ring|QPainter环19 / green #5F9F64 / track #E6EFDF|不变|不接受点击|剩余比例；统计环17|状态文案明确显示待开始/暂停/保存失败|
|Empty|state_*.svg 64×64 + 辅助文本 + 对应入口|只作用于入口按钮|新增/清除筛选/导入|不适用|无效操作禁用，不伪造数值|
|Toast|white / border1 #CFE2C9 / 左边4px绿 / radius12|不变|不适用|error左边 #C15B54|3600ms后关闭，关键错误保留可重试入口|

## Layout 与字体

所有页用 QVBoxLayout，页面标题59高，标题后18基准间隔，其余16；紧凑尺寸切为12。卡片包含内容后以布局约束求高度，七页整体用QScrollArea纵向滚动。任务长备注在只读详情内滚动，任务列表自身可滚动。不能把500字备注挤进固定单行。

字体按 Microsoft YaHei → Noto Sans CJK SC → 系统sans回退；用 setPixelSize，避免 pointSize 与DPI双重缩放。页面标题27/800，卡片标题16/700（原CSS760映射Qt可用字重），正文14/400，辅助12/400，统计28/800，计时54/800。CSS line-height1.52不能复制为QSS属性；单行按字体度量，多行按QTextDocument/明确行距与wrap规则。参数表的「单行字框」是基线字体度量，目标Windows字体须下阶段重新核对。

QSS 不支持的 CSS box-shadow、conic-gradient、transition、filter、grid、flex 均不进入 main.qss；用QGraphicsDropShadowEffect、QPainter和Qt Layout实现。卡片阴影 blur26、offset0/8、rgba55/83/45/.055；不参与布局占位。宠物花园背景通过真实SVG加载。QSS只用Qt支持的选择器、qlineargradient和属性。

## 复用与命名

创建公共组件时传入唯一ID、role与内容，不复制一份修改颜色。动态ID为 tasks.row{taskId}.check/edit/delete/focus，ObjectName将点换下划线。新建/编辑弹窗每次可新实例化同一类，不能为每页另建样式。状态只改变 property(selected/done/error) 并重新polish，不替换整个页面。

资源全部来自 `:/fg/UI_ASSETS/...`。相同图标只用一个英文文件名，颜色变体用明确独立文件（如ic_check_white.svg）。PNG用KeepAspectRatio与SmoothTransformation，禁止setScaledContents拉伸变形；SVG采用QIcon/QSvgRenderer或imageformats插件。下一阶段部署必须包含对应SVG插件。

布局调整与字体差异应更新表中的基线/实测差异，禁止只改代码而让规范失效。页面不能直接调用SQLite，不能重建播放器或FocusEngine。

小按钮必须设置动态属性 size=small，QSS按该属性给出30高、radius9、12px/600。任务标题使用QLabel的13px/700与ElideRight，整行点击负责选择；不另画标题按钮。主按钮图标需要QPainter SourceIn着白色，不能把深绿图标直接叠在绿底上。绘制子部件的精确字体与局部矩形见Qt_UI_Implementation_Blueprint/COMPONENT_CONTRACTS.json。

开发职责与方法签名以FG-API-1.0为准。前5天交付全部真实功能，QSS与布局规范不等于软件功能验收。
