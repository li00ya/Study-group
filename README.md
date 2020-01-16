# Study-group
Study group of chicken dinner team  
吃鸡小分队学习小组  

#Git operation manual  
###第一次同步，需要克隆远程git仓库  
#步骤: 进入工作目录，然后使用克隆命令(后面的网址为我们小分队学习小组git仓库链接)  

git clone https://github.com/li00ya/Study-group.git  

#执行完上面的命令后，就会在当前目录下生成一个Study-group文件夹，然后再里面进行操作就行  
##然后，创建自己的分支,进入项目文件，使用  

git checkout -b name  

#创建自己的分支，一般name是自己名字的拼音  


##正常使用  

##一般是复制自己的分支到其他文件夹下（非克隆文件夹），然后在这里面对文件进行修改、添加，想要提交代码时，  
##进入克隆的文件夹下进行操作  

####
###1、更新远程最新代码  
#切换到master分支，然后把远程仓库中的master分支同步到本地仓库  
git branch  
#查看分支  
git checkout master  
#切换到分支名为主分支上  
git pull origin master  
#同步远程仓库  

####
###2、合并最新代码到自己分支  
#切换到自己的分支，然后合并master分支到当前分支  
git checkout name  
#切换到name分支  
git merge master  
#合并主分支最新代码到当前分支  
#如果有冲突，修改冲突。修改冲突，尽量以master分支为准。若冲突确实是由自己修改的，则以自己修改后的代码为主。  

#####
###3、合并修改代码  
#使用beyond compare或者其他比较工具，将自己工作目录的修改同步到git仓库  

#####
###4、代码合并完成后，将git仓库代码提交到版本库  
git status  
#查看当前分支下，所有文件的改动状态  
git add -A  
#添加git仓库下所有的改动文件到缓冲区  
git commit -m "add the note about modify"  
#提交代码到本地仓库，双引号里面的内容应是本次的修改说明  
git push origin name  
#推送自己的分支到远程仓库  
#在commit的时候，附加信息要尽量如实反映本次提交的修改。  

####
###5、合并自己分支到主分支  
#登录github网页，在本项目仓库主页，点击branchs，然后点击右侧的New pull request请求，依次填写，  
#然后点击Create pull request，然后等待分支合并即可。  
