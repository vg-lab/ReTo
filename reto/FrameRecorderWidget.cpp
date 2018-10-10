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

#include <QtWidgets>
#include <QMessageBox>
#include <QFile>
#include <QScreen>
#include <QGuiApplication>
#include <QPixmap>
#include <QColor>

#include "FrameRecorderWidget.h"

#include <thread>

extern "C"{
#include <x264.h>
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include <libavutil/mathematics.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
}

#include <iostream>
#include <chrono>

namespace reto
{
	FrameRecorderWidget::FrameRecorderWidget(QWidget * frameSource, QWidget * parent)
		: QWidget(parent)
		, dataSource(frameSource)
		, recording(false)
	{
		inputFrameRateEL = new QLineEdit("30");
		inputFrameRateEL->setReadOnly(false);
		inputFrameRateEL->setAlignment(Qt::AlignRight);
		inputFrameRateEL->setMaxLength(3);
		inputFrameRateEL->setMaximumWidth(70);
		inputFrameRateEL->setInputMask(tr("009"));// Number digits, 000 - 999

		QLabel * frameRateLabel = new QLabel(tr("Recording frame rate"));

		fileWidthEL = new QLineEdit("1920");
		fileWidthEL->setAlignment(Qt::AlignRight);
		fileWidthEL->setMaxLength(4);
		fileWidthEL->setMaximumWidth(70);
		fileWidthEL->setInputMask(tr("0009"));
		
		//QLabel * fileSizeLabel = new QLabel(tr("Video dimensions"));
		//QLabel * multplLabel = new QLabel(tr("x"));

		fileHeightEL = new QLineEdit("1080");
		fileHeightEL->setAlignment(Qt::AlignRight);
		fileHeightEL->setMaxLength(4);
		fileHeightEL->setMaximumWidth(70);
		fileHeightEL->setInputMask(tr("0009"));
		
		recordButton = createButton(tr("Record"), SLOT(startRecordingClicked()));
		recordButton->setFixedSize(QSize(30,30));
		recordButton->setIcon(QIcon(QString::fromUtf8(":/icons/record_icon.png")));
		QSize currentIconSize = recordButton->iconSize();
		currentIconSize = currentIconSize / 1.5;
		recordButton->setIconSize(currentIconSize);

		stopRecordButton = createButton(tr("Stop"), SLOT(stopRecordingClicked()));
		stopRecordButton->setFixedSize(QSize(30,30));
		stopRecordButton->setIcon(QIcon(QString::fromUtf8(":/icons/stop_record_icon.png")));
		stopRecordButton->setIconSize(currentIconSize);
		stopRecordButton->setEnabled(false);

		QGridLayout * mainLayout = new QGridLayout;
		mainLayout->setSizeConstraint(QLayout::SetFixedSize);
		mainLayout->addWidget(recordButton, 	0, 0, 1, 1);
		mainLayout->addWidget(stopRecordButton, 	0, 1, 1, 1);
		mainLayout->addWidget(frameRateLabel,	0, 2, 1, 1);
		mainLayout->addWidget(inputFrameRateEL, 	0, 3, 1, 1);
		//mainLayout->addWidget(fileSizeLabel,	0, 4, 1, 1);
		//mainLayout->addWidget(fileWidthEL,		0, 5, 1, 1);
		//mainLayout->addWidget(multplLabel,		0, 6, 1, 1);
		//mainLayout->addWidget(fileHeightEL,		0, 7, 1, 1);

		setLayout(mainLayout);
		setWindowTitle(tr("Frame recorder widget"));
	}

	void FrameRecorderWidget::setFrameSource(QWidget * frameSource)
	{
		this->dataSource = frameSource;
	}

