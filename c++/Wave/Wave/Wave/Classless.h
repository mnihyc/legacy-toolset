/*============================================================================*/
/*                                                                            */
/*                            C O P Y R I G H T                               */
/*                                                                            */
/*                          (C) Copyright 2016 by                             */
/*                              Yang Guojun                                   */
/*                           All Rights Reserved                              */
/*                                                                            */
/*      The author assumes no responsibility for the use or reliability of    */
/*      his software.                                                         */
/*                                                                            */
/*============================================================================*/

/* ############################################################################################################################## */

#ifndef __CLASSLESS_H_122333444455555__
#define __CLASSLESS_H_122333444455555__

#include <windows.h>

//用于动态链接库的时候，注释掉#else中的第二行
//用于静态库或者源码时，注释掉#else中的第一行
#ifdef CChart_EXPORTS
#	define CChart_API __declspec(dllexport)
#else
#	define CChart_API __declspec(dllimport)
//#	define CChart_API
#endif

namespace CChart_Classless
{
/////////////////////////////////////////////////////////////////////////////
// 以下函数来自CChartWnd
	// 获得视图类型
	CChart_API int		GetChartType(int nChartIndex=0);
	// 改变视图类型
	CChart_API bool	SetChartType(int nType, int nChartIndex=0);
	// 获得窗口句柄
	CChart_API HWND	GetHWnd(int nHWndIndex=0);
	// 设置窗口句柄
	CChart_API void	SetHWnd(HWND hWnd, int nHWndIndex=0);

	// 粘窗口，最基本形式
	CChart_API bool	Attach(HWND hWnd);
	// 粘窗口，同时设置绘图类型
	CChart_API bool	Attach(HWND hWnd, int nType);
	// 粘窗口，同时设置绘图类型，并限制绘图区
	CChart_API bool	Attach(HWND hWnd, int nType, RECT rtClient);
	// 粘窗口，粘到对话框上, 给定控件句柄
	CChart_API bool	Attach(HWND hDlg, HWND hCtrl, int nType);
	// 粘窗口，粘到对话框上，给定控件ID
	CChart_API bool	Attach(HWND hDlg, UINT nCtrlID, int nType);
	// 卸窗口
	CChart_API bool	Detach();
	CChart_API bool	Detach(HWND hWnd);
	// 重绘
	CChart_API void	ReDraw();
	CChart_API void	ReDraw(HWND hWnd);
	CChart_API void	ReDraw(int nChartIndex);

/////////////////////////////////////////////////////////////////////////////
// 以下函数来自CChart
	// GDI+是否初始化
	CChart_API bool	IsGdiPlusInitialized();
	//初始化GDI+
	CChart_API void	InitGdiPlus();
	// 减小GDI+初始化计数，如果减小到0，彻底释放
	CChart_API void	FreeGdiPlus();
	// 释放GDI+
	//CChart_API void	FinalizeGdiPlus();
	// 设置对话框、右键菜单使用的语言
	CChart_API	void	SetLangurage(int nLang);
	// 设置曲线是否可以用鼠标移动
	CChart_API	void	SetOscilloMenus(bool bMoveable);
	// 设置数据处理菜单是否显示
	CChart_API	void	SetDataProcessing(bool process);
	// 拷贝图像到剪贴板
	CChart_API	void	CopyImageToClipBoard(HWND hWnd);
	// 保存图像到文件
	CChart_API	void	SaveImageToFile(HWND hWnd, TCHAR *strFileName, TCHAR *strFileExt);
	// 打印图像
	CChart_API	void	PrintImage(HWND hWnd);

	// 测试是否启用工具提示
	CChart_API	bool	IsEnableToolTip();
	// 设置是否启用工具提示
	CChart_API	void	SetEnableToolTip(bool enable);
	// 设置工具提示字符串类型
	CChart_API	void	SetToolTipType(int type);
	// 检查视图号是否有效
	CChart_API bool	CheckPlotType(int nType);
	// 设置默认颜色配置表
	CChart_API void	SetColorTable(int nTableIndex);
	
	// 测试是否响应鼠标键盘消息
	CChart_API bool		IsReact();
	// 设置是否响应鼠标键盘消息
	CChart_API void		SetReact(bool react);
	// 测试消息响应时是否自动刷新，主要用于末尾带R的几个函数和Interactive
	CChart_API bool		IsAutoRedraw();
	// 设置消息响应时是否自动刷新，主要用于末尾带R的几个函数和Interactive
	CChart_API void		SetAutoRedraw(bool redraw);
	// 测试是否限制绘图区
	CChart_API bool		IsRectConfined();
	// 设置是否限制绘图区
	CChart_API void		SetRectConfined(bool confine);
	// 测试双缓冲是否使用
	CChart_API bool		IsBuffered();
	// 设置双缓冲是否使用
	CChart_API void		SetBuffered(bool buffered);
	// 设置绘图区偏移
	CChart_API void		SetOffsetClient(int offsetX, int offsetY);

