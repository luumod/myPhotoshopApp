﻿/**
 * @file widget.h
 * @brief 程序主窗口文件
 * @author yuleo (lyu198691@gmail.com)
 * @version 1.0
 * @date 2023-10-15
 *
 * @copyright Copyright (c) 2023 Yuleo
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2023-10-15 <td>1.0     <td>Yuleo     <td>主程序文件
 * </table>
 */
#ifndef WIDGET_H_
#define WIDGET_H_
#define FUNCTION_
#include "assist/config.h"
#include <QMainWindow>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <stack>
#include <QDebug>
#include <functional>
#if 1
class QAbstractButton;
class QButtonGroup;
class QGridLayout;
class QToolButton;
class QToolBox;
class QLabel;	//主图片
class Sub_widget;	//预览图片
class QLabel;
class QAction;
class QStackedWidget;
class QVBoxLayout;
class Object;
class Blur;
class Threshold;
class Morphology;
class Connected;
class Contours;
class QToolBar;
class QFileDialog;
class QColorDialog;
class CvtColor;
class QActionGroup;
class QHBoxLayout;
class DrawWidget;
class BaseOperate;
class QMouseEvent;
class Showeffect;
class QScrollArea;
class QPushButton;
class QCheckBox;
class QWheelEvent;
class LookWidget;
class Res;
class CaptureWidget;
class QBoxLayout;
class QSlider;
class QComboBox;
#endif
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
/**
 * @class Widget
 * @brief 程序主窗口。
 *
 * 它的实现运用了单例设计模式，在整个程序中只有一个Widget实例。
 * 它的全部属性都是公开的，保证整个程序只有一个实例。
 * 在处理对图片的任何操作时，都可以使用getInstance函数（通常简化为get()）直接获取其实例。例如：
 * @li get()->inter_mt：单次操作下对图片的修改。
 * @li get()->flash_mt：混合模式下对快照图片的修改。
 * 并且使用get因此Widget类具有很高的扩展性与简易性。
 *
 * @see Object
 */
class Widget :public QMainWindow {
    Q_OBJECT
private:
    static Widget* widget; /// < 唯一的单例对象

    /**
     * @brief Construct a new Widget object
     * @param  parent 父对象
     */
    Widget(QWidget* parent = nullptr);
public:
    /**
     * @brief Get the Instance object
     * @return Widget* (static)
     */
    static Widget* getInstance();

    /**
     * @brief Destroy the Widget object
     */
    ~Widget();
public:

    /**
     * @brief 初始化MainWindow窗口的基本信息
     */
    void init_WidgetInfo();

    /**
     * @brief 解析json配置文件，此文件为程序的基本配置文件
     */
    void init_readJson();


    /**
     * @brief 初始化两个Label；一个是原始的Label，另一个是操作Label。可以选择隐藏第一个Label，只显示第二个Label
     */
    void init_Label();

    /**
     * @brief 初始化所有OpenCV操作的主对象
     * @li Blur 模糊操作
     * @li Threshold 阈值化操作
     * @li Morphology 形态学操作
     * @li Connected 连通性分析
     * @li Contours 轮廓检测
     * @li Showeffect 图像增强操作
     * @li BaseOperate 图像基础操作
     */
    void init_OpencvFunctions();


    void init_GroupBoxCutImage();
    void init_GroupBoxGUIAdjust();

    /**
     * @brief 这是一个特化函数，具体函数功能请查阅下方 "参见"
     * @see update_Image
     */
    void update_Image_1(double f_scaledDelta);

    /**
     * @brief 根据 "缩放比例" 缩放图片为指定大小。
     * @param  f_scaledDelta 缩放比例
     * @li f_scaledDelta = ori_scaledDelta，则呈现完美比例（即正好容纳下整个QLabel控件）
     * @li f_scaledDelta为其他数值，则为自定义缩放比例。
     *
     * @see 调用示例片段：
     * @code{.cpp}
     * update_Image(0.5); //更新操作图片为 0.5倍，即缩小了一倍。
     * update_Image(2);	//更新操作图片为 2倍，即扩大了一倍。
     * update_Image(ori_scaledDelta); //更新操作图片为 "完美缩放比例" ，即正好容纳QLabel的窗口
     * @endcode
     */
    void update_Image(double f_scaledDelta);

