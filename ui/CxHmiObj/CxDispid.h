////////////////////////////////////////////////////////////////////////////////////////////////
//CxComDispid.h : Common DISPID define file
//所属项目: AutoCx中的图形对象
//创建日期: 2001年11月
//创 建 人: 卢文江
//功能描述: 图形对象公有属性接口ID集
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CXCOMDISPID_H__
#define __CXCOMDISPID_H__

#define		CXDISPID_MIN						0x80010000
#define		CXDISPID_MAX						0x8001FFFF
#define		CXDISPID_PROP						CXDISPID_MIN //扩展的属性
#define		CXDISPID_METH						(CXDISPID_PROP + 0x4000) //对象属性开始 
#define		CXDISPID_EVEN						(CXDISPID_METH + 0x2000) //对象方法开始
#define		CXDISPID_EXTPROP					(CXDISPID_MIN + 0x8000) //事件开始


//公用属性DISPID
#define		CXPROPID_Name						(CXDISPID_PROP)			//名称  
#define     CXPROPID_FillStyle					(CXDISPID_PROP + 1)			//填充类型        
#define     CXPROPID_HorFillDirection			(CXDISPID_PROP + 2)			//水平填充方向    
#define     CXPROPID_VerFillDirection			(CXDISPID_PROP + 3)			//垂直填充方向    
#define     CXPROPID_VerFillPercent				(CXDISPID_PROP + 4)			//垂直填充度      
#define     CXPROPID_HorFillPercent				(CXDISPID_PROP + 5)			//水平填充度 
#define     CXPROPID_BrushImageFileName			(CXDISPID_PROP + 6)		//位图画刷的位图文件名
#define     CXPROPID_EdgeStyle					(CXDISPID_PROP + 7)			//边界线型        
#define     CXPROPID_EdgeWidth					(CXDISPID_PROP + 8)			//边界线宽        
#define		CXPROPID_EdgeColor					(CXDISPID_PROP + 9)			//边界颜色
#define		CXPROPID_HighLightEdgeColor			(CXDISPID_PROP + 10)			//边界高亮色  
#define		CXPROPID_FillColor					(CXDISPID_PROP + 11)			//填充颜色  
#define		CXPROPID_HighLightFillColor			(CXDISPID_PROP + 12)			//填充高亮色 
#define		CXPROPID_ForeColor					(CXDISPID_PROP + 13)   
#define		CXPROPID_BackColor					(CXDISPID_PROP + 14)			//背景颜色
#define		CXPROPID_BackStyle					(CXDISPID_PROP + 15)			//背景风格
#define		CXPROPID_HorPosition				(CXDISPID_PROP + 16)			//水平位置       
#define		CXPROPID_VerPosition				(CXDISPID_PROP + 17)			//垂直位置       
#define		CXPROPID_RotateAngle				(CXDISPID_PROP + 18)			//旋转角度       
#define		CXPROPID_ScaleBasePoint				(CXDISPID_PROP + 19)			//变换基点
#define		CXPROPID_UniformScale				(CXDISPID_PROP + 20)			//统一缩放
#define		CXPROPID_HorScale					(CXDISPID_PROP + 21)			//水平缩放比例
#define		CXPROPID_VerScale					(CXDISPID_PROP + 22)			//垂直缩放比例
#define		CXPROPID_Height						(CXDISPID_PROP + 23)			//高度(DrawRectangle & DrawRoundRect & DrawEllipse & DrawLine)
#define		CXPROPID_Width						(CXDISPID_PROP + 24)			//宽度(DrawRectangle & DrawRoundRect & DrawEllipse & DrawLine)
#define		CXPROPID_StartX						(CXDISPID_PROP + 25)			//起点横坐标(DrawChord)
#define		CXPROPID_StartY						(CXDISPID_PROP + 26)			//起点纵坐标(DrawChord)
#define		CXPROPID_EndX						(CXDISPID_PROP + 27)			//终点横坐标(DrawChord)
#define		CXPROPID_EndY						(CXDISPID_PROP + 28)			//终点纵坐标(DrawChord)
#define		CXPROPID_MiddleX					(CXDISPID_PROP + 29)			//中间点横坐标(DrawChord)
#define		CXPROPID_MiddleY					(CXDISPID_PROP + 30)			//中间点纵坐标(DrawChord)
#define		CXPROPID_CenterX					(CXDISPID_PROP + 31)			//中心点横坐标(DrawPie)
#define		CXPROPID_CenterY					(CXDISPID_PROP + 32)			//中心点纵坐标(DrawPie)
#define		CXPROPID_PieType					(CXDISPID_PROP + 33)			//画弧方式(DrawPie)
#define		CXPROPID_StartAngle					(CXDISPID_PROP + 34)			//园弧起始角度(DrawPie)
#define		CXPROPID_SweepAngle					(CXDISPID_PROP + 35)			//园弧终止角度(DrawPie)
#define		CXPROPID_Radius						(CXDISPID_PROP + 36)			//园弧半径(DrawPie)
#define		CXPROPID_LineJoinStyle				(CXDISPID_PROP + 37)			//线接头类型(DrawPolyLine & DrawLine)
#define		CXPROPID_Caption					(CXDISPID_PROP + 38)			//文本(DrawText)
#define		CXPROPID_Font						(CXDISPID_PROP + 39)
#define		CXPROPID_FontName					(CXDISPID_PROP + 40)			//字体名(DrawText)
#define		CXPROPID_FontSize					(CXDISPID_PROP + 41)
#define		CXPROPID_FontStyle					(CXDISPID_PROP + 42)
#define		CXPROPID_UnderLine					(CXDISPID_PROP + 43)
#define		CXPROPID_StrikeThrough				(CXDISPID_PROP + 44)
#define		CXPROPID_Alignment					(CXDISPID_PROP + 45)
#define		CXPROPID_FontColor					(CXDISPID_PROP + 46) 
#define		CXPROPID_AutoSize					(CXDISPID_PROP + 47)			//自动尺寸(DrawText)
#define		CXPROPID_Visible					(CXDISPID_PROP + 48)			//可见(All)
#define		CXPROPID_Layer						(CXDISPID_PROP + 49)			//层(All)
#define		CXPROPID_RoateBasePointX			(CXDISPID_PROP + 50)
#define		CXPROPID_RoateBasePointY			(CXDISPID_PROP + 51)
#define		CXPROPID_ToolTipText				(CXDISPID_PROP + 52)			//提示文本
#define		CXPROPID_MousePointer				(CXDISPID_PROP + 53)			//鼠标指针
#define		CXPROPID_SecurityArea				(CXDISPID_PROP + 54)			//安全区
#define		CXPROPID_Flow						(CXDISPID_PROP + 55)		
#define		CXPROPID_FlowDirection				(CXDISPID_PROP + 56)			
#define		CXPROPID_FlowSpeed					(CXDISPID_PROP + 57)
#define		CXPROPID_LiquidColor				(CXDISPID_PROP + 58) 
#define		CXPROPID_ScanInterval				(CXDISPID_PROP + 59) 
#define		CXPROPID_GridEnabled				(CXDISPID_PROP + 60)
#define		CXPROPID_GridColor  				(CXDISPID_PROP + 61) 
#define		CXPROPID_VisibleLayer				(CXDISPID_PROP + 62)  
#define		CXPROPID_TagGroup					(CXDISPID_PROP + 63)
#define		CXPROPID_AssocTagGroup				(CXDISPID_PROP + 64)
#define		CXPROPID_Description				(CXDISPID_PROP + 65)
#define		CXPROPID_Privilege					(CXDISPID_PROP + 66)			//权限
#define		CXPROPID_LeftMargin					(CXDISPID_PROP + 67)
#define		CXPROPID_TopMargin					(CXDISPID_PROP + 68)
#define		CXPROPID_RightMargin				(CXDISPID_PROP + 69)
#define		CXPROPID_BottomMargin				(CXDISPID_PROP + 70)
#define		CXPROPID_RecordSet					(CXDISPID_PROP + 71)
#define		CXPROPID_KeepTogether				(CXDISPID_PROP + 72)
#define		CXPROPID_ForcePageBreak 			(CXDISPID_PROP + 73)
#define		CXPROPID_DataConnection				(CXDISPID_PROP + 74)
#define		CXPROPID_DataRecordset				(CXDISPID_PROP + 75)
#define		CXPROPID_PageCount					(CXDISPID_PROP + 76)
#define		CXPROPID_Page						(CXDISPID_PROP + 77)
#define		CXPROPID_BorderStyle				(CXDISPID_PROP + 78)
#define		CXPROPID_StartUpPosition			(CXDISPID_PROP + 79)
//#define		CXPROPID_ShowModal					(CXDISPID_PROP + 80)
#define		CXPROPID_Value						0
#define		CXPROPID_VariableType				(CXDISPID_PROP + 81)
#define		CXPROPID_Transparency				(CXDISPID_PROP + 82)
#define		CXPROPID_Enabled					(CXDISPID_PROP + 83)
#define		CXPROPID_Locked						(CXDISPID_PROP + 84)
#define		CXPROPID_Menu						(CXDISPID_PROP + 85)
#define		CXPROPID_TabIndex					(CXDISPID_PROP + 86)
#define		CXPROPID_TabStop					(CXDISPID_PROP + 87)
#define		CXPROPID_Default					(CXDISPID_PROP + 88)
#define		CXPROPID_Cancel						(CXDISPID_PROP + 89)
#define		CXPROPID_Objects					(CXDISPID_PROP + 90)
#define		CXPROPID_WordWrap					(CXDISPID_PROP + 91)
#define		CXPROPID_IdealHeight				(CXDISPID_PROP + 92)
#define		CXPROPID_ControlBox					(CXDISPID_PROP + 93)
#define		CXPROPID_WindowHandle				(CXDISPID_PROP + 94)
#define		CXPROPID_AllowDirectDraw			(CXDISPID_PROP + 95)