	// 增加右键菜单项，插件菜单功能
	CChart_API void		SetPluginMenu(void (*fcnAddPluginMenu)( void *plot, HMENU addMenu ), int (*fcnInterpretPluginMenu)( void *plot, HWND hWnd, int result ), void *pPara);
	// 隐藏或显示缺省右键菜单，结合插件菜单功能可实现右键菜单全部自定义
	CChart_API void		SetDefMenus(bool bShow);
	// 用户自定义绘图操作
	CChart_API void		SetUserDrawingFunc(void	(*fcnUserDrawing)( HDC hDC, RECT clientRect, RECT plotRect, void *pPara ), void *pPara);
	// 用户自定义绘图操作
	CChart_API void		SetUserDrawingFunc(void	(*fcnUserDrawing)( HDC hDC, RECT clientRect, RECT plotRect, void *pPara ), void *pPara, int nPlotIndex);
	// 用户自定义更新操作
	CChart_API void		SetUserUpdateFunc(void	(*fcnUserUpdate)( void *pPara ), void *pPara);
	// 用户自定义更新操作
	CChart_API void		SetUserUpdateFunc(void	(*fcnUserUpdate)( void *pPara ), void *pPara, int nPlotIndex);
	// 用户自定义鼠标移动前操作
	CChart_API void		SetPreMouseMoveFunc(bool	(*fcnPreMouseMove)( void *plot, HDC hDC, POINT point, UINT ctrlKey, void *para ), void *pPara);
	// 用户自定义鼠标单击前操作
	CChart_API void		SetPreLButtonDownFunc(bool	(*fcnPreLButtonDown)( void *plot, HDC hDC, POINT point, UINT ctrlKey, void *para ), void *pPara);
	// 用户自定义鼠标抬起前操作
	CChart_API void		SetPreLButtonUpFunc(bool	(*fcnPreLButtonUp)( void *plot, HDC hDC, POINT point, UINT ctrlKey, void *para ), void *pPara);
	// 用户自定义鼠标双击前操作
	CChart_API void		SetPreLButtonDblClkFunc(bool	(*fcnPreLButtonDblClk)( void *plot, HDC hDC, POINT point, UINT ctrlKey, void *para ), void *pPara);
	// 用户自定义按键前操作
	CChart_API void		SetPreKeyDownFunc(bool	(*fcnPreKeyDown)( void *plot, HDC hDC, UINT key, void *para ), void *pPara);
	// 用户自定义鼠标移动后操作
	CChart_API void		SetPostMouseMoveFunc(bool	(*fcnPostMouseMove)( void *plot, HDC hDC, POINT point, UINT ctrlKey, void *para ), void *pPara);
	// 用户自定义鼠标单击后操作
	CChart_API void		SetPostLButtonDownFunc(bool	(*fcnPostLButtonDown)( void *plot, HDC hDC, POINT point, UINT ctrlKey, void *para ), void *pPara);
	// 用户自定义鼠标抬起后操作
	CChart_API void		SetPostLButtonUpFunc(bool	(*fcnPostLButtonUp)( void *plot, HDC hDC, POINT point, UINT ctrlKey, void *para ), void *pPara);
	// 用户自定义鼠标双击后操作
	CChart_API void		SetPostLButtonDblClkFunc(bool	(*fcnPostLButtonDblClk)( void *plot, HDC hDC, POINT point, UINT ctrlKey, void *para ), void *pPara);
	// 用户自定义按键后操作
	CChart_API void		SetPostKeyDownFunc(bool	(*fcnPreKeyDown)( void *plot, HDC hDC, UINT key, void *para ), void *pPara);

	// 获得子图序号，仅仅用于kTypeSplit
	CChart_API int			GetPlotIndexByMousePos(POINT point);

	// 设置在没有数据的时候是否画坐标轴及标题
	CChart_API void		SetDrawBasicAnyway(bool draw);
	// 设置在没有数据的时候子图是否画坐标轴及标题
	CChart_API void		SetDrawBasicAnyway(bool draw, int nPlotIndex);
	// 设置无数据时默认屏幕
	CChart_API void		SetDefScreen( void (*fcnDefScreen)( void *plot, HDC hDC, RECT plotRect, void *pPara ), void *pPara );
	// 设置无数据时子图默认屏幕
	CChart_API void		SetDefScreen( void (*fcnDefScreen)( void *plot, HDC hDC, RECT plotRect, void *pPara ), void *pPara, int nPlotIndex );

	// 获取图像类型编码
	CChart_API int			GetType();
	// 设置图像类型编码
	CChart_API bool		SetType(int nType);
	// 设置图像类型编码，并设置绘图窗口，主要用于三维视图，二维视图将忽略hWnd
	CChart_API bool		SetType(int nType, HWND hWnd);
	
	// 设置限制区域
	CChart_API bool		SetConfineRect(RECT ConfineRect);
	// 获取限制区域
	CChart_API RECT		GetConfinedRect();
	
	// 测试内部是否无数据
	CChart_API bool		IsEmpty();

	// 重置外观数据
	CChart_API void		ResetApperance();
	// 获得外观模式序号
	CChart_API int			GetApperanceMode();
	// 设置外观模式
	CChart_API void		SetApperanceMode(int mode);

	// 添加曲线，前三个参数是曲线数据
	// 第四个参数主要用于分裂视图、分层视图、共享X轴视图，表示子图序号。后续函数相同
	// 返回值是曲线的ID号，注意不是序号，返回-1表示添加曲线失败。后续函数相同
	CChart_API int			AddCurve(double *pX, double *pY, int nLen, int nPlotIndex=0);
	// 添加曲线，其中X坐标为时间，用字符串形式输入
	//字符串应为"20130528234428"，即年月日时分秒，共14位数字，否则出错
	CChart_API int			AddCurve(TCHAR **pStrTime, double *pY, int nLen, int nPlotIndex=0);
	// 添加空曲线
	CChart_API int			AddCurve(int nPlotIndex=0);
	// 瀑布图专用
	CChart_API int			AddCurve(double *pX, double *pY, double z, int nLen, int nPlotIndex=0);
	// 更新曲线数据
	CChart_API int			UpdateCurve(int nDataIndex, double *pX, double *pY, int nLen, int nPlotIndex=0);
	// 根据ID号更新曲线数据
	CChart_API int			UpdateCurveByID(int nDataID, double *pX, double *pY, int nLen, int nPlotIndex=0);
	// 从曲线的DataID获得Index
	CChart_API int			GetIndex(int dataID, int nPlotIndex=0);
	// 从曲线的Index获得DataID
	CChart_API int			GetDataID(int nIndex, int nPlotIndex=0);
	// 添加饼图的一个值
	CChart_API int			AddPie(double value);
	// 添加饼图的一个值，同时设置该块饼的标题
	CChart_API int			AddPie(double value, TCHAR* title);
	// 添加柱图的一个序列
	CChart_API int			AddStems(double *pData, int nLen);
	// 添加柱图的一个序列，同时设置该序列的标题
	CChart_API int			AddStems(double *pData, int nLen, TCHAR* title);
	// 添加2维曲线的一个数据点
	// nDataIndex表示曲线的序号,
	// nDataIndex如果等于当前曲线的条数，则新增一条曲线，如果小于0或者大于当前曲线条数，则添加失败
	CChart_API int			AddPoint2D(double x, double y, int nDataIndex=0, int nPlotIndex=0);
	// 添加数据点，其中X坐标为时间，用字符串形式输入
	//字符串应为"20130528234428"，即年月日时分秒，共14位数字，否则出错
	CChart_API int			AddPoint2D(TCHAR *strTime, double y, int nDataIndex=0, int nPlotIndex=0);
	// 插入2维曲线的一个数据点
	// nPos表示需要插入的位置,
	CChart_API int			InsertPoint2D(double x, double y, int nPos, int nDataIndex=0, int nPlotIndex=0);
	// 添加三维曲线的一个数据点
	CChart_API int			AddPoint3D(double x, double y, double z, int nDataIndex=0);
	// 添加一条三维曲线
	CChart_API int			AddCurve(double *pX, double *pY, double *pZ, int nLen);	
	// 设置场函数，用于等高线图、云图、三维曲面图
	CChart_API void		SetFieldFcn(double (*_pFieldFcn) (double, double));
	// 获得场函数指针，用于等高线图、云图、三维曲面图
	CChart_API double		(*GetFieldFcn( ))( double, double );
	// 添加等高线数据点
	CChart_API void		AddContourPoint(double x, double y, double h);
	// 清除等高线数据点
	CChart_API void		ClrContourPoints();
	// 清除所有曲线数据
	CChart_API void		ClrAllData();
	// 清除单子图全部曲线数据
	CChart_API void		ClrPlotData(int nPlotIndex=0);
	// 清除单条曲线数据
	CChart_API void		ClrSingleData(int nDataIndex, int nPlotIndex=0);
	// 清空单条曲线数据
	CChart_API void		EmptySingleData(int nDataIndex, int nPlotIndex=0);
	// 为曲线预留内存空间。主要目的是加快速度
	// CChart内部数据采用vector。如果数据点过多，随着数据点的增加，CChart将不断地为vector重新分配内存，影响速度
	// 如果根据实际情况预留了内存，则会一次性成功分配内存
	CChart_API void		SetReservedDataLength(int nLen, int nPlotIndex=0);
	// 获得曲线的数据点数
	CChart_API int			GetDataPointsCount(int nDataIndex, int nPlotIndex=0);
	// 获取数据点x
	CChart_API double		GetDataPointX(int nPointIndex, int nDataIndex, int nPlotIndex=0);
	// 获取数据点y
	CChart_API double		GetDataPointY(int nPointIndex, int nDataIndex, int nPlotIndex=0);
	//获取数据点
	CChart_API bool		GetDataPoint(double data[2], int nPointIndex, int nDataIndex, int nPlotIndex=0);
	// 去除曲线上单个数据点
	CChart_API int			RemoveSinglePoint(int nPointIndex, int nDataIndex, int nPlotIndex=0);
	// 去除曲线上第一个数据点
	CChart_API int			RemoveFirstPoint(int nDataIndex, int nPlotIndex=0);
	// 去除曲线上第一个数据点，同时内部数据左移，保证占用内存位置不变
	CChart_API int			RemoveFirstPointAndShift(int nDataIndex, int nPlotIndex=0);
	// 去除曲线上最后一个数据点
	CChart_API int			RemoveLastPoint(int nDataIndex, int nPlotIndex=0);
	// 滑动X数据
	CChart_API int			SlipXValue(int nStep, int nDataIndex, int nPlotIndex=0);
	// 滑动Y数据
	CChart_API int			SlipYValue(int nStep, int nDataIndex, int nPlotIndex=0);

