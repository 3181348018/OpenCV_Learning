# Git学习笔记

## 使用指令式遇到的坑


**1.版本回退报错**
执行`git reset --hard HEAD^`
报错fatal: `ambiguous argument 'HEAD^'`

**解决**:
(1).因为cmd控制台中换行符默认是^，而不是\ ，所以^符号被git编译为换行符了，解决方案：
`git reset --hard "HEAD^"`
`git reset --hard HEAD~[return times]`
(2).该仓库到目前为止只有commit过一次代码，故已经是head版本，也会报这样的错，无需关心直接commit或者rm即可
