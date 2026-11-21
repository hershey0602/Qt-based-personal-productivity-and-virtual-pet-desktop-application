# 页面控件树和布局契约

根结构：QMainWindow → centralWidget → 外边距18/10 → AppShell的QHBoxLayout → Sidebar固定218/188 + MainColumn伸展1 → TopHeader固定74 + QStackedWidget伸展1。Stack中每页只有一份QScrollArea。

下面树和 layouts/*.json 逐ID对应。所有坐标是客户区逻辑像素；正常列表按数据重复行，动态容器从服务快照填充。

## tasks

| ID | Qt 类型 | 父ID | 布局与约束 |
|---|---|---|---|
|tasks|QFrame + QVBoxLayout|pageScroll.viewport|padding=0, gap=16|
|tasks.title|QFrame + QHBoxLayout|tasks|h=59, gap=14, compactH=59|
|tasks.heading|QFrame + QVBoxLayout|tasks.title|gap=3, stretch=1|
|tasks.titleText|QLabel|tasks.heading||
|tasks.subtitle|QLabel|tasks.heading||
|tasks.add|QPushButton|tasks.title||
|tasks.metrics|QFrame + QHBoxLayout|tasks|h=105|
|tasks.metric1|QFrame + QHBoxLayout|tasks.metrics|h=105, padding=19, gap=14, stretch=1|
|tasks.metric1.icon|QLabel + QPixmap|tasks.metric1|w=49, h=49|
|tasks.metric1.text|QFrame + QVBoxLayout|tasks.metric1|gap=4, stretch=1|
|tasks.metric1.label|QLabel|tasks.metric1.text||
|tasks.metric1.value|QLabel|tasks.metric1.text||
|tasks.metric2|QFrame + QHBoxLayout|tasks.metrics|h=105, padding=19, gap=14, stretch=1|
|tasks.metric2.icon|QLabel + QPixmap|tasks.metric2|w=49, h=49|
|tasks.metric2.text|QFrame + QVBoxLayout|tasks.metric2|gap=4, stretch=1|
|tasks.metric2.label|QLabel|tasks.metric2.text||
|tasks.metric2.value|QLabel|tasks.metric2.text||
|tasks.metric3|QFrame + QHBoxLayout|tasks.metrics|h=105, padding=19, gap=14, stretch=1|
|tasks.metric3.icon|QLabel + QPixmap|tasks.metric3|w=49, h=49|
|tasks.metric3.text|QFrame + QVBoxLayout|tasks.metric3|gap=4, stretch=1|
|tasks.metric3.label|QLabel|tasks.metric3.text||
|tasks.metric3.value|QLabel|tasks.metric3.text||
|tasks.filters|QFrame + QHBoxLayout|tasks|h=40, gap=10|
|tasks.search|QLineEdit|tasks.filters|stretch=15|
|tasks.category|QComboBox|tasks.filters|stretch=8|
|tasks.priority|QComboBox|tasks.filters|stretch=8|
|tasks.status|QComboBox|tasks.filters|stretch=8|
|tasks.clear|QPushButton|tasks.filters||
|tasks.body|QFrame + QHBoxLayout|tasks||
|tasks.list|QFrame + QVBoxLayout|tasks.body|h=449, padding=19, gap=15, stretch=150|
|tasks.list.heading|QLabel|tasks.list||
|tasks.rows|QFrame + QVBoxLayout|tasks.list|stretch=1, 动态=tasks|
|tasks.detail|QFrame + QVBoxLayout|tasks.body|h=449, padding=19, gap=15, stretch=85|
|tasks.detail.heading|QLabel|tasks.detail||
|tasks.readonly|QLabel|tasks.detail||
|tasks.fields|QFrame + QVBoxLayout|tasks.detail|gap=4, stretch=1, 动态=detail|

## dashboard

| ID | Qt 类型 | 父ID | 布局与约束 |
|---|---|---|---|
|dashboard|QFrame + QVBoxLayout|pageScroll.viewport|padding=0, gap=16|
|dashboard.title|QFrame + QHBoxLayout|dashboard|h=59, gap=14, compactH=59|
|dashboard.heading|QFrame + QVBoxLayout|dashboard.title|gap=3, stretch=1|
|dashboard.titleText|QLabel|dashboard.heading||
|dashboard.subtitle|QLabel|dashboard.heading||
|dashboard.banner|QFrame + QHBoxLayout|dashboard|h=210, padding=28, compactH=176|
|dashboard.welcome|QFrame + QVBoxLayout|dashboard.banner|gap=10, stretch=1|
|dashboard.greeting|QLabel|dashboard.welcome||
|dashboard.message|QLabel|dashboard.welcome||
|dashboard.newTask|QPushButton|dashboard.welcome|maxW=142|
|dashboard.art|QLabel + QPixmap|dashboard.banner|w=430, h=160|
|dashboard.metrics|QFrame + QHBoxLayout|dashboard|h=105|
|dashboard.metric1|QFrame + QHBoxLayout|dashboard.metrics|h=105, padding=19, gap=14, stretch=1|
|dashboard.metric1.icon|QLabel + QPixmap|dashboard.metric1|w=49, h=49|
|dashboard.metric1.text|QFrame + QVBoxLayout|dashboard.metric1|gap=4, stretch=1|
|dashboard.metric1.label|QLabel|dashboard.metric1.text||
|dashboard.metric1.value|QLabel|dashboard.metric1.text||
|dashboard.metric2|QFrame + QHBoxLayout|dashboard.metrics|h=105, padding=19, gap=14, stretch=1|
|dashboard.metric2.icon|QLabel + QPixmap|dashboard.metric2|w=49, h=49|
|dashboard.metric2.text|QFrame + QVBoxLayout|dashboard.metric2|gap=4, stretch=1|
|dashboard.metric2.label|QLabel|dashboard.metric2.text||
|dashboard.metric2.value|QLabel|dashboard.metric2.text||
|dashboard.metric3|QFrame + QHBoxLayout|dashboard.metrics|h=105, padding=19, gap=14, stretch=1|
|dashboard.metric3.icon|QLabel + QPixmap|dashboard.metric3|w=49, h=49|
|dashboard.metric3.text|QFrame + QVBoxLayout|dashboard.metric3|gap=4, stretch=1|
|dashboard.metric3.label|QLabel|dashboard.metric3.text||
|dashboard.metric3.value|QLabel|dashboard.metric3.text||
|dashboard.mid|QFrame + QHBoxLayout|dashboard||
|dashboard.todayTasks|QFrame + QVBoxLayout|dashboard.mid|h=220, padding=19, gap=15, stretch=108|
|dashboard.todayTasks.heading|QLabel|dashboard.todayTasks||
|dashboard.todayList|QFrame + QVBoxLayout|dashboard.todayTasks|stretch=1, 动态=today|
|dashboard.allTasks|QPushButton|dashboard.todayTasks||
|dashboard.weekChart|QFrame + QVBoxLayout|dashboard.mid|h=220, padding=19, gap=15, stretch=92|
|dashboard.weekChart.heading|QLabel|dashboard.weekChart||
|dashboard.chart|QWidget + QPainter|dashboard.weekChart|h=135|
|dashboard.allStats|QPushButton|dashboard.weekChart||
|dashboard.bottom|QFrame + QHBoxLayout|dashboard||
|dashboard.pet|QFrame + QHBoxLayout|dashboard.bottom|h=195, padding=19, stretch=108|
|dashboard.petCopy|QFrame + QVBoxLayout|dashboard.pet|stretch=1|
|dashboard.petHeading|QLabel|dashboard.petCopy||
|dashboard.petStatus|QLabel|dashboard.petCopy||
|dashboard.petCoins|QLabel|dashboard.petCopy||
|dashboard.feed|QPushButton|dashboard.petCopy|maxW=130|
|dashboard.petArt|QLabel + QPixmap|dashboard.pet|w=160, h=155|
|dashboard.quick|QFrame + QVBoxLayout|dashboard.bottom|h=195, padding=19, gap=15, stretch=92|
|dashboard.quick.heading|QLabel|dashboard.quick||
|dashboard.shortcuts|QFrame + QHBoxLayout|dashboard.quick||
|dashboard.quick25|QPushButton|dashboard.shortcuts||
|dashboard.quick50|QPushButton|dashboard.shortcuts||
|dashboard.music|QPushButton|dashboard.shortcuts||
|dashboard.quickHint|QLabel|dashboard.quick||

## music

| ID | Qt 类型 | 父ID | 布局与约束 |
|---|---|---|---|
|music|QFrame + QVBoxLayout|pageScroll.viewport|padding=0, gap=16|
|music.title|QFrame + QHBoxLayout|music|h=59, gap=14, compactH=59|
|music.heading|QFrame + QVBoxLayout|music.title|gap=3, stretch=1|
|music.titleText|QLabel|music.heading||
|music.subtitle|QLabel|music.heading||
|music.add|QPushButton|music.title||
|music.body|QFrame + QHBoxLayout|music||
|music.playlist|QFrame + QVBoxLayout|music.body|h=580, padding=19, gap=15, stretch=86|
|music.playlist.heading|QLabel|music.playlist||
|music.list|QListView/QListWidget|music.playlist|minH=310, stretch=1|
|music.empty|QLabel|music.playlist||
|music.import|QPushButton|music.playlist||
|music.remove|QPushButton|music.playlist||
|music.right|QFrame + QVBoxLayout|music.body|stretch=114|
|music.player|QFrame + QVBoxLayout|music.right|h=450, padding=19, gap=7|
|music.player.heading|QLabel|music.player||
|music.album|QLabel + QPixmap|music.player|w=180, h=180|
|music.track|QLabel|music.player|h=30|
|music.trackHint|QLabel|music.player|h=20|
|music.seek|QSlider|music.player||
|music.timeRow|QFrame + QHBoxLayout|music.player|h=20|
|music.position|QLabel|music.timeRow|stretch=1|
|music.duration|QLabel|music.timeRow||
|music.controls|QFrame + QHBoxLayout|music.player||
|music.previous|QPushButton|music.controls||
|music.play|QPushButton|music.controls||
|music.stop|QPushButton|music.controls||
|music.next|QPushButton|music.controls||
|music.volume|QFrame + QVBoxLayout|music.right|h=114, padding=19, gap=4|
|music.volume.heading|QLabel|music.volume||
|music.volumeSlider|QSlider|music.volume||
|music.volumeHint|QLabel|music.volume||

## pet

| ID | Qt 类型 | 父ID | 布局与约束 |
|---|---|---|---|
|pet|QFrame + QVBoxLayout|pageScroll.viewport|padding=0, gap=16|
|pet.title|QFrame + QHBoxLayout|pet|h=59, gap=14, compactH=59|
|pet.heading|QFrame + QVBoxLayout|pet.title|gap=3, stretch=1|
|pet.titleText|QLabel|pet.heading||
|pet.subtitle|QLabel|pet.heading||
|pet.body|QFrame + QHBoxLayout|pet||
|pet.hero|QFrame + QHBoxLayout|pet.body|h=360, padding=25, stretch=130|
|pet.info|QFrame + QVBoxLayout|pet.hero|stretch=1|
|pet.unique|QLabel|pet.info||
|pet.name|QLabel|pet.info||
|pet.levelTag|QLabel|pet.info||
|pet.mood|QLabel|pet.info||
|pet.coins|QLabel|pet.info||
|pet.art|QLabel + QPixmap|pet.hero|w=340, h=315|
|pet.feed|QFrame + QVBoxLayout|pet.body|h=429, padding=19, gap=15, stretch=70|
|pet.feed.heading|QLabel|pet.feed||
|pet.feedTitle|QLabel|pet.feed||
|pet.feedHint|QLabel|pet.feed||
|pet.carrot|QLabel + QPixmap|pet.feed|w=110, h=120|
|pet.feedButton|QPushButton|pet.feed||
|pet.feedFootnote|QLabel|pet.feed||
|pet.metrics|QFrame + QHBoxLayout|pet|h=105|
|pet.metric1|QFrame + QHBoxLayout|pet.metrics|h=105, padding=19, gap=14, stretch=1|
|pet.metric1.icon|QLabel + QPixmap|pet.metric1|w=49, h=49|
|pet.metric1.text|QFrame + QVBoxLayout|pet.metric1|gap=4, stretch=1|
|pet.metric1.label|QLabel|pet.metric1.text||
|pet.metric1.value|QLabel|pet.metric1.text||
|pet.metric2|QFrame + QHBoxLayout|pet.metrics|h=105, padding=19, gap=14, stretch=1|
|pet.metric2.icon|QLabel + QPixmap|pet.metric2|w=49, h=49|
|pet.metric2.text|QFrame + QVBoxLayout|pet.metric2|gap=4, stretch=1|
|pet.metric2.label|QLabel|pet.metric2.text||
|pet.metric2.value|QLabel|pet.metric2.text||
|pet.metric3|QFrame + QHBoxLayout|pet.metrics|h=105, padding=19, gap=14, stretch=1|
|pet.metric3.icon|QLabel + QPixmap|pet.metric3|w=49, h=49|
|pet.metric3.text|QFrame + QVBoxLayout|pet.metric3|gap=4, stretch=1|
|pet.metric3.label|QLabel|pet.metric3.text||
|pet.metric3.value|QLabel|pet.metric3.text||
|pet.experience|QFrame + QVBoxLayout|pet|h=124, padding=19, gap=15|
|pet.experience.heading|QLabel|pet.experience||
|pet.expText|QLabel|pet.experience||
|pet.progress|QProgressBar|pet.experience|h=11|

## statistics

| ID | Qt 类型 | 父ID | 布局与约束 |
|---|---|---|---|
|statistics|QFrame + QVBoxLayout|pageScroll.viewport|padding=0, gap=16|
|statistics.title|QFrame + QHBoxLayout|statistics|h=59, gap=14, compactH=59|
|statistics.heading|QFrame + QVBoxLayout|statistics.title|gap=3, stretch=1|
|statistics.titleText|QLabel|statistics.heading||
|statistics.subtitle|QLabel|statistics.heading||
|statistics.metrics|QFrame + QHBoxLayout|statistics|h=105|
|statistics.metric1|QFrame + QHBoxLayout|statistics.metrics|h=105, padding=19, gap=14, stretch=1|
|statistics.metric1.icon|QLabel + QPixmap|statistics.metric1|w=49, h=49|
|statistics.metric1.text|QFrame + QVBoxLayout|statistics.metric1|gap=4, stretch=1|
|statistics.metric1.label|QLabel|statistics.metric1.text||
|statistics.metric1.value|QLabel|statistics.metric1.text||
|statistics.metric2|QFrame + QHBoxLayout|statistics.metrics|h=105, padding=19, gap=14, stretch=1|
|statistics.metric2.icon|QLabel + QPixmap|statistics.metric2|w=49, h=49|
|statistics.metric2.text|QFrame + QVBoxLayout|statistics.metric2|gap=4, stretch=1|
|statistics.metric2.label|QLabel|statistics.metric2.text||
|statistics.metric2.value|QLabel|statistics.metric2.text||
|statistics.metric3|QFrame + QHBoxLayout|statistics.metrics|h=105, padding=19, gap=14, stretch=1|
|statistics.metric3.icon|QLabel + QPixmap|statistics.metric3|w=49, h=49|
|statistics.metric3.text|QFrame + QVBoxLayout|statistics.metric3|gap=4, stretch=1|
|statistics.metric3.label|QLabel|statistics.metric3.text||
|statistics.metric3.value|QLabel|statistics.metric3.text||
|statistics.metric4|QFrame + QHBoxLayout|statistics.metrics|h=105, padding=19, gap=14, stretch=1|
|statistics.metric4.icon|QLabel + QPixmap|statistics.metric4|w=49, h=49|
|statistics.metric4.text|QFrame + QVBoxLayout|statistics.metric4|gap=4, stretch=1|
|statistics.metric4.label|QLabel|statistics.metric4.text||
|statistics.metric4.value|QLabel|statistics.metric4.text||
|statistics.middle|QFrame + QHBoxLayout|statistics||
|statistics.chart|QFrame + QVBoxLayout|statistics.middle|h=301, padding=19, gap=15, stretch=130|
|statistics.chart.heading|QLabel|statistics.chart||
|statistics.bars|QWidget + QPainter|statistics.chart|h=225|
|statistics.completion|QFrame + QVBoxLayout|statistics.middle|h=301, padding=19, gap=15, stretch=70|
|statistics.completion.heading|QLabel|statistics.completion||
|statistics.donutRow|QFrame + QHBoxLayout|statistics.completion|stretch=1|
|statistics.donut|QWidget + QPainter|statistics.donutRow|w=148, h=148|
|statistics.ratioText|QLabel|statistics.donutRow|stretch=1|
|statistics.history|QFrame + QVBoxLayout|statistics|minH=155, padding=19, gap=15|
|statistics.history.heading|QLabel|statistics.history||
|statistics.table|QTableView/QTableWidget|statistics.history|minH=90|

## settings

| ID | Qt 类型 | 父ID | 布局与约束 |
|---|---|---|---|
|settings|QFrame + QVBoxLayout|pageScroll.viewport|padding=0, gap=16|
|settings.title|QFrame + QHBoxLayout|settings|h=59, gap=14, compactH=59|
|settings.heading|QFrame + QVBoxLayout|settings.title|gap=3, stretch=1|
|settings.titleText|QLabel|settings.heading||
|settings.subtitle|QLabel|settings.heading||
|settings.body|QFrame + QHBoxLayout|settings||
|settings.focus|QFrame + QVBoxLayout|settings.body|h=387, padding=19, gap=15, stretch=1|
|settings.focus.heading|QLabel|settings.focus||
|settings.focusRow|QFrame + QHBoxLayout|settings.focus|minH=85|
|settings.focusCopy|QFrame + QVBoxLayout|settings.focusRow|stretch=1|
|settings.focusTitle|QLabel|settings.focusCopy||
|settings.focusHint|QLabel|settings.focusCopy||
|settings.focusMinutes|QComboBox|settings.focusRow|w=142|
|settings.restRow|QFrame + QHBoxLayout|settings.focus|minH=85|
|settings.restCopy|QFrame + QVBoxLayout|settings.restRow|stretch=1|
|settings.restTitle|QLabel|settings.restCopy||
|settings.restHint|QLabel|settings.restCopy||
|settings.restMinutes|QComboBox|settings.restRow|w=142|
|settings.soundRow|QFrame + QHBoxLayout|settings.focus|minH=85|
|settings.soundCopy|QFrame + QVBoxLayout|settings.soundRow|stretch=1|
|settings.soundTitle|QLabel|settings.soundCopy||
|settings.soundHint|QLabel|settings.soundCopy||
|settings.sound|QCheckBox|settings.soundRow||
|settings.data|QFrame + QVBoxLayout|settings.body|h=387, padding=19, gap=15, stretch=1|
|settings.data.heading|QLabel|settings.data||
|settings.dataTitle|QLabel|settings.data||
|settings.dataHint|QLabel|settings.data||
|settings.about|QLabel|settings.data||
|settings.appName|QLabel|settings.data||
|settings.version|QLabel|settings.data||
|settings.team|QLabel|settings.data||

## focus

| ID | Qt 类型 | 父ID | 布局与约束 |
|---|---|---|---|
|focus|QFrame + QVBoxLayout|pageScroll.viewport|padding=0, gap=16|
|focus.title|QFrame + QHBoxLayout|focus|h=59, gap=14, compactH=59|
|focus.heading|QFrame + QVBoxLayout|focus.title|gap=3, stretch=1|
|focus.titleText|QLabel|focus.heading||
|focus.subtitle|QLabel|focus.heading||
|focus.body|QFrame + QHBoxLayout|focus||
|focus.timerPanel|QFrame + QVBoxLayout|focus.body|minH=515, padding=19, gap=12, stretch=135|
|focus.selectRow|QFrame + QHBoxLayout|focus.timerPanel||
|focus.taskField|QFrame + QVBoxLayout|focus.selectRow|maxW=440, gap=6, stretch=1|
|focus.taskLabel|QLabel|focus.taskField||
|focus.task|QComboBox|focus.taskField||
|focus.state|QLabel|focus.selectRow||
|focus.mode|QFrame + QHBoxLayout|focus.timerPanel|h=40|
|focus.mode25|QPushButton|focus.mode||
|focus.mode50|QPushButton|focus.mode||
|focus.custom|QPushButton|focus.mode||
|focus.rest|QPushButton|focus.mode||
|focus.customMinutes|QComboBox|focus.timerPanel|maxW=120, 按状态显隐|
|focus.ring|QWidget + QPainter|focus.timerPanel|w=265, h=265|
|focus.controls|QFrame + QHBoxLayout|focus.timerPanel||
|focus.start|QPushButton|focus.controls||
|focus.pause|QPushButton|focus.controls||
|focus.resume|QPushButton|focus.controls||
|focus.abandon|QPushButton|focus.controls||
|focus.errorRow|QFrame + QHBoxLayout|focus.timerPanel|按状态显隐|
|focus.error|QLabel|focus.errorRow|stretch=1|
|focus.retry|QPushButton|focus.errorRow||
|focus.hint|QLabel|focus.timerPanel||
|focus.aside|QFrame + QVBoxLayout|focus.body|stretch=65|
|focus.todayHistory|QFrame + QVBoxLayout|focus.aside|minH=126, padding=19, gap=15|
|focus.todayHistory.heading|QLabel|focus.todayHistory||
|focus.historyRows|QFrame + QVBoxLayout|focus.todayHistory|动态=focusHistory|
|focus.viewHistory|QPushButton|focus.todayHistory||
|focus.reward|QFrame + QVBoxLayout|focus.aside|minH=331, padding=19, gap=15|
|focus.reward.heading|QLabel|focus.reward||
|focus.rewardValue|QLabel|focus.reward||
|focus.rewardHint|QLabel|focus.reward||
|focus.cheer|QLabel|focus.reward||
|focus.petArt|QLabel + QPixmap|focus.reward|w=180, h=122|

## 三尺寸适配

| 参数 | 1600×900 | 1366×768 | 1280×720 |
|---|---|---|---|
| 外边距 | 18 | 10 | 10 |
| 侧栏 | 218 | 188 | 188 |
| Header | 74 | 74 | 74 |
| 页面边距 | 24 | 18 | 18 |
| 卡片边距 | 19 | 15 | 15 |
| 主间距 | 16 | 12 | 12 |
| 导航高度 | 48 | 43 | 43 |
| 圆环 | 265 | 265 | 265 |

七页保持原双列关系。Dashboard中部108:92；Tasks列表:详情150:85；Focus主:侧135:65；Pet/Statistics为130:70；Music为86:114；Settings为1:1。宽度由可用空间扣去gap后按stretch分配。最大内容宽1320；大于1600仍居中，不无限拉伸。

当内容高度超过viewport时只纵向滚动；Task列表、长详情、Music列表和历史表独立处理长数据。小窗口不能把按钮挤出父容器。1280×720为最低客户区尺寸，低于该尺寸由窗口minimumSize限制。滚动条占位8px，参数差异表保留与原浏览器截图的占位差别。