	// 增加一个图层，用于分层视图、共享X轴视图
	CChart_API int			AddLayer();
	// 设置子图数量，用于分裂视图，其中mode表示分裂模式，nRows表示列数，nCols表示行数
	// 分裂模式参加本文件头部被注释掉的enum。可以解除注释，就能直接使用这些enum了。
	CChart_API void		ResizePlots(int mode, int nRows, int nCols);
	// 设置图层数量，用于分层视图、共享X轴视图
	CChart_API void		ResizePlots(int nLayers);
	// 设置分裂视图子图数
	CChart_API	bool		ResizeSplit(int nSubPlots);
	
	// 获得图像标题
	CChart_API const TCHAR*		GetTitle();
	// 获得子图标题，可用于分裂视图、分层视图、共享X轴视图
	CChart_API const TCHAR*		GetTitle(int nPlotIndex);
	// 设置图像标题
	CChart_API void		SetTitle(const TCHAR* title);
	// 设置子图标题
	CChart_API void		SetTitle(const TCHAR* title, int nPlotIndex);
	// 获得图像标题位置
	CChart_API int			GetTitlePosition();
	// 获得子图标题位置
	CChart_API int			GetTitlePosition(int nPlotIndex);
	// 设置图像标题位置。负数：左；0：中；正数：右
	CChart_API void		SetTitlePosition(int position);
	// 设置子图标题位置。负数：左；0：中；正数：右
	CChart_API void		SetTitlePosition(int position, int nPlotIndex);
	// 图像标题是否显示
	CChart_API bool		IsTitleShow();
	// 子图标题是否显示
	CChart_API bool		IsTitleShow(int nPlotIndex);
	// 设置显示图像标题
	CChart_API void		SetTitleShow(bool show);
	// 设置显示子图标题
	CChart_API void		SetTitleShow(bool show, int nPlotIndex);
	// 获得图像标题颜色
	CChart_API COLORREF	GetTitleColor();
	// 获得子图标题颜色
	CChart_API COLORREF	GetTitleColor(int nPlotIndex);
	// 设置图像标题颜色
	CChart_API void		SetTitleColor(COLORREF color);
	// 设置子图标题颜色
	CChart_API void		SetTitleColor(COLORREF color, int nPlotIndex);
	// 获得图像副标题
	CChart_API TCHAR*		GetSubTitle();
	// 获得子图副标题，可用于分裂视图、分层视图、共享X轴视图
	CChart_API TCHAR*		GetSubTitle(int nPlotIndex);
	// 设置图像副标题
	CChart_API void		SetSubTitle(const TCHAR* title);
	// 设置子图副标题
	CChart_API void		SetSubTitle(const TCHAR* title, int nPlotIndex);
	// 获得图像副标题位置
	CChart_API int			GetSubTitlePosition();
	// 获得子图副标题位置
	CChart_API int			GetSubTitlePosition(int nPlotIndex);
	// 设置图像副标题位置。负数：左；0：中；正数：右
	CChart_API void		SetSubTitlePosition(int position);
	// 设置子图副标题位置。负数：左；0：中；正数：右
	CChart_API void		SetSubTitlePosition(int position, int nPlotIndex);
	// 图像副标题是否显示
	CChart_API bool		IsSubTitleShow();
	// 子图副标题是否显示
	CChart_API bool		IsSubTitleShow(int nPlotIndex);
	// 设置显示图像副标题
	CChart_API void		SetSubTitleShow(bool show);
	// 设置显示子图副标题
	CChart_API void		SetSubTitleShow(bool show, int nPlotIndex);
	// 获得图像副标题颜色
	CChart_API COLORREF	GetSubTitleColor();
	// 获得子图副标题颜色
	CChart_API COLORREF	GetSubTitleColor(int nPlotIndex);
	// 设置图像副标题颜色
	CChart_API void		SetSubTitleColor(COLORREF color);
	// 设置子图副标题颜色
	CChart_API void		SetSubTitleColor(COLORREF color, int nPlotIndex);
	// 分层视图显示主标题
	CChart_API void		SetShowParentTitle(bool set);
	// 获得标题字体
	CChart_API LOGFONT		GetTitleFont();
	// 获得子图标题字体
	CChart_API LOGFONT		GetTitleFont(int nPlotIndex);
	// 设置标题字体
	CChart_API void		SetTitleFont(LOGFONT logFont);
	// 设置子图标题字体
	CChart_API void		SetTitleFont(LOGFONT logFont, int nPlotIndex);
	// 获得副标题字体
	CChart_API LOGFONT		GetSubTitleFont();
	// 获得子图副标题字体
	CChart_API LOGFONT		GetSubTitleFont(int nPlotIndex);
	// 设置副标题字体
	CChart_API void		SetSubTitleFont(LOGFONT logFont);
	// 设置子图副标题字体
	CChart_API void		SetSubTitleFont(LOGFONT logFont, int nPlotIndex);