#define		CXMETHID_IsConnected				(CXDISPID_METH)
#define		CXMETHID_ConnectObject				(CXDISPID_METH + 1)
#define		CXMETHID_ConnectDirect				(CXDISPID_METH + 2)		
#define		CXMETHID_GetConnectObject			(CXDISPID_METH + 3)		
#define		CXMETHID_Disconnect					(CXDISPID_METH + 4)			
#define		CXMETHID_GetConnectState			(CXDISPID_METH + 5)
#define		CXMETHID_AddPoint					(CXDISPID_METH + 6)		
#define		CXMETHID_GetNumberOfPoints			(CXDISPID_METH + 7)		
#define		CXMETHID_DeletePoint				(CXDISPID_METH + 8)			
#define		CXMETHID_GetPointAt					(CXDISPID_METH + 9)
#define		CXMETHID_SetFocus					(CXDISPID_METH + 10)
#define		CXMETHID_GetHitObject				(CXDISPID_METH + 11)
#define		CXMETHID_GetObjectByName			(CXDISPID_METH + 12)
#define		CXMETHID_ForceRefresh				(CXDISPID_METH + 13)
#define		CXMETHID_BindDataLinksToTagGroup	(CXDISPID_METH + 14)

#define		CXEVENID_CLICK						(CXDISPID_EVEN) 
#define		CXEVENID_DBLCLICK					(CXDISPID_EVEN + 1) 
#define		CXEVENID_KEYDOWN					(CXDISPID_EVEN + 2)   
#define		CXEVENID_KEYUP						(CXDISPID_EVEN + 3)   
#define		CXEVENID_KEYPRESS					(CXDISPID_EVEN + 4)   
#define		CXEVENID_MOUSEDOWN					(CXDISPID_EVEN + 5)   
#define		CXEVENID_MOUSEMOVE					(CXDISPID_EVEN + 6)   
#define		CXEVENID_MOUSEUP					(CXDISPID_EVEN + 7)
#define		CXEVENID_INITIALIZE					(CXDISPID_EVEN + 8) 
#define		CXEVENID_ACTIVATE					(CXDISPID_EVEN + 9)   
#define		CXEVENID_DEACTIVATE					(CXDISPID_EVEN + 10)  
#define		CXEVENID_SCAN						(CXDISPID_EVEN + 11)
#define		CXEVENID_MOUSELEAVE					(CXDISPID_EVEN + 12)
#define		CXEVENID_TERMINATE					(CXDISPID_EVEN + 13)
#define		CXEVENID_RESIZE						(CXDISPID_EVEN + 14)
#define		CXEVENID_QUERYCLOSE					(CXDISPID_EVEN + 15)

#define		CXPROPID_DEFAULT_EDGECOLOR			0x80020000
#define		CXPROPID_DEFAULT_FILLCOLOR			0x80020001
#define		CXPROPID_DEFAULT_FILLSTYLE			0x80020002
#define		CXPROPID_DEFAULT_FONTCOLOR			0x80020003

#endif //__CXCOMDISPID_H__