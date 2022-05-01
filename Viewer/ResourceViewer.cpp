//
// Created by Rache on 2021/10/30.
//

#include "ResourceViewer.h"
#include <QVBoxLayout>
#include <QBuffer>
#include <ToolFunction.h>
#include <globals.h>

ImageViewer::ImageViewer() {
    init();
}

ImageViewer::ImageViewer(const QImage& i) {
    init();
    set_image(i);
}

void ImageViewer::set_image(const QImage& i) {
    image = i;
    sizeLabel->setText(QObject::tr("<b>长</b> %1像素<br><b>宽</b> %2像素").arg(i.width()).arg(i.height()));

    QImage p2;
    int h = i.height(), w = i.width();
    if (h > fixedPixmapHeight and w > fixedPixmapWidth) {
        if (h / fixedPixmapHeight > w / fixedPixmapWidth) {
            p2 = i.scaledToHeight(fixedPixmapHeight);
        } else {
            p2 = i.scaledToWidth(fixedPixmapWidth);
        }
    } else if (h > fixedPixmapHeight) {
        p2 = i.scaledToHeight(fixedPixmapHeight);
    } else if (h > fixedPixmapWidth) {
        p2 = i.scaledToHeight(fixedPixmapWidth);
    } else {
        p2 = i;
    }

    pixmapWidget->set_image(p2);
    pixmapWidget->setFixedSize(p2.width(), p2.height());
}

void ImageViewer::init() {
    pixmapWidget = new ImageWidget();
    sizeLabel = new QLabel();
    sizeLabel->setWordWrap(true);

    auto vBoxLayout = new QVBoxLayout;
    vBoxLayout->addWidget(sizeLabel);
    vBoxLayout->addWidget(pixmapWidget);
    vBoxLayout->addStretch();
    setLayout(vBoxLayout);
}

AudioViewer::AudioViewer() {
    mediaPlayer = new QMediaPlayer;
    timeLabel = new QLabel;
    playButton = new QToolButton;
    timeSlider = new QSlider(Qt::Horizontal);

    playButton->setIcon(PlayIcon);

    mediaBuffer = new QBuffer;

    auto *toolLayout = new QHBoxLayout;
    toolLayout->addWidget(playButton);
    toolLayout->addWidget(timeLabel);
    auto *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(timeSlider);
    mainLayout->addLayout(toolLayout);
    mainLayout->addStretch(0);
    setLayout(mainLayout);

    connect(playButton, SIGNAL(clicked()), this, SLOT(playButton_clicked()));
    connect(mediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(player_duration_changed(long long)));
    connect(mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(position_changed(long long)));
    connect(timeSlider, SIGNAL(sliderMoved(int)), this, SLOT(timeSlider_moved(int)));
}

bool AudioViewer::set_audio(const QByteArray &bytes) {
    mediaPlayer->stop();
    mediaBuffer->close();
    mediaBuffer->setData(bytes);
    mediaBuffer->open(QBuffer::ReadOnly);
    mediaPlayer->setMedia(QMediaContent(), mediaBuffer);
    playing = false;
    playButton->setIcon(PlayIcon);
    return mediaPlayer->state();
}

void AudioViewer::player_duration_changed(long long int d) {
    duration = d;
    timeSlider->setValue(0);
    timeSlider->setMaximum(d);
    durationText = "/" + time_to_string(d / 1000);
    timeLabel->setText("0:00" + durationText);
}

void AudioViewer::position_changed(long long int p) const {
    timeSlider->setValue(p);
    timeLabel->setText(time_to_string(p / 1000) + durationText);
}

void AudioViewer::playButton_clicked() {
    if (playing) {
        playing = false;
        playButton->setIcon(PlayIcon);
        mediaPlayer->pause();
    } else {
        playing = true;
        playButton->setIcon(PauseIcon);
        mediaPlayer->play();
    }
}

void AudioViewer::timeSlider_moved(int v) {
    mediaPlayer->setPosition(v);
}

AudioViewer::~AudioViewer() {
    mediaPlayer->stop();
    mediaBuffer->close();
    delete mediaPlayer;
    delete mediaBuffer;
}

VideoViewer::VideoViewer() {
    mediaPlayer = new QMediaPlayer;
    timeLabel = new QLabel;
    playButton = new QToolButton;
    timeSlider = new QSlider(Qt::Horizontal);
    videoWidget = new QVideoWidget;

    playButton->setIcon(PlayIcon);
    mediaPlayer->setVideoOutput(videoWidget);

    mediaBuffer = new QBuffer;

    auto *toolLayout = new QHBoxLayout;
    toolLayout->addWidget(timeSlider);
    toolLayout->addWidget(playButton);
    toolLayout->addWidget(timeLabel);
    auto *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(videoWidget);
    mainLayout->addLayout(toolLayout);
    mainLayout->addStretch(0);
    setLayout(mainLayout);

    connect(playButton, SIGNAL(clicked()), this, SLOT(playButton_clicked()));
    connect(mediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(player_duration_changed(long long)));
    connect(mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(position_changed(long long)));
    connect(timeSlider, SIGNAL(sliderMoved(int)), this, SLOT(timeSlider_moved(int)));
}

VideoViewer::~VideoViewer() {
    mediaPlayer->stop();
    mediaBuffer->close();
    delete mediaPlayer;
    delete mediaBuffer;
}

bool VideoViewer::set_video(const QByteArray &byteArray) {
    mediaPlayer->stop();
    mediaBuffer->close();
    mediaBuffer->setData(byteArray);
    mediaBuffer->open(QBuffer::ReadOnly);
    mediaPlayer->setMedia(QMediaContent(), mediaBuffer);
    playing = false;
    playButton->setIcon(PlayIcon);
    return mediaPlayer->state();
}

void VideoViewer::position_changed(long long int p) const {
    timeSlider->setValue(p);
    timeLabel->setText(time_to_string(p / 1000) + durationText);
}

void VideoViewer::player_duration_changed(long long int d) {
    duration = d;
    timeSlider->setValue(0);
    timeSlider->setMaximum(d);
    durationText = "/" + time_to_string(d / 1000);
    timeLabel->setText("0:00" + durationText);
    videoWidget->setMinimumSize(1280, 720);
}

void VideoViewer::playButton_clicked() {
    if (playing) {
        playing = false;
        playButton->setIcon(PlayIcon);
        mediaPlayer->pause();
    } else {
        playing = true;
        playButton->setIcon(PauseIcon);
        mediaPlayer->play();
    }
}

void VideoViewer::timeSlider_moved(int v) {
    mediaPlayer->setPosition(v);
}
