# GitHub资料分发与协作说明

最简单的做法：一个仓库、一套FocusGarden资料、一份Qt_UI_Starter工程。组员每人克隆同一仓库一次；队长只发“仓库地址＋角色目录”，不发四份大素材包。角色小包/Word可单独领取，但共享资料以仓库同一main版本为准。

## 1 哪些上传 哪些不上传

| 内容 | 操作 |
|---|---|
| 00～08中文目录 | 提交；含任务书/指令/清单，方便每人找入口 |
| FocusGarden/ | 提交唯一一套规范、素材、原型和冻结接口，不复制到各人目录 |
| Qt_UI_Starter/ | A下一次Codex建立后提交CMake、src、forms、tests、deployment说明 |
| 本地归档/ | 不提交；旧上传原文只留队长本地追溯，不能作为AI最新接口输入 |
| build*/、CMake缓存、IDE用户设置、运行.sqlite/.db/app.ini、发布二进制目录 | 不提交到日常源码分支；按忽略模板处理 |
| 用户歌曲/账号/凭据 | 不提交；测试用自己的合法本地文件，记录测试结果即可 |

大量图片只有FocusGarden/UI_ASSETS一份，reference是已有原图册截图，不再在角色目录复制。Word内图像属于文档内容；四人引用同一资源路径。日常发给AI的是克隆的仓库，不是把图片反复上传进四个临时工程。

## 2 队长首次放进仓库

若还没有仓库，先在GitHub网页登录你的账号，右上角“+”选择New repository，名称填FocusGarden，建议选Private，勾选Add a README file，点Create repository。打开仓库Settings → Collaborators（或Collaborators and teams），点Add people，输入B/C/D的GitHub用户名并邀请，提醒三人接受邀请。复制仓库绿色Code中的HTTPS地址，再按下面步骤克隆。若已经有共享仓库，直接用它。安装并登录GitHub Desktop，选择File → Clone repository，选该仓库或粘贴HTTPS地址，Local Path可用C:/Projects/FocusGarden，点Clone。打开得到的仓库文件夹。

把本工程资料版解压后的00～08目录和FocusGarden目录复制到这个仓库根目录，不复制“本地归档”。复制的是里面内容，别再套两层FocusGarden_四人开发启动包。A按第1天手册让Codex在同一根目录创建Qt_UI_Starter，不覆盖已有非本项目文件。

回GitHub Desktop看Changes，确认没有数据库/歌曲/凭据/构建产物。填写Summary“docs: freeze FG-API-1.0 and startup tasks”，点Commit to main，再Push origin。A建立并实际编译公共壳后，再提交“bootstrap: shared Qt foundation”，把这个提交SHA写到ENVIRONMENT_LOCK。现有仓库若有代码，不强推覆盖；先用A分支审查合并。

## 3 组员领取

确认自己接受协作者邀请并能访问仓库。GitHub Desktop File → Clone repository，选择同一仓库克隆；Current Branch切到main，点Fetch origin，有Pull origin就点它。先用Qt Creator打开Qt_UI_Starter/CMakeLists.txt并按共同Kit构建原基线；缺公共文件报A，不另建项目。

A：读02目录；B：03；C：04；D：05。Codex打开整个仓库根目录，复制本人“给Codex的完整指令.md”；AI即可按精确路径读取共享资料和实际工程。只有任务书不等于拥有公共源码，A未发基线不能自造另一套接口。

## 4 分支和小提交

main是全组可构建版本。建议feature/a-shell、feature/b-task-crud、feature/b-focus、feature/b-statistics、feature/c-rewards、feature/c-pet、feature/d-music、feature/d-settings；修Bug用fix/成员-问题。不要各维护永久“完整工程”目录。

Desktop先main→Fetch/Pull，再Current Branch→New Branch，填名字点Create Branch。让Codex只改允许文件。每做一个能编译的小功能，在Changes检查差异，写Summary，Commit，再Publish branch（首次）或Push origin。日1就开Draft PR，后续可继续推送同一小任务；不能等五天写完才第一次合并。

## 5 发起合并请求和检查

在Desktop点Preview Pull Request，确认base为main、compare为本人分支，再Create Pull Request进入GitHub。标题写结果，如“B: persist task create and edit”；正文按PR模板写需求ID、文件、接口版本、实际命令/退出码、截图路径、未完成项。Requested reviewer选A。

A拉取这个分支实际构建并看功能证据，确认没改别人文件或新增第二套素材，再合并到main。A自己的公共接口/基础工程PR由B复核后A合并，避免没人检查；不要要求A在GitHub给自己的PR点Approve。若仓库可用保护规则，可要求PR后合并；否则按这套人工流程执行，组员不直接推main。

## 6 出现冲突怎么办

先保存/提交自己的工作，不点“全用我的”覆盖公共文件。Desktop在本分支用Branch → Update from main（或等价合并main），出现冲突记下文件。自己模块的冲突可让Codex逐处解释后合并，再构建；DomainTypes、ServicePorts、CMake、QSS/qrc等公共冲突交A统一处理。解决所有冲突、构建通过后提交并Push，再复查PR。不强推覆盖main。

## 7 公共接口更新

提议人先写受影响签名、原因、调用方、数据库影响及迁移办法；A更新技术契约、API_REGISTRY和对应公共头文件，B复核，版本/哈希随同一个PR提交。A在项目现有群里手动发“FG-API版本/主分支SHA/受影响人/需重编译文件”；本资料不会替你发送消息。

其他成员保存工作后main Fetch/Pull，再更新自己的分支并重新构建。任务书完整范围不需要每天重发；只同步公共变更和验收问题。接口版本变化前不得各自加“兼容版Task”或换返回类型。

## 8 官方操作参考

以下为本轮核对的官方说明；界面语言不同可能显示英文或中文等价项。

- GitHub Desktop克隆：https://docs.github.com/en/desktop/adding-and-cloning-repositories/cloning-and-forking-repositories-from-github-desktop
- 管理分支：https://docs.github.com/en/desktop/making-changes-in-a-branch/managing-branches-in-github-desktop
- 创建PR：https://docs.github.com/en/desktop/working-with-your-remote-repository-on-github-or-github-enterprise/creating-an-issue-or-pull-request-from-github-desktop
- 合并冲突：https://docs.github.com/en/pull-requests/reference/merge-conflicts

- 创建仓库：https://docs.github.com/en/repositories/creating-and-managing-repositories/creating-a-new-repository
- 邀请协作者：https://docs.github.com/en/repositories/managing-your-repositorys-settings-and-features/repository-access-and-collaboration/inviting-collaborators-to-a-personal-repository
