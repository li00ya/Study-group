吃鸡小分队学习小组(git手册)
====
Study group of chicken dinner team(Git operation manual)
----- 

**第一次同步，需要克隆远程git仓库**

步骤: 

**进入工作目录，然后使用克隆命令**

```
克隆master分支
git clone https://github.com/xxx/仓库.git  
克隆liya分支
git clone -b liya https://github.com/xxx/仓库.git
```

执行完上面的命令后，就会在当前目录下生成一个仓库文件夹，然后再里面进行操作就行  
然后，进入仓库文件夹下，创建自己的分支

```
git checkout -b name  
创建自己的分支
```

**正常使用**

*一般是复制自己的分支到其他文件夹下（非克隆文件夹），然后在这里面对文件进行修改、添加，想要提交代码时,进入克隆的文件夹下进行操作*


**1、更新远程最新代码**

切换到master分支，然后把远程仓库中的master分支同步到本地仓库  
```
git branch  
查看分支
  
git checkout master  
切换到分支名为主分支上  

git pull origin master  
同步远程仓库  
```


**2、合并最新代码到自己分支**

```
git checkout name  
切换到name分支  

git merge master  
合并主分支最新代码到当前分支  
```
*如果有冲突，修改冲突。修改冲突，尽量以master分支为准。若冲突确实是由自己修改的，则以自己修改后的代码为主.*


**3、合并修改代码**

*使用beyond compare或者其他比较工具，将自己工作目录的修改同步到git仓库*


**4、代码合并完成后，将git仓库代码提交到版本库**

```
git status  
查看当前分支下，所有文件的改动状态  

git add -A  
添加git仓库下所有的改动文件到缓冲区  

git commit -m "add the note about modify"  
提交代码到本地仓库，双引号里面的内容应是本次的修改说明 

修改git默认编辑器 Nano为 Vim
git config --global core.editor vim

git --amend 追加提交(被追加的提交没有被merge)

git push origin name  
推送自己的分支到远程仓库  
```

在commit的时候，附加信息要尽量如实反映本次提交的修改。  
*提交远程仓库时，若遇到 ‘non-fast-forward’ 错误提示，且确认自己本地代码为最新代码时，可使用-f参数强行使本地仓库覆盖远端要同步的分支*
`git push -f origin name`



**5、合并自己分支到主分支**

*登录github网页，在本项目仓库主页，点击branchs，然后点击右侧的New pull request请求，依次填写,然后点击Create pull request，然后等待分支合并即可.*


**其他使用技巧**

*更改远程仓库分支名*
```
更改本地仓库分支名
git branch -m old-name new-name

删除远程旧分支
git push origin :old-name

推送本地所有分支
git push --all
```

**存储git同步账号密码到本地**

*输入一次账号密码，以后本机同步代码免除输入*
```
git config --global credential.helper store
git push origin 分支名
```
*完成之后，该命令会在本地当前用户宿主目录(`~/.git-credential`)文件里存储账号密码*

**修改git配置，同时将代码同步到github和gitee上**

**方法一:(一次push， 同时同步到多个远程代码仓库)**

打开.git/config，在[remote "origin"]下面的url下面，再添加gitee的仓库网址,
如下：
```
[remote "origin"]
	url = https://github.com/xxx.git
	url = https://gitee.com/xxx.git
	fetch = +refs/heads/*:refs/remotes/origin/*
```
push时，会同时推送到两个仓库


**方法二:(按名称推送)**

打开.git/config，将`[remote "origin"]`改为`[remote "github"]`，然后再添加gitee仓库的配置(url和fetch)，改为`[remote "gitee"]`,
如下：
```
[remote "github"]
	url = https://github.com/xxx.git
	fetch = +refs/heads/*:refs/remotes/origin/*
[remote "gitee"]
	url = https://gitee.com/xxx.git
	fetch = +refs/heads/*:refs/remotes/origin/*
```
push时，选择git push github 分支名，或者git push gitee 分支名