	// 设置高亮边框颜色
	CChart_API void		SetLightColor(COLORREF color);
	// 设置子图高亮边框颜色
	CChart_API void		SetLightColor(COLORREF color, int nPlotIndex);
	// 设置高亮边框线宽
	CChart_API void		SetLightLineSize(int nSize);
	// 设置子图高亮边框线宽
	CChart_API void		SetLightLineSize(int nSize, int nPlotIndex);
	// 设置高亮边框线型
	CChart_API void		SetLightLineStyle(int nStyle);
	// 设置子图高亮边框线型
	CChart_API void		SetLightLineStyle(int nStyle, int nPlotIndex);

	// 获得子图数量
	CChart_API int			GetSubPlotsCount();
	
	// 设置单条曲线是否响应鼠标键盘消息
	CChart_API void		SetReact(bool react, int nDataIndex, int nPlotIndex=0);

	// 获得数据曲线的标题,nDataIndex表示曲线的序号
	CChart_API const TCHAR*		GetDataTitle(int nDataIndex, int nPlotIndex=0);
	// 设置数据标题
	CChart_API void		SetDataTitle(const TCHAR* title, int nDataIndex, int nPlotIndex=0);
	// 获得数据颜色
	CChart_API COLORREF	GetDataColor(int nDataIndex, int nPlotIndex=0);
	// 设置数据颜色
	CChart_API void		SetDataColor(COLORREF color, int nDataIndex, int nPlotIndex=0);
	// 是否双色渐变模式
	CChart_API bool		IsBiColorMode(int nDataIndex, int nPlotIndex=0);
	// 设置双色渐变模式
	CChart_API void		SetBiColorMode(bool bBi, int nDataIndex, int nPlotIndex=0);
	// 获得双色渐变模式下第二数据颜色
	CChart_API COLORREF	GetDataColor2(int nDataIndex, int nPlotIndex=0);
	// 设置双色渐变模式下第二数据颜色
	CChart_API void		SetDataColor2(COLORREF color, int nDataIndex, int nPlotIndex=0);
	// 是否多色渐变模式
	CChart_API bool		IsMultiColorMode(int nDataIndex, int nPlotIndex=0);
	// 设置多色渐变模式
	CChart_API void		SetMultiColorMode(bool bMul, int nDataIndex, int nPlotIndex=0);
	// 获得多色渐变模式下输入节点的方法，0：数据点比例0.0-1.0，1：X数据值，2：Y数据值
	CChart_API int			GetMultiColorInputType(int nDataIndex, int nPlotIndex=0);
	// 设置多色渐变模式下输入节点的方法
	CChart_API void		SetMultiColorInputType(int nType, int nDataIndex, int nPlotIndex=0);
	// 设置多色渐变模式的节点，其中ratio的的范围由输入方法决定
	CChart_API void		AddSegmentColor(double ratio, COLORREF color, int nDataIndex, int nPlotIndex=0);
	// 是否逐点定色
	CChart_API bool		IsColorPtByPt(int nDataIndex, int nPlotIndex=0);
	// 是否逐点定色
	CChart_API void		SetColorPtByPt(bool bBy, int nDataIndex, int nPlotIndex=0);
	// 添加数据点颜色
	CChart_API void		AddDataPointColor(COLORREF color, int nDataIndex, int nPlotIndex=0);
	// 设置数据点颜色
	CChart_API void		SetDataPointColor(COLORREF color, int nPointIndex, int nDataIndex, int nPlotIndex=0);
	// 删除一个数据点颜色
	CChart_API void		EraseDataPointColor(int nPointIndex, int nDataIndex, int nPlotIndex=0);
	// 插入一个数据点颜色
	CChart_API void		InsertDataPointColor(COLORREF color, int nPointIndex, int nDataIndex, int nPlotIndex=0);
	// 获得数据线型
	CChart_API int			GetDataLineStyle(int nDataIndex, int nPlotIndex=0);
	// 设置数据线型
	CChart_API void		SetDataLineStyle(int nStyle, int nDataIndex, int nPlotIndex=0);
	// 获得数据线宽
	CChart_API int			GetDataLineSize(int nDataIndex, int nPlotIndex=0);
	// 设置数据宽
	CChart_API void		SetDataLineSize(int nSize, int nDataIndex, int nPlotIndex=0);
	// 设置曲线的绘制方式
	CChart_API void		SetPlotType(int nType, int nDataIndex, int nPlotIndex=0);
	// 设置曲线数据点标记是否显示
	CChart_API void		SetMarkerShow(bool bShow, int nDataIndex, int nPlotIndex=0);
	// 设置曲线数据点标记是否填充
	CChart_API void		SetMarkerFill(bool bFill, int nDataIndex, int nPlotIndex=0);
	// 设置曲线数据点标记的形状
	CChart_API void		SetMarkerType(int nType, int nDataIndex, int nPlotIndex=0);
	// 设置曲线数据点的大小
	CChart_API void		SetMarkerSize(int nSize, int nDataIndex, int nPlotIndex=0);
	// 设置曲线数据点标记频率，0表示不标记，正n表示每隔n个点标记一次，负n表示一共标记n个点
	CChart_API void		SetMarkerFreq(int nFreq, int nDataIndex, int nPlotIndex=0);
	// 设置曲线数据点标记的起始点，用于不是全部标记时
	CChart_API void		SetMarkerStartIndex(int nStartIndex, int nDataIndex, int nPlotIndex=0);
	// 设置曲线数据点第一个点标记
	CChart_API void		SetMarkFirstPoint(bool bMark, int nPlotIndex=0);
	// 设置曲线数据点最后一个点标记
	CChart_API void		SetMarkLastPoint(bool bMark, int nPlotIndex=0);
	// 设置曲线散点显示模式
	CChart_API void		SetScatter(int nDataIndex, int nPlotIndex=0);
	// 设置曲线填充颜色
	CChart_API void		SetDataFillColor(bool bFill, COLORREF color, int nOpaque, int nFillMode, int nDataIndex, int nPlotIndex=0);
	// 设置曲线数据棒显示
	CChart_API void		SetDataBarMode(bool bShowBar, int nBarBase, int nBarSize, int nDataIndex, int nPlotIndex=0);
	// 设置选择和高亮模式
	CChart_API void		SetSLMode(int nMode, int nDataIndex, int nPlotIndex=0);
	// 获取数据的标志，特殊用途
	CChart_API bool		GetDataFlag(int nDataIndex, int nPlotIndex=0);
	// 设置数据的标志，特殊用途
	CChart_API void		SetDataFlag(bool bFlag, int nDataIndex, int nPlotIndex=0);
	// 获取数据可见标志
	CChart_API bool		IsDataVisible(int nDataIndex, int nPlotIndex=0);
	//设置数据是否可见
	CChart_API void		SetDataVisible(bool bVis, int nDataIndex, int nPlotIndex=0);
	// 设置压缩显示模式，主要用于大数据量时的显示
	// 压缩显示模式中，如果相邻数据点在屏幕上显示时的X相同，将压缩成一个数据点，值取平均，
	CChart_API void		SetDataCompactDraw(bool bCompact, int nDataIndex, int nPlotIndex=0);
	// 设置数据按直方图模式显示
	CChart_API void		SetDataHistro(bool bHistro, int nDataIndex, int nPlotIndex=0);