    /**
     * @brief 计算图片的缩放比例，并且保持宽高比的铺满整个QLabel。
     *
     * 例如在程序刚刚打开或者打开一个新图片的时候，调用此函数来计算出能够容纳QLabel大小的最大图片比例，并且更新为此 "完美比例"，使得图片可以保持宽高比的铺满整个QLabel。
     * 为什么不在QLabel中使用自动缩放？ 因为每个图片的尺寸是不同的，我暂时无法为不同尺寸的图片定义一个统一的展示规则。
     *
     * @return double 返回完美缩放比例，并且赋值为ori_scaledDelta，使得其保留。
     */
    double init_scaledImageOk();
signals:
    /**
     * @brief 当处于 “单图片模式” 的情况下触发此信号
     *
     * @li number = 1：单图片
     * @li number = 2：双图片
     * “单图片模式” 为用户选择隐藏原始图片，即切换到 “专注模式”，只需要专注对于目标图片的操作。
     */
    void signal_singleImageMode(int number);

    /**
     * @brief 当处于 “双图片模式” 的情况下触发此信号
     *
     * “双图片模式” 为用户选择显示原始图片，即切换到 “对比模式”，用户可以选择与原始图片进行对比。
     */
    void signal_doubleImageMode();

    /**
     * @brief 当改变状态的情况下触发此信号
     * @param  st 可以为以下两种状态：
     * @li false：表示当前处于“普通操作模式”，下一次的操作会覆盖当前操作。
     * @li true：表示当前处于“混合加工模式”，下一次的操作会在当前操作后的基础上继续进行。
     */
    void signal_changeMode(bool st);

    /**
     * @brief 以不同方式打开图片时触发此信号
     * @li on_action_open_triggered：从文件夹中选择单张图片。
     * @li on_action_works_triggered：打开整个文件夹的所有图片，相当于打开了一个工作区。
     */
    void signal_changeTo_FileOrWork();

    /**
     * @brief 在某个ToolBox页中，选择一个按钮的时候触发此信号。
     * @param  optName 表示此时某个具体操作的名字
     */
    void signal_choiceToolButton(const QString& optName = "",int id = -1);
protected:
    /**
     * @brief 窗口移动：移动窗口获取当前左上角坐标，保存在config.json配置文件中，使得下次打开程序位置不变。
     * @param  ev 窗口移动事件
     */
    void moveEvent(QMoveEvent* ev)override;

    /**
     * @brief 鼠标滑轮移动：Ctrl+滚轮实现缩放图片
     * @param  ev 滚轮移动事件
     */
    void wheelEvent(QWheelEvent* ev)override;

    /**
     * @brief 窗口尺寸改变事件：使得窗口保持一个好看的尺寸。
     * @param  ev 尺寸改变事件
     *
     * @todo 引入此函数的目的是当 “对比模式” 与 “专注模式” 之间窗口的布局进行切换的时候，尺寸可能会发生变化，因此 “强行” 调用此函数来恢复原始尺寸
     */
    void resizeEvent(QResizeEvent* ev)override;
public:
    /**
     * @brief 创建程序中所有的行为
     */
    void createAction();

    /**
     * @brief 创建程序的菜单项
     */
    void createMenu();

    /**
     * @brief 创建程序的工具栏
     */
    void createToolBar();

    /**
     * @brief 创建程序左侧 OptArea 区域
     */
    void createToolBox();

    /**
     * @brief 创建程序的底部状态栏
     */
    void createStatusBar();