	void FrameRecorderWidget::startRecordingClicked()
	{
		if(dataSource == 0)
		{
			QMessageBox::critical(
				this, 
				tr("Recording error"), 
				tr("Wrong frame recorder widget set up. No source widget to record from was setted."),
				QMessageBox::Ok);
			return;
		}

		if(recording)
		{
			QMessageBox::information(
				this, 
				tr("Recording widget"), 
				tr("A recording is undergoing at the momment. Stop the current recording to start a new one"),
				QMessageBox::Ok);
			return;
		}

		bool flag = false;
		/*
		frameWidth = fileWidthEL->displayText().toInt(&flag);
		if(!flag)
		{
			QMessageBox::critical(
				this, 
				tr("Recording error"), 
				tr("The input video width is not a valid value"),
				QMessageBox::Ok);
			return;
		}

		flag = false;
		frameHeigth = fileHeightEL->displayText().toInt(&flag);
		if(!flag)
		{
			QMessageBox::critical(
				this, 
				tr("Recording error"), 
				tr("The input video height is not a valid value"),
				QMessageBox::Ok);
			return;
		}
		*/
		flag = false;
		frameRate = inputFrameRateEL->displayText().toDouble(&flag);
		if(!flag)
		{
			QMessageBox::critical(
				this, 
				tr("Recording error"), 
				tr("The input frame rate is not a valid value"),
				QMessageBox::Ok);
			return;
		}

		QString filter = tr("*.mp4");
		QString qtFileName = QFileDialog::getSaveFileName(
			this,
        		tr("Save video to..."), 
			tr(""),
        		filter,
			&filter);

		if(qtFileName.isEmpty())
		{
			return;
		}

		if(!qtFileName.endsWith(".mp4"))
		{
			qtFileName = qtFileName.append(".mp4");
		}

		QByteArray ba = qtFileName.toLatin1();
		const char * cStr = ba.data();
		strcpy(filename, cStr);

		QRect rect = dataSource->rect();
		frameWidth = rect.width();
		frameHeigth = rect.height();

		screen = QGuiApplication::primaryScreen();

		recordButton->setEnabled(false);
		stopRecordButton->setEnabled(true);
		inputFrameRateEL->setEnabled(false);
		//fileWidthEL->setEnabled(false);
		//fileHeightEL->setEnabled(false);

		recording = true;

		threads.emplace_back(std::thread (&FrameRecorderWidget::produceFrame, this));
		threads.emplace_back(std::thread (&FrameRecorderWidget::consumeFrame, this));
	}
	
	void FrameRecorderWidget::stopRecordingClicked()
	{
		recording = false;
	
		// Await for threads to finish
		for(auto & th : threads)
		{
			th.join();
		}

		threads.clear();
		
		recordButton->setEnabled(true);
		stopRecordButton->setEnabled(false);
		inputFrameRateEL->setEnabled(true);
		//fileWidthEL->setEnabled(true);
		//fileHeightEL->setEnabled(true);
	}

	QToolButton * FrameRecorderWidget::createButton(const QString & text, const char * slotMember)
	{	
		QToolButton * newButton = new QToolButton();
		newButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
		newButton->setText(text);
		connect(newButton, SIGNAL(clicked()), this, slotMember);
		return newButton;
	}

	void FrameRecorderWidget::produceFrame()
	{
		
		std::unique_lock<std::mutex> lock(mtx);
		lock.unlock();

		auto start = std::chrono::high_resolution_clock::now();
		double rate = 1.0/frameRate;
		while(recording)
		{
			// Framerate sync
			auto point = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> diff = point - start;

			double syncTime = diff.count() - rate;
			if(syncTime < 0.0)
			{
				std::this_thread::sleep_for(std::chrono::duration<double>(-syncTime));
			}
			start = std::chrono::high_resolution_clock::now();

			// Produce frame
			QPixmap ss = screen->grabWindow(dataSource->winId());
			frameWidth = ss.width();
			frameHeigth = ss.height();

			QImage image(ss.toImage().convertToFormat(QImage::Format_ARGB32));			
			
			// Add to queue
			lock.lock();
			frameQueue.push(image);
			lock.unlock();

			// Wake up consumer
			monitor.notify_all();
		}
		
	}

