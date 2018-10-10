/*
 * Copyright (c) 2014-2018 GMRV/URJC.
 *
 * Authors: Nadir Román Guerrero <nadir.ro.gue@gmail.com>
 *
 * This file is part of ReTo <https://gitlab.gmrv.es/nsviz/ReTo>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef __FRAMERECORDER_FRAMERECORDERWIDGET_H__
#define __FRAMERECORDER_FRAMERECORDERWIDGET_H__

#include <QWidget>
#include <QScreen>
#include <QToolButton>
#include <QImage>
#include <queue>
#include <mutex>
#include <vector>
#include <thread>
#include <condition_variable>

class QLineEdit;	// Text field to choose framerate

namespace reto
{
  class FrameRecorderWidget : public QWidget
  {
    Q_OBJECT

  private:
    QWidget * dataSource;
    QLineEdit * inputFrameRateEL;
    QLineEdit * fileWidthEL;
    QLineEdit * fileHeightEL;
    QToolButton * recordButton;
    QToolButton * stopRecordButton;

    char filename[0xff];
    QScreen * screen;
    std::queue<QImage> frameQueue;
    std::mutex mtx;
    std::condition_variable monitor;
    bool recording;
    int frameWidth, frameHeigth;
    double frameRate;
    std::vector<std::thread> threads;

  public:
    FrameRecorderWidget(QWidget * frameSource = 0, QWidget * parent = 0);
    void setFrameSource(QWidget * frameSource);

  private slots:
    void startRecordingClicked();
    void stopRecordingClicked();

  private:
    QToolButton * createButton(const QString & text, const char * slotMember);
    void produceFrame();
    void consumeFrame();
  };
}

#endif