    /**
     * @brief 创建 Blur 操作中“均值模糊”的参数调整栏。其调整栏布局如下：
     * @li QSlider: 控制均值模糊的Kernel大小
     * @li QSlider: 控制均值模糊的anchorX偏移
     * @li QSlider: 控制均值模糊的anchorY偏移
     * @return QWidget*： 表示上述三个QSlider组成的窗口
     */
    QWidget* create_GUIAvgBlur();


    /**
     * @brief 创建 Blur 操作中“高斯模糊”的参数调整栏。其调整栏布局如下：
     * @li QSlider: 控制高斯模糊的Kernel大小
     * @li QSlider: 控制高斯模糊的anchorX偏移
     * @li QSlider: 控制高斯模糊的anchorY偏移
     * @return QWidget*： 表示上述三个QSlider组成的窗口
     */
    QWidget* create_GUIGaussian();

    /**
     * @brief 创建 Blur 操作中“中值模糊”的参数调整栏
     * @li QSlider: 控制中值模糊的Kernel大小
     */
    QWidget* create_GUIMedian();


    /**
     * @brief 创建 Blur 操作中“双边滤波”的参数调整栏。其调整栏布局如下：
     * @li QSlider: 控制双边滤波的Kernel大小
     * @li QSlider: 控制双边滤波的sigmaColor大小
     * @li QSlider: 控制双边滤波的sigmaSpace大小
     * @return QWidget*： 表示上述三个QSlider组成的窗口
     */
    QWidget* create_GUIBilateral();

    /**
     * @brief 创建 Threshold 的参数调整栏。其调整栏布局如下：
     * @li QSlider: 控制阈值化操作的阈值具体值。
     * @li QSlider: 控制阈值化操作的阈值最大值。
     * @return QWidget*： 表示上述两个QSlider组成的窗口
     */
    QWidget* create_GUIThreshold();

    /**
     * @brief 创建 Morphology 的参数调整栏。其调整栏布局如下：
     * @li QSlider: 控制形态学操作的Kernel大小
     * @li QSlider: 控制形态学操作的anchor偏移
     * @li QSlider: 控制形态学操作的anchor偏移
     * @return QWidget*： 表示上述三个QSlider组成的窗口
     */
    QWidget* create_GUIMorphology();

    /**
     * @brief 创建 Connected 的参数调整栏。其调整栏布局如下：
     * @li QComboBox: 控制连通区域检测的邻域
     * @li QComboBox: 控制连通区域检测的输出图深度
     * @return QWidget*： 表示上述两个QComboBox组成的窗口
     */
    QWidget* create_GUIConnected();

    /**
     * @brief 创建 Contours 的参数调整栏。
     */
    QWidget* create_GUIContours();

    /**
     * @brief 创建 Showeffect 中”亮度“的参数调整栏。
     */
    QWidget* create_GUIbright();

    /**
     * @brief 创建 Showeffect 中”gamma变换“的参数调整栏。
     */
    QWidget* create_GUIgamma();

    /**
     * @brief 创建 Showeffect 中”对比度线性展宽“的参数调整栏。
     */
    QWidget* create_GUIContrast();

    /**
     * @brief 创建 Showeffect 中”灰级窗处理“的参数调整栏。
     */
    QWidget* create_GUIGrayWindow();

    /**
     * @brief 创建 Showeffect 中”线性动态范围调整“的参数调整栏。
     */
    QWidget* create_GUIDPLinear();

    /**
     * @brief 创建 Showeffect 中”非线性动态范围调整“的参数调整栏。
     */
    QWidget* create_GUINoneDpLinear();

    /**
     * @brief 从以下的窗口中选择一个GUI窗口并且创建
     * @li create_GUIAvgBlur
     * @li create_GUIGaussian
     * @li create_GUIMedian
     * @li create_GUIBilateral
     * @li create_GUIThreshold
     * @li ...
     * @li create_GUINoneDpLinear
     * @param  id 某个操作对应的id，根据这个id来选择对应的窗口然后创建。
     * @todo 实际上这个id是无意义的，因为我直接全部for循环添加了，这个操作可能会占用较长的时间，我希望可以用户选择时添加，而不是一开始就全部添加，但是目前没有找到合适的解决办法。
     * @return QWidget*：表示返回的窗口
     */
    QWidget* choice_GUI_create(int id);