	// 设置曲线数据点数值是否显示
	CChart_API void		SetDataValueShow(bool bShow, int nDataIndex, int nPlotIndex=0);
	// 设置曲线数据点数值显示方式，1为X，2为Y，3为X:Y，其它为不显示
	CChart_API void		SetDataValueType(int nType, int nDataIndex, int nPlotIndex=0);
	// 设置曲线数据点数值的字体大小
	CChart_API void		SetDataValueFontSize(int nSize, int nDataIndex, int nPlotIndex=0);
	// 设置曲线数据点数值显示频率，0表示不显示，正n表示每隔n个点显示一次，负n表示一共显示n个点
	CChart_API void		SetDataValueFreq(int nFreq, int nDataIndex, int nPlotIndex=0);
	// 设置曲线数据点数值显示的起始点，用于不是全部显示时
	CChart_API void		SetDataValueStartIndex(int nStartIndex, int nDataIndex, int nPlotIndex=0);


	// 检测示波器模式
	CChart_API bool		IsOscilloMode(int nPlotIndex=0);
	// 设置示波器模式
	CChart_API void		SetOscilloMode(bool bOscillo, int nPlotIndex=0);
	// 检测是否可以拖动曲线
	CChart_API bool		IsEnableDataDrag();
	// 设置是否可以拖动曲线
	CChart_API void		SetEnableDataDrag(bool enable);
	// 检测是否可以拖动区间边线
	CChart_API bool		IsEnableZoneDrag();
	// 设置是否可以拖动区间边线
	CChart_API void		SetEnableZoneDrag(bool enable);
	// 移动曲线
	CChart_API bool		MoveData(double offsetx, double offsety, int nDataIndex, int nPlotIndex=0);
	// 获得曲线的水平偏移
	CChart_API double		GetDataOffsetX(int nDataIndex, int nPlotIndex=0);
	// 设置曲线的水平偏移
	CChart_API void		SetDataOffsetX(double offset, int nDataIndex, int nPlotIndex=0);
	// 获得曲线的竖直偏移
	CChart_API double		GetDataOffsetY(int nDataIndex, int nPlotIndex=0);
	// 设置曲线的竖直偏移
	CChart_API void		SetDataOffsetY(double offset, int nDataIndex, int nPlotIndex=0);
	// 设置X方向单位，仅仅用于模拟示波器
	CChart_API void		SetXUnit(TCHAR *unit, int nDataIndex);
	// 设置Y方向单位，仅仅用于模拟示波器
	CChart_API void		SetYUnit(TCHAR *unit, int nDataIndex);
	// 在图例中仅仅显示Y方向的比例尺，仅仅用于模拟示波器
	CChart_API void		SetShowYSCaleOnly(bool only);
	// 检测光标绑定到鼠标
	CChart_API bool		IsHoldCursorToMouse(int nPlotIndex=0);
	// 设置光标绑定到鼠标
	CChart_API void		SetHoldCursorToMouse(bool hold, int nPlotIndex=0);
	// 检测光标绑定到曲线
	CChart_API bool		IsHoldCursorToCurve(int nPlotIndex=0);
	// 设置光标绑定到曲线
	CChart_API void		SetHoldCursorToCurve(bool hold, int nPlotIndex=0);
	// 获得光标绑定的曲线序号
	CChart_API int			GetIndexToHoldCursor(int nPlotIndex=0);
	// 设置光标绑定的曲线序号
	CChart_API void		SetIndexToHoldCursor(int nCurveIndex, int nPlotIndex=0);
	// 检测零点箭头绘制
	CChart_API bool		IsShowZeroArrow(int nPlotIndex=0);
	// 设置零点箭头绘制
	CChart_API void		SetShowZeroArrow(bool show, int nPlotIndex=0);
	// 检测触发箭头绘制
	CChart_API bool		IsShowTrigArrow(int nPlotIndex=0);
	// 设置触发箭头绘制
	CChart_API void		SetShowTrigArrow(bool show, int nPlotIndex=0);
	// 显示水平光标线，仅仅用于模拟示波器
	CChart_API void		SetShowHCursor(bool show, int nPlotIndex=0);
	// 显示竖直光标线，仅仅用于模拟示波器
	CChart_API void		SetShowVCursor(bool show, int nPlotIndex=0);
	// 获得光标线颜色，仅仅用于模拟示波器
	CChart_API COLORREF	GetCursorColor(int nPlotIndex=0);
	// 设置光标线颜色，仅仅用于模拟示波器
	CChart_API void		SetCursorColor(COLORREF color, int nPlotIndex=0);
	// 获得光标线线宽，仅仅用于模拟示波器
	CChart_API int			GetCursorWidth(int nPlotIndex=0);
	// 设置光标线线宽，仅仅用于模拟示波器
	CChart_API void		SetCursorSize(int size, int nPlotIndex=0);
	// 获得光标线线型，仅仅用于模拟示波器
	CChart_API int			GetCursorSize(int nPlotIndex=0);
	// 设置光标线线型，仅仅用于模拟示波器
	CChart_API void		SetCursorStyle(int style, int nPlotIndex=0);
	// 获得水平光标值，仅仅用于模拟示波器
	CChart_API double		GetCursorX(int nPlotIndex=0);
	// 设置水平光标值，仅仅用于模拟示波器
	CChart_API void		SetCursorX( double cursor, int nPlotIndex=0);
	// 获得竖直光标值，仅仅用于模拟示波器
	CChart_API double		GetCursorY(int nPlotIndex=0);
	// 设置竖直光标值，仅仅用于模拟示波器
	CChart_API void		SetCursorY( double cursor, int nPlotIndex=0);
	// 显示水平选择区，仅仅用于模拟示波器
	CChart_API void		SetShowHSel(bool show, int nPlotIndex=0);
	// 显示竖直选择区，仅仅用于模拟示波器
	CChart_API void		SetShowVSel(bool show, int nPlotIndex=0);
	// 获得水平选择区的下限，仅仅用于模拟示波器
	CChart_API double		GetHSelLower(int nPlotIndex=0);
	// 设置水平选择区的下限，仅仅用于模拟示波器
	CChart_API void		SetHSelLower(int val, int nPlotIndex=0);
	// 获得水平选择区的上限，仅仅用于模拟示波器
	CChart_API double		GetHSelUpper(int nPlotIndex=0);
	// 设置水平选择区的上限，仅仅用于模拟示波器
	CChart_API void		SetHSelUpper(int val, int nPlotIndex=0);
	// 获得竖直选择区的下限，仅仅用于模拟示波器
	CChart_API double		GetVSelLower(int nPlotIndex=0);
	// 设置竖直选择区的下限，仅仅用于模拟示波器
	CChart_API void		SetVSelLower(int val, int nPlotIndex=0);
	// 获得竖直选择区的上限，仅仅用于模拟示波器
	CChart_API double		GetVSelUpper(int nPlotIndex=0);
	// 设置竖直选择区的上限，仅仅用于模拟示波器
	CChart_API void		SetVSelUpper(int val, int nPlotIndex=0);
	// 获得水平选择区的宽度，仅仅用于模拟示波器
	CChart_API double		GetHSelWidth(int nPlotIndex=0);
	// 获得竖直选择区的宽度，仅仅用于模拟示波器
	CChart_API double		GetVSelWidth(int nPlotIndex=0);
	
