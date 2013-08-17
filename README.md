SuperCx_HMI
===========

This is SuperCx SCADA/HMI Software's HMI.


# 环境要求
1. VC6 + sp5以上。
1. Windwos 2000以上。

# 构建过程
1. 使用Git客户端下载代码到d:\SuperCx_HMI文件夹。
1. 在VC6中菜单Tools|Options|Directory中设置C++目录如下：
   * D:\SuperCx_HMI\LIB\WTL\INCLUDE
   * D:\SuperCx_HMI\LIB\GDIPLUS
   * D:\SuperCx_HMI\LIB\HTMLHELP
1. 在d:\SuperCx_HMI\bld文件夹中，以管理员权限运行批处理BuildUI.bat，自动完成编译，输出为d:\SuperCx_HMI\bin文件夹。 