    /**
     * @brief  响应某个具体操作的槽函数。
     *
     * 对于所有的opencv操作按钮，封装为此一个函数，此函数可以用于传递指定的op操作，传递对应op操作的按钮组，传递当前按下的按钮：即可完成对于指定行为的响应。
     * @param  op       		抽象操作类型：一个Object*类型的对象
     * @param  btn_group        抽象操作按钮组：Blur Threshold Morphology等不同的抽象操作都包含多个具体的操作的ToolButton
     * @param  btn              在按钮组中按下的某个ToolButton按钮
     *
     * 调用片段：
     * @code{.cpp}
     * //连接信号
     *	connect(btngroup_blur, &QButtonGroup::buttonClicked, this, [=](QAbstractButton* btn) {
     *		Object* op = blur;
     *		on_buttonGroup_everyOpeartions_choice(op,btngroup_blur,btn);
     *	});
     * @endcode
     */
    void on_buttonGroup_everyOpeartions_choice(Object* op, QButtonGroup* btn_group, QAbstractButton* btn);

    /**
     * @brief 原图片的右键菜单
     *
     * 菜单项如下：
     * @li 隐藏原图片，即转换为”专注模式“
     * @li 退出程序
     * @param  pos              鼠标的点击位置
     *
     * @see context_menu__
     */
    void on_label_customContextMenuRequested__(const QPoint& pos);

    /**
     * @brief 目标操作图片的右键菜单
     *
     * 菜单项如下：
     * @li 只有在”专注模式“下：显示原图片，切换为 ”对比模式“。
     * @li 保存图片
     * @li 退出程序
     * @li 重置所有操作，恢复至原始图片
     * @li 显示图片信息（已废弃，待移除）
     * @param  pos          	鼠标的点击位置
     *
     * @see context_menu
     */
    void on_label_customContextMenuRequested(const QPoint& pos);

    //选择颜色
    /**
     * @brief 响应进行轮廓操作时，切换颜色时也应当进行轮廓检测。
     *
     * 此函数不应该被当作是一个正式的action行为，因为它是一个colorDialog的槽函数。
     *
     * @param  color            当前选择的颜色，对应信号：QColorDialog::currentColorChanged(color)
     *
     * @see colorDialog
     */
    void on_colorDialog_triggered(const QColor& color);

    /**
     * @brief 响应不同主题之间的切换
     *
     * @param  type            表示主题代码
     * @li 0：白色主题
     * @li 1：黑色主题
     *
     * @see action_theme
     */
    void on_action_theme_triggered(int type);

    //切换图片的格式：LAB  HSV 等格式
    /**
     * @brief 响应切换图片的不同格式
     *
     * 目前仅支持：
     * @li BGR（默认）
     * @li LAB格式
     * @li HSV格式
     * @li HLS格式
     * @li RGB格式
     * @param  action           按钮组中被选择的某个按钮
     *
     * @see action_cvtColor_group
     */
    void on_action_cvtColor_group_triggered(QAction* action);

    /**
     * @brief 响应图片的不同旋转方式
     *
     * 例如：
     * @li 顺时针90度
     * @li 顺时针180度
     * @li 逆时针90度
     * @param  action           按钮组中被选择的某个按钮
     *
     * @see	action_rotate_group
     */
    void on_action_rotate_group_triggered(QAction* action);

    /**
     * @brief 响应图片的不同翻转方式
     *
     * 例如：
     * @li 垂直翻转
     * @li 水平翻转
     * @li 水平垂直翻转
     * @param  action           按钮组中被选择的某个按钮
     *
     * @see action_flip_group
     */
    void on_action_flip_group_triggered(QAction* action);

    /**
     * @brief 响应帮助菜单
     *
     * @param  action           按钮组中被选择的某个按钮
     *
     * @see	actionGroupHelp
     */
    void on_actionGroupHelp_triggered(QAction* action);
public slots:
    /**
     * @brief on_action_hide_triggered
     *
     * @see action_hide
     */
    void on_action_hide_triggered();