	// 增加坐标轴，坐标轴的位置location见本文件头部被注释掉的enum
	CChart_API void		AddAxis(int location, int nPlotIndex=0);
	// 设置子图坐标轴标题
	CChart_API void		SetAxisTitle(const TCHAR* title, int location, int nPlotIndex);
	// 设置坐标轴标题
	CChart_API void		SetAxisTitle(const TCHAR* title, int location);
	// 设置子图坐标轴标题字体
	CChart_API void		SetAxisTitleFont(LOGFONT logFont, int location, int nPlotIndex);
	// 设置坐标轴标题字体
	CChart_API void		SetAxisTitleFont(LOGFONT logFont, int location);
	// 设置子图坐标轴标签字体
	CChart_API void		SetAxisLabelFont(LOGFONT logFont, int location, int nPlotIndex);
	// 设置坐标轴标签字体
	CChart_API void		SetAxisLabelFont(LOGFONT logFont, int location);
	// 设置子图坐标轴标题位置
	CChart_API void		SetAxisTitlePosition(int position, int location, int nPlotIndex);
	// 设置坐标轴标题位置
	CChart_API void		SetAxisTitlePosition(int position, int location);
	// 子图自动设置坐标轴刻度数
	CChart_API void		SetAxisAutoTicks(bool bAuto, int location, int nPlotIndex);
	// 自动设置坐标轴刻度数
	CChart_API void		SetAxisAutoTicks(bool bAuto, int location);
	// 设置子图坐标轴刻度数
	CChart_API void		SetAxisTickCount(int count, int location, int nPlotIndex);
	// 设置坐标轴刻度数
	CChart_API void		SetAxisTickCount(int count, int location);
	// 子图自动设置坐标轴副刻度数
	CChart_API void		SetAxisAutoMinorTicks(bool bAuto, int location, int nPlotIndex);
	// 自动设置坐标轴副刻度数
	CChart_API void		SetAxisAutoMinorTicks(bool bAuto, int location);
	// 设置子图坐标轴副刻度数
	CChart_API void		SetAxisMinorTickCount(int count, int location, int nPlotIndex);
	// 设置坐标轴副刻度数
	CChart_API void		SetAxisMinorTickCount(int count, int location);
	// 设置子图时间坐标轴
	CChart_API void		SetAxisToTime(bool bTime, int location, int nPlotIndex);
	// 设置时间坐标轴
	CChart_API void		SetAxisToTime(bool bTime, int location);
	// 设置子图时间坐标轴
	CChart_API void		SetAxisTimeFormat(TCHAR *format, int location, int nPlotIndex);
	// 设置时间坐标轴
	CChart_API void		SetAxisTimeFormat(TCHAR *format, int location);
	// 设置子图显示颜色棒
	CChart_API void		SetAxisColorBar(bool bShow, int location, int nPlotIndex);
	// 设置显示颜色棒
	CChart_API void		SetAxisColorBar(bool bShow, int location);
	// 设置子图坐标轴颜色
	CChart_API void		SetAxisColor(COLORREF color, int location, int nPlotIndex);
	// 设置坐标轴颜色
	CChart_API void		SetAxisColor(COLORREF color, int location);
	// 设置坐标轴是否显示刻度值
	CChart_API void		SetAxisLabelShow(bool bShow, int location);
	// 设置子图坐标轴是否显示刻度值
	CChart_API void		SetAxisLabelShow(bool bShow, int location, int nPlotIndex);
	// 设置子图坐标值的格式,参考printf的格式化方法
	CChart_API void		SetAxisLabelFormat(TCHAR *format, int location, int nPlotIndex);
	// 设置坐标值的格式,参考printf的格式化方法
	CChart_API void		SetAxisLabelFormat(TCHAR *format, int location);
	// 设置子图坐标轴浮动标记
	CChart_API void		SetXAxisFloatTicks(bool flt, int nPlotIndex);
	// 设置坐标轴浮动标记
	CChart_API void		SetXAxisFloatTicks(bool flt);
	// 设置子图坐标轴浮动标记
	CChart_API void		SetYAxisFloatTicks(bool flt, int nPlotIndex);
	// 设置坐标轴浮动标记
	CChart_API void		SetYAxisFloatTicks(bool flt);
	// 设置坐标轴是否显示
	CChart_API void		SetAxisShow(bool bShow, int location);
	// 设置子图坐标轴是否显示
	CChart_API void		SetAxisShow(bool bShow, int location, int nPlotIndex);
	//坐标轴是否显示
	CChart_API bool		IsAxisShow(int location);
	// 子图坐标轴是否显示
	CChart_API bool		IsAxisShow(int location, int nPlotIndex);
	// 设置坐标轴成指数
	CChart_API void		SetAxisLogarithm(bool bLog, int Location);
	// 设置子图坐标轴成指数
	CChart_API void		SetAxisLogarithm(bool bLog, int Location, int nPlotIndex);
	// 设置坐标轴紧凑模式
	CChart_API void		SetAxesCompact(int nPlotIndex=0);
	// 添加隐藏坐标轴标签的一个数据范围
	CChart_API void		AddRange4LabelHiden(double low, double high, int location);
	// 添加子图隐藏坐标轴标签的一个数据范围
	CChart_API void		AddRange4LabelHiden(double low, double high, int location, int nPlotIndex);
	// 设置坐标横轴两端是某数的倍数
	CChart_API void		SetAtomX(double atom);
	// 设置子图坐标横轴两端是某数的倍数
	CChart_API void		SetAtomX(double atom, int nPlotIndex);
	// 设置坐标纵轴两端是某数的倍数
	CChart_API void		SetAtomY(double atom);
	// 设置子图坐标纵轴两端是某数的倍数
	CChart_API void		SetAtomY(double atom, int nPlotIndex);
	// 在设置坐标范围是某数倍数情况下，坐标横轴分割份数
	CChart_API void		SetSegsPreferX(int segs);
	// 在设置坐标范围是某数倍数情况下，子图坐标横轴分割份数
	CChart_API void		SetSegsPreferX(int segs, int nPlotIndex);
	// 在设置坐标范围是某数倍数情况下，坐标纵轴分割份数
	CChart_API void		SetSegsPreferY(int segs);
	// 在设置坐标范围是某数倍数情况下，子图坐标纵轴分割份数
	CChart_API void		SetSegsPreferY(int segs, int nPlotIndex);

