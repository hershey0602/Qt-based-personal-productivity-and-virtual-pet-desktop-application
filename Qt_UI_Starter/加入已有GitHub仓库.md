# 把整体框架加入已有GitHub仓库

你的 GitHub 已有 00～08 和 FocusGarden，现在只追加 Qt_UI_Starter 源码和两份仓库规则文件。使用已有仓库，不再创建新仓库。

## 直接用网页上传

如果你之前就是在网页上传 00～08，可以继续用同样的方式：

1. 打开你现有 GitHub 仓库的 Code 页面，停在能看到 00～08 和 FocusGarden 的根目录。
2. 点击 Add file → Upload files。
3. 在电脑上打开 `整体框架/01_GitHub`，把里面的 `Qt_UI_Starter` 文件夹、`.gitignore`、`.gitattributes` 三项拖入网页上传区域。
4. 检查上传列表出现的是 `Qt_UI_Starter/src/...`，不是 `整体框架/01_GitHub/...`。
5. 提交说明填 `feat: add shared Qt page framework`，选择创建 `feature/a-shell` 新分支，点击 Propose changes。随后 Create pull request，确认后 Merge pull request，将新增内容合并到 main。
6. 回到 main 的 Code 页面，确认 Qt_UI_Starter 与 FocusGarden 同级。队员更新 main 后就能打开新工程。

如果仓库已有同名 .gitignore 或 .gitattributes，先保留其已有规则，再合入这次新增规则。两种上传方式任选一种即可。

下面是适合后续持续协作的 GitHub Desktop 操作。

## 第一步 打开已有仓库的本地副本

打开 GitHub Desktop，在左上角 Current repository 选择你现有的拾光岛仓库。

如果列表中还没有它，选择 File → Clone repository → URL，粘贴现有仓库的 HTTPS 地址。Local path 建议选 `C:/Projects/FocusGarden`。点击 Clone。

这里克隆的是你 GitHub 上现有的仓库；桌面启动包上次生成的本地 Git 历史已归档，不需要导入。

## 第二步 先同步线上内容

在 GitHub Desktop 选择 main 分支，点击 Fetch origin。如果出现 Pull origin，再点击它。然后选择 Repository → Show in Explorer，打开该仓库真正的本地文件夹。

打开后应能看到原有 00～08、FocusGarden 和 README.md。后面的复制都放在这个位置。

## 第三步 复制新增文件

打开桌面启动包里的 `整体框架/01_GitHub`。把里面的 `Qt_UI_Starter`、`.gitignore`、`.gitattributes` 复制到第二步打开的仓库根目录。

完成后 `Qt_UI_Starter` 与 `FocusGarden` 应是同一层级。这样 CMake 才能直接引用已有共享素材。

若已有 `.gitignore` 或 `.gitattributes`，将这次文件里的规则追加合并到原文件中，保留原仓库已有规则。如果团队已经新增了 Qt_UI_Starter，请先保留队员改动，在分支中合并这次框架内容。

## 第四步 提交新增内容

回到 GitHub Desktop，Changes 中应主要出现 `Qt_UI_Starter/...`、`.gitignore`、`.gitattributes`。00～08 和 FocusGarden 不需要重复上传。

Summary 填写：

```text
feat: add shared Qt page framework
```

点击 Commit to main，然后点击 Push origin。你的仓库已有远程地址，这里是 Push origin，不是 Publish repository。若团队使用保护分支，就先 New Branch 创建 `feature/a-shell`，提交并 Publish branch，再创建合并到 main 的 Pull Request。

## 第五步 检查网页并通知队员

选择 Repository → View on GitHub。确认根目录已出现 Qt_UI_Starter，打开后能看到 CMakeLists.txt、src、tests 和 README.md。

在 Desktop 的 History 中找到刚才提交，右键 Copy SHA。这个 SHA 是已有仓库里的新基线；将它填写到 ENVIRONMENT_LOCK，并与仓库地址一起交给 B/C/D。

队员 Fetch/Pull 后，从原仓库直接打开 `Qt_UI_Starter/CMakeLists.txt`。不需要各建一个工程，也不需要重新领取 00～08。

## 操作依据

[GitHub Desktop 克隆已有仓库](https://docs.github.com/en/desktop/adding-and-cloning-repositories/cloning-and-forking-repositories-from-github-desktop)

[GitHub 网页上传文件和文件夹](https://docs.github.com/en/repositories/working-with-files/managing-files/adding-a-file-to-a-repository)

[GitHub Desktop 同步远程仓库](https://docs.github.com/en/desktop/working-with-your-remote-repository-on-github-or-github-enterprise/syncing-your-branch-in-github-desktop)