	void FrameRecorderWidget::consumeFrame()
	{
		av_register_all ( );

		 // Shut up ffmepg
		av_log_set_level(AV_LOG_QUIET);

		struct SwsContext* convertCtx = sws_getContext(frameWidth, frameHeigth, AV_PIX_FMT_RGB24, frameWidth, frameHeigth, AV_PIX_FMT_YUV420P, SWS_FAST_BILINEAR, NULL, NULL, NULL); 
		// Preparing to convert my generated RGB images to YUV frames.

		AVDictionary * opt = NULL;
		// Preparing the data concerning the format and codec in order to write properly the header, frame data and end of file.
		//const char *fmtext="mp4";
		//char *filename = new char[100];
		//sprintf(filename, "GeneratedVideo.%s", fmtext);
		AVOutputFormat * fmt = av_guess_format("mp4", NULL, NULL);
		AVFormatContext *oc = NULL;
		avformat_alloc_output_context2(&oc, NULL, NULL, filename);
		AVStream * stream = avformat_new_stream(oc, 0);
		AVCodec *codec=NULL;
		AVCodecContext *c= NULL;
		int ret;

		codec = avcodec_find_encoder_by_name("libx264");

		// Setting up the codec:
		av_dict_set( &opt, "preset", "slow", 0 );
		av_dict_set( &opt, "crf", "20", 0 );
		avcodec_get_context_defaults3(stream->codec, codec);
		c=avcodec_alloc_context3(codec);
		c->width = frameWidth;
		c->height = frameHeigth;
		c->pix_fmt = AV_PIX_FMT_YUV420P;

		// Setting up the format, its stream(s), linking with the codec(s) and write the header:
		if (oc->oformat->flags & AVFMT_GLOBALHEADER) // Some formats require a global header.
		    c->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
		avcodec_open2( c, codec, &opt );
		av_dict_free(&opt);
		stream->time_base=(AVRational){1, 25};
		stream->codec=c; // Once the codec is set up, we need to let the container know which codec are the streams using, in this case the only (video) stream.
		av_dump_format(oc, 0, filename, 1);
		avio_open(&oc->pb, filename, AVIO_FLAG_WRITE);
		ret=avformat_write_header(oc, &opt);
		av_dict_free(&opt); 

		// Preparing the containers of the frame data:
		AVFrame *rgbpic, *yuvpic;

		// Allocating memory for each RGB frame, which will be lately converted to YUV:
		rgbpic=av_frame_alloc();
		rgbpic->format=AV_PIX_FMT_RGB24;
		rgbpic->width= frameWidth;
		rgbpic->height=frameHeigth;
		ret=av_frame_get_buffer(rgbpic, 1);

		// Allocating memory for each conversion output YUV frame:
		yuvpic=av_frame_alloc();
		yuvpic->format=AV_PIX_FMT_YUV420P;
		yuvpic->width=frameWidth;
		yuvpic->height=frameHeigth;
		ret=av_frame_get_buffer(yuvpic, 1);

		int got_output;
		AVPacket pkt;
		int i = 0;
		std::unique_lock<std::mutex> lock(mtx);
		lock.unlock();

		while(recording)
		{
			// Wait until the producer has placed a frame
			lock.lock();		     	
			while(frameQueue.empty())
			{
				monitor.wait(lock);
			}
			QImage image = frameQueue.front();
			frameQueue.pop();
			lock.unlock();
	
		    // The AVFrame data will be stored as RGBRGBRGB... row-wise, from left to right and from top to bottom, hence we have to proceed as follows:
		    for (int y=0; y<frameHeigth; y++)
		    {
			  for (int x=0; x<frameWidth; x++)
			  {
				// Decode qt pixel from uint
				QRgb pixelColor = image.pixel(x,y);
				int b = pixelColor & 0xff;
				int g = (pixelColor >> 8) & 0xff;
				int r = (pixelColor >> 16) & 0xff;
				// rgbpic->linesize[0] is equal to width.
				rgbpic->data[0][y*rgbpic->linesize[0]+3*x]=r;
				rgbpic->data[0][y*rgbpic->linesize[0]+3*x+1]=g;
				rgbpic->data[0][y*rgbpic->linesize[0]+3*x+2]=b;
			  }
		    }
		    sws_scale(convertCtx, rgbpic->data, rgbpic->linesize, 0, frameHeigth, yuvpic->data, yuvpic->linesize); 
			// Not actually scaling anything, but just converting the RGB data to YUV and store it in yuvpic.
		    av_init_packet(&pkt);
		    pkt.data = NULL;
		    pkt.size = 0;
		    yuvpic->pts = i; // The PTS of the frame are just in a reference unit, unrelated to the format we are using. We set them, for instance, as the corresponding frame number.
		    ret=avcodec_encode_video2(c, &pkt, yuvpic, &got_output);
		    if (got_output)
		    {
			  fflush(stdout);
			  av_packet_rescale_ts(&pkt, (AVRational){1, 25}, stream->time_base); 
				// We set the packet PTS and DTS taking in the account our FPS (second argument) and the time base that our selected format uses (third argument).
			  pkt.stream_index = stream->index;
			  //printf("Write frame %6d (size=%6d)\n", i, pkt.size);
			  av_interleaved_write_frame(oc, &pkt); // Write the encoded frame to the mp4 file.
			  av_packet_unref(&pkt);
		    }

			i++;
		}

		// Writing the delayed frames:
		for (got_output = 1; got_output; i++) {
		    ret = avcodec_encode_video2(c, &pkt, NULL, &got_output);
		    if (got_output) {
			  fflush(stdout);
			  av_packet_rescale_ts(&pkt, (AVRational){1, 25}, stream->time_base);
			  pkt.stream_index = stream->index;
			  //printf("Write frame %6d (size=%6d)\n", i, pkt.size);
			  av_interleaved_write_frame(oc, &pkt);
			  av_packet_unref(&pkt);
		    }
		}
		
		av_write_trailer(oc); // Writing the end of the file.
		if (!(fmt->flags & AVFMT_NOFILE))
		    avio_closep(&(oc->pb)); // Closing the file.
		avcodec_close(stream->codec);
		// Freeing all the allocated memory:
		sws_freeContext(convertCtx);
		av_frame_free(&rgbpic);
		av_frame_free(&yuvpic);
		avformat_free_context(oc);

		if(ret != 0)
		{
			// Error handling
		}
	}
}