	// 检测是否使用图例
	CChart_API bool		IsUseLegend(int nPlotIndex=0);
	// 设置是否使用图例
	CChart_API void		SetUseLegend(bool bUse, int nPlotIndex=0);
	// 设置图例的最大行数
	CChart_API void		SetLegendMaxRows(int nRows, int nPlotIndex=0);
	// 设置每行显示的图例数
	CChart_API void		SetLegendMaxCols(int nCols, int nPlotIndex=0);
	// 设置图例位置
	CChart_API void		SetLegendPosition(int nPos, int nPlotIndex=0);
	// 设置图例字体
	CChart_API void		SetLegendFont(LOGFONT logFont, int nPlotIndex=0);
	// 设置图例字体高度
	CChart_API void		SetLegendFontHeight(int nHeight, int nPlotIndex=0);

	// 设置坐标轴范围
	CChart_API void		SetPlotRange(double xRange[2], double yRange[2]);
	CChart_API void		SetXRange(double low, double high, int nPlotIndex);
	CChart_API void		SetXRange(double low, double high);
	CChart_API void		SetYRange(double low, double high, int nPlotIndex);
	CChart_API void		SetYRange(double low, double high);
	// 设置坐标轴范围自动计算
	CChart_API void		SetXAutoRange(bool bAuto);
	CChart_API void		SetYAutoRange(bool bAuto);
	CChart_API void		SetXAutoRange(bool bAuto, int nPlotIndex);
	CChart_API void		SetYAutoRange(bool bAuto, int nPlotIndex);
	// 设置坐标轴范围与数据范围相同，不向两端扩展
	CChart_API void		SetExactXRange(bool bExact, int nPlotIndex=0);
	CChart_API void		SetExactYRange(bool bExact, int nPlotIndex=0);
	// 设置坐标轴范围尽量与数据范围相同
	CChart_API void		SetOptimalExactXRange(bool bExact, int nPlotIndex=0);
	CChart_API void		SetOptimalExactYRange(bool bExact, int nPlotIndex=0);
	// 设置坐标轴范围尽量向两端扩展一格
	CChart_API void		SetOptimalXExtend(bool bExtend, int nPlotIndex=0);
	CChart_API void		SetOptimalYExtend(bool bExtend, int nPlotIndex=0);

	// 设置坐标轴等长，只对kTypeXY有效
	CChart_API void		SetEqualXYAxis(bool bEqual);
	// 设置坐标轴等范围，只对kTypeXY有效
	CChart_API void		SetEqualXYRange(bool bEqual);
	// 设置坐标轴等比例尺，只对kTypeXY有效
	CChart_API void		SetEqualXYScale(bool bEqual);

	// 设置图像边框显示
	CChart_API void		SetEdgeShow(bool bShow);
	// 设置图像边框显示
	CChart_API void		SetEdgeRoundConor(bool bRound);
	// 设置图像边框颜色
	CChart_API void		SetEdgeColor(COLORREF color);
	// 设置图像边框宽度
	CChart_API void		SetEdgeWidth(int width);

	// 设置数据区边框显示
	CChart_API void		SetBorderShow(bool bShow);
	// 设置数据区边框颜色
	CChart_API void		SetBorderColor(COLORREF color);
	// 设置数据区边框宽度
	CChart_API void		SetBorderWidth(int width);

	// 设置饼图单个饼的标题
	CChart_API void		SetPieTitle(const TCHAR* title, int which);
	// 设置饼图单个饼的颜色
	CChart_API void		SetPieColor(COLORREF color, int which);

	// 设置柱图单根柱子的标签
	CChart_API void		SetStemLabel(const TCHAR* label, int which);
	// 设置柱图单个序列的标题
	CChart_API void		SetStemTitle(const TCHAR* title, int nDataIndex);
	// 设置柱图单根柱子所占比例
	CChart_API void		SetStemRatio(double ratio);
	// 设置柱图从那条坐标轴开始画
	CChart_API void		SetStemBase(int base);