    /**
     * @brief on_action_disp_triggered
     */
    void on_action_disp_triggered();

    /**
     * @brief 响应关闭窗口
     *
     * @see action_exit
     */
    void on_action_exit_triggered();

    /**
     * @brief 响应从文件夹中打开一张新图片
     *
     * @see action_open
     */
    void on_action_open_triggered();

    /**
     * @brief 响应选择整个文件夹中的所有图片作为一个工作区
     *
     * @see action_works
     */
    void on_action_works_triggered();

    /**
     * @brief 保存当前图片到文件夹
     *
     * @see action_save
     */
    void on_action_save_triggered();

    //重置所有操作至原始状态
    /**
     * @brief 重置所有
     *
     * @see action_restore
     */
    void on_action_restore_triggered();

    /**
     * @brief 响应打开绘图板
     *
     * @see action_draw
     */
    void on_action_draw_triggered();

    //开启图片加工模式

    /**
     * @brief 响应打开图片混合加工模式
     *
     * @see action_begin
     */
    void on_action_begin_triggered();

    /**
     * @brief 响应撤销当前操作，返回到上一次保存时的操作
     *
     * @see action_return
     */
    void on_action_return_triggered();


    /**
     * @brief 响应图片的截取功能
     *
     * @see	action_jie
     */
    void on_action_jie_triggered();

    /**
     * @brief 响应桌面截图功能
     *
     * @see action_capture
     */
    void on_action_capture_triggered();

    /**
     * @brief 响应按钮的下一页切换操作
     *
     * 用于在工作区中切换图片
     *
     * @see btn_work_next
     */
    void on_btn_work_next_clicked();

    /**
     * @brief 响应按钮的上一页切换操作
     *
     * 用于在工作区中切换图片
     *
     * @see btn_work_prev
     */
    void on_btn_work_prev_clicked();

    //工作区：自动保存
    /**
     * @brief 响应图片的自动保存操作
     *
     * 用于在工作区中切换图片时，自动保存上一页的图片
     *
     * @see cbx_work_autoSave
     */
    void on_cbx_work_autoSave_clicked();
public:
    //加载/重新加载图片资源并且重置场景
    //
    /**
     * @brief 加载图片资源并且重置场景
     *
     * 实际上此函数需要完成的操作：
     * @li 加载或重置图片资源
     * @li 滑动区域图片的重置
     * @li 图片尺寸的缩放
     * @li 所有Opts操作及GUI等控件的数值重置
     *
     * @param  fileName        	新的图片资源的路径
     * @param  mode             根据不同的原因来切换图片资源
     * @li mode=0：加载单个图片
     * @li mode!=0：加载工作区
     *
     * @see scrollArea
     *
     */
    void reload_Resources_ScrollArea(const QString& fileName, int mode = 0);


    /**
     * @brief 打开工作区：从文件夹中加载所有的图片
     *
     * 为节省资源，保存的是每一个图片的路径。
     * 此函数应该是一个辅助函数，不应该在别处调用。
     *
     * @param  floderPath       工作区路径
     * @return true 工作区图片加载成功
     * @return false 工作区图片加载失败
     *
     * @see	work_files：保存所有图片的路径。
     */
    bool loadImagesFormFloder(const QString& floderPath);

    /**
     * @brief 非混合加工模式下：切换不同的操作时，清除原来的操作
     *
     * 每次都重置回原始图片
     *
     * @see updateFromIntermediate
     */
    void restore_cutOperation();

    /**
     * @brief 混合加工模式下：保存当前图片快照
     *
     * 便于实现撤销
     *
     * @see undo_sta
     */
    void savePoint();

    //
    /**
     * @brief 混合加工模式下：返回上一次图片快照
     *
     * 撤销操作
     *
     * @see undo_sta
     */
    void returnPoint();

