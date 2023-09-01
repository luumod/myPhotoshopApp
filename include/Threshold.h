﻿#ifndef THRESHOLD_H_
#define THRESHOLD_H_

#include "Object.h"
#include <QWidget>
#include <opencv2/opencv.hpp>
class QImage;

class Threshold :public Object {
public:
	Threshold();
	~Threshold();

	void threshold();
	void initialize()override;
public slots:
	void onTriggered_slider1_valueChanged_thresholdValue(int value);
	void onTriggered_slider2_valueChanged_maxValue(int value);

	void onReturnPressed_Edit(QList<QString> strs);
public:
	//阈值 最大值
	int threshold_value = 128, maxVal = 255;
	int current_choice = -1;
};


#endif