	// 设置等高线图等高线的数量
	CChart_API void		SetContourLineNum(int num);
	// 设置等高线图等高线绘制的精度，n越大，精度越高，一般5就可以了
	CChart_API void		SetContourPrecision(int n);
	// 设置显示等高线数值
	CChart_API void		SetContourValueShow(bool show);

	// 设置云图的起始颜色
	CChart_API void		SetContourMapColor1(COLORREF cr);
	// 设置云图的最终颜色
	CChart_API void		SetContourMapColor2(COLORREF cr);

	// 设置由数据点输入，而不是场函数
	CChart_API void		SetContourByPoints();

	// 设置背景网格与哪一个子图绑定
	// 用于共享X轴视图
	CChart_API void		SetGridBindLayer(int nPlotIndex);
	// 检测子图的网格线
	CChart_API bool		GetGridLine(bool &MajorH, bool &MajorV, bool &MinorH, bool &MinorV, int nPlotIndex);
	// 检测网格线
	CChart_API bool		GetGridLine(bool &MajorH, bool &MajorV, bool &MinorH, bool &MinorV);
	// 设置子图网格线的绘制
	CChart_API void		SetGridLine(bool MajorH, bool MajorV, bool MinorH, bool MinorV, int nPlotIndex);
	// 设置网格线的绘制
	CChart_API void		SetGridLine(bool MajorH=true, bool MajorV=true, bool MinorH=false, bool MinorV=false);
	// 设置主网格线颜色
	CChart_API void		SetMajorGridColor(COLORREF color);
	// 设置子图主网格线颜色
	CChart_API void		SetMajorGridColor(COLORREF color, int nPlotIndex);
	// 设置副网格线颜色
	CChart_API void		SetMinorGridColor(COLORREF color);
	// 设置子图副网格线颜色
	CChart_API void		SetMinorGridColor(COLORREF color, int nPlotIndex);
	// 设置主网格线线宽
	CChart_API void		SetMajorGridLineSize(int nSize);
	// 设置子图主网格线线宽
	CChart_API void		SetMajorGridLineSize(int nSize, int nPlotIndex);
	// 设置副网格线线宽
	CChart_API void		SetMinorGridLineSize(int nSize);
	// 设置子图副网格线线宽
	CChart_API void		SetMinorGridLineSize(int nSize, int nPlotIndex);
	// 设置主网格线线型
	CChart_API void		SetMajorGridLineStyle(int nStyle);
	// 设置子图主网格线线型
	CChart_API void		SetMajorGridLineStyle(int nStyle, int nPlotIndex);
	// 设置副网格线线型
	CChart_API void		SetMinorGridLineStyle(int nStyle);
	// 设置子图副网格线线型
	CChart_API void		SetMinorGridLineStyle(int nStyle, int nPlotIndex);
	// 获取子图背景颜色
	CChart_API COLORREF	GetBkgndColor(int nPlotIndex);
	// 获取背景颜色
	CChart_API COLORREF	GetBkgndColor();
	// 设置子图背景颜色
	CChart_API void		SetBkgndColor(COLORREF color, int nPlotIndex);
	// 设置背景颜色
	CChart_API void		SetBkgndColor(COLORREF color);
	// 背景色是否渐变
	CChart_API bool		IsGradientBkgnd();
	// 背景色渐变
	CChart_API void		SetGradientBkgnd(bool bGrad);

	// 保存子图数据到文件
	CChart_API bool		WriteToFile(TCHAR *pathName, int nPlotIndex);
	// 保存子图某曲线数据到文件
	CChart_API bool		WriteToFile(TCHAR *pathName, int nDataIndex, int nPlotIndex);

	// 设置内置缩放模式，打开后可响应鼠标拖放
	CChart_API void		SetZoomModeBuildIn(bool zoom);
	// 设置图像整体缩放模式，只是绘制，适用于不采用CChartWnd的情况
	CChart_API void		SetZoomMode(bool zoom);
	// 设置图像整体缩放时X方向的缩放系数
	CChart_API void		SetZoomFactorX(double fact);
	// 设置图像整体缩放时Y方向的缩放系数
	CChart_API void		SetZoomFactorY(double fact);
	// 设置图像整体缩放时两个方向的缩放系数
	CChart_API void		SetZoomFactor(double fact);
	// 设置图像整体缩放时的缩放中心，中心的代号见本文件开始被注释掉的enum
	CChart_API void		SetZoomCenterMode(int center);

	// 设置三维曲线的网格
	CChart_API void		SetShowXYMajorGrid( bool show );
	CChart_API void		SetShowYZMajorGrid( bool show );
	CChart_API void		SetShowZXMajorGrid( bool show );
	CChart_API void		SetShowXYMinorGrid( bool show );
	CChart_API void		SetShowYZMinorGrid( bool show );
	CChart_API void		SetShowZXMinorGrid( bool show );
	CChart_API void		SetGridLine(bool MajorXY, bool MinorXY, bool MajorYZ, bool MinorYZ, bool MajorZX, bool MinorZX);

	// 在分层视图和分裂视图中，设置第一层显示左坐标轴，第二层显示右坐标轴
	CChart_API void		SetLRAxis();
	// 在分层视图中，设置第一层显示下坐标轴，第二层显示上坐标轴
	CChart_API void		SetTBAxis();
	// 底层函数，获取子图的绘图区域
	CChart_API RECT		GetPlotRect(int nPlotIndex);
	// 底层函数，获取绘图区域
	CChart_API RECT		GetPlotRect();
	// 底层函数，获取坐标轴的范围
	CChart_API void		GetPlotRange(double xRange[2], double yRange[2], int nPlotIndex=0);
	// 底层函数，获取数据的范围
	CChart_API void		GetDataRange(double xRange[2], double yRange[2], int nPlotIndex=0);

	// 获得内部特殊指针，此指针一般没用，只用于在某些情况下传递参数，例如线程函数
	CChart_API void		*GetUserPointer();
	// 设置内部特殊指针
	CChart_API void		SetUserPointer(void *pUser);
	// 获得内部特殊标志状态，特殊用途
	CChart_API bool		GetUserFlag();
	// 设置内部特殊标志状态，特殊用途
	CChart_API void		SetUserFlag(bool flag);
}

#ifdef CChart_API
#	undef CChart_API
#endif //CChart_API

#endif //__CLASSLESS_H_122333444455555__