    //依照inter_mt更新所有图片数据（不包括对图像格式的改变与旋转等基础操作造成的改变）
    /**
     * @brief 重置图片资源至中间层
     *
     * @see Res.inter_mt
     */
    void updateFromIntermediate();

    /**
     * @brief 重置图片资源至原始状态root
     *
     * 相当于清除所有操作
     *
     * @see Res.root_mt
     */
    void updateFromRoot();

private FUNCTION_: //辅助函数
                /**
     * @brief 创建OptArea区域中的每个具体操作按钮ToolButton
     *
     * 此函数是内部函数，不应该在外部被单独调用。
     *
     * @param  text             具体操作的名称
     * @param  id               指定的id
     * @param  fileName         图片资源的文件名称
     * @return QWidget*         具体按钮，其由ToolButton和一个QLabel组成
     */
                QWidget* createToolBtnItemWidget(const QString& text, int id, const QString& fileName = "");

    //创建每种对话框种的的输入数值方式
    /**
     * @brief 创建每个具体操作的数值手动输入方式
     *
     * 此函数是内部函数，不应该在外部被单独调用。
     *
     * @tparam Type 			抽象操作类型，如Blur，Threshold
     * 具有输入功能的操作应该实现 onReturnPressed_Edit
     *
     * @param  filter           数据验证器的过滤
     * @param  text            	占位提示文本
     * @param  t                抽象操作类型
     *
     *
     * @return QHBoxLayout* 返回一个水平布局，因为其通常是 输入框+按钮 的结构
     */
    template <typename Type>
    QHBoxLayout* create_Edit_hLayout(const QString& filter, const QString& text, Type* t);

    /**
     * @brief 工作区中切换图片的具体实现函数
     *
     * 此函数是内部函数，不应该在外部被单独调用。
     */
    void work_cutImage();

    //Dialog: 创建n个滑块和输入框
    /**
     * @brief 对于GUI窗口的封装：可以创建n个QSlider
     * @tparam T 				最小值，最大值，步长的数据类型
     * @tparam Type 			create_Edit_hLayout 的模板类型
     * @param  ls_slider       	窗口中所有的QSlider实体，把这些QSlider放到一个QList中进行管理
     * @param  low              n个QSlider分别对应的最小值
     * @param  high             n个QSlider分别对应的最大值
     * @param  step            	n个QSlider分别对应的步长
     * @param  objectName       n个QSlider分别对应的类名
     * @param  lab_name         n个QSlider分别对应的提示标签的名字
     * @param  slotFunction     n个QSlider分别对应槽函数，连接其 SliderMoved 信号
     * @param  edit            	true：表示允许有输入功能 ，false表示不允许输入
     * @param  filter          	参见 create_Edit_hLayout
     * @param  text             参见 create_Edit_hLayout
     * @param  t                参见 create_Edit_hLayout
     * @return QBoxLayout* 		返回的是包含n个QSlider的窗口布局
     *
     * @todo 实际上直接返回QWidget就行
     */
    template <typename T, typename Type>
    QBoxLayout* createDialog_nSlider_GUItemplate(
        QList<QSlider*>& ls_slider,
        QList<T> low, QList<T> high, QList<T> step,
        QList< QString> objectName,
        QList< QString> lab_name,
        QList<std::function<void(int)>> slotFunction,
        bool edit = false,
        const QString& filter = "",
        const QString& text = "",
        Type* t = nullptr);

    /**
     * @brief 对于GUI窗口的封装：可以创建n个QComboBox
     *
     * @see 具体内容请参见：createDialog_nSlider_GUItemplate 其实现基本一致
     */
    QBoxLayout* createDialog_nComBox_GUItemplate(
        QList<QComboBox*>& ls_combox,
        QList<QStringList> ls_item,
        QList< QString> objectName,
        QList< QString> lab_name,
        QList<std::function<void(int)>> slotFunction);
public:
    Ui::MainWindow *ui = nullptr;
    /**
     * @brief 配置文件
     */
    ExeConfig config;

