	这里主要用于实现数据结构与算法相关代码，基于设计模式的。
	从线性表，字符串，矩阵，树，图等基本机构有，查找，排序
	等基本算法也会有，同时还会增加数据结构习题中的一些代码。
	才开始，持续更新的，希望能够使用新的方式来实现这些。也
	为以后作为自己的工具使用打下基础。

基本结构介绍
1、	环境介绍
	1）	deepin linux 64位操作系统
	2）	gcc工具链
	3）	dell Inspiron15-3521笔记本
	4）	内存4G, cpu为Intel(R) Core(TM) i3-3217U CPU @ 1.80GHz

2、	其他工具
	在编译的时候用到了comm_src_select.sh脚本，这个脚本在另一个
	工程中可以找到，地址如下:
		https://github.com/sclgcj/shell_tool.git
	这个工程中包含了日常开发中用到的vim 配置，cscope脚本等。建
	议先安装cscope和ctags， 这在使用c 开发的时候很有用。

3、	目录结构介绍
	build_conf				用于配置需要编译的c文件以及头文件
	include					用于存放公用的头文件
	introduction			数据结构教材绪论
	linear_table			线性表
	test					单元测试代码