    /**
     * @brief 管理整个程序的图片资源
     */
    Res* res = nullptr;

    /**
     * @brief 显示截取的图片的Widget
     *
     * @see on_action_jie_triggered
     */
    LookWidget* look = nullptr;

    /**
     * @brief 加工模式的标记
     *
     * @li true：混合加工模式
     * @li false: 普通模式
     *
     */
    bool mode = false;

    /**
     * @brief 记录图片铺满整个QLabel的完美缩放比例
     *
     * @see update_Image
     */
    double ori_scaledDelta = 1.0; //原始完美缩放比例

    /**
     * @brief 用于在任何时候修改图片的缩放比例
     *
     * 用于鼠标滑轮时缩放图片
     *
     * @see wheelEvent
     */
    double scaledDelta = 1.0;

public:
    //---------------------
    //opencv操作按钮组
    QButtonGroup* btngroup_blur = nullptr;
    QButtonGroup* btngroup_threshold = nullptr;
    QButtonGroup* btngroup_form = nullptr;
    QButtonGroup* btngroup_connected = nullptr;
    QButtonGroup* btngroup_contours = nullptr;
    QButtonGroup* btngroup_show = nullptr;

    /**
     * @brief 抽象操作的按钮组
     *
     * @see btngroup_blur btngroup_threshold btngroup_form btngroup_connected btngroup_contours btngroup_show
     *
     */
    QList<QButtonGroup*> btngroups;

    /**
     * @brief 抽象操作的具体操作类
     *
     * @see blur threshold morphology connected contours showeffect img_base
     *
     */
    Object* op = nullptr;

    BaseOperate* img_base = nullptr; //图像基础操作

    /**
     * @brief 具备撤销功能的撤销栈
     *
     * @see returnPoint savePoint
     *
     */
    std::stack<cv::Mat> undo_sta;

    /**
     * @brief 简单的绘图板
     *
     * @see on_action_draw_triggered
     */
    DrawWidget* widget_draw = nullptr;

    /**
     * @brief 存储打开的工作区中的所有的图片路径
     *
     * @see loadImagesFormFloder
     */
    QStringList	work_files; //打开工作区的图片名称组

    /**
     * @brief 用于实现工作区中切换图片
     *
     * @see on_pushButton_prev_clicked on_btn_work_next_clicked
     *
     */
    int work_currentIndex = 0, work_prevIndex = 0;
public: // GUI部分
    //---------------------
    QActionGroup* action_cvtColor_group = nullptr;

    QActionGroup* action_flip_group = nullptr;

    QActionGroup* action_rotate_group = nullptr;

    QActionGroup* actionGroupHelp = nullptr;

    //上下文菜单
    QMenu* context_menu__ = nullptr; //原图片的
    QMenu* context_menu = nullptr;	 //目标图片的

    QAction* action_theme = nullptr;

    int preToolBoxIndex = 0, curToolBoxIndex = 0; //切换toolbox页面时清除选择状态

    //---------------------
    //底部状态栏
    QLabel* statusLab = nullptr;

    //-------------------------
    //选择颜色
    QColorDialog* colorDialog = nullptr;

    QHBoxLayout* btn_work_layout;

    //桌面截图
    CaptureWidget* all_screen = nullptr;

    //gamma
    QList<QSlider*> ls_slider_blur;
    QList<QSlider*> ls_slider_gaussian;
    QList<QSlider*> ls_slider_median;
    QList<QSlider*> ls_slider_bilateral;
    QList<QSlider*> ls_slider_threshold;
    QList<QSlider*> ls_slider_morphology;
    QList<QComboBox*> ls_combox_connected;
    QList<QComboBox*> ls_combox_contours;
    QList<QSlider*> ls_slider_light;
    QList<QSlider*> ls_slider_gamma;
    QList<QSlider*> ls_slider_linear;
    QList<QSlider*> ls_slider_grayWindow;
    QList<QSlider*> ls_slider_dpLinear;
    QList<QSlider*> ls_slider_NoneDpLinear;
};


#endif
