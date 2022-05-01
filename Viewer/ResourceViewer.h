//
// Created by Rache on 2021/10/30.
//

#ifndef RACHEFILE_RESOURCEVIEWER_H
#define RACHEFILE_RESOURCEVIEWER_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QSlider>
#include <QToolButton>
#include <QBuffer>

class ImageWidget : public QWidget {
public:
    ImageWidget() = default;
    void set_image(const QImage& i) {
        image = i;
        update();
    }

    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.drawImage(0, 0, image);

        QWidget::paintEvent(event);
    }

    QImage image;
};

class ImageViewer : public QWidget{
public:
    ImageViewer();
    explicit ImageViewer(const QImage& i);

    QImage image;
    void set_image(const QImage& i);

private:
    void init();
    QLabel *sizeLabel;

    ImageWidget *pixmapWidget;

    const int fixedPixmapHeight  = 600;
    const int fixedPixmapWidth   = 800;
};

class AudioViewer : public QWidget{
    Q_OBJECT
public:
    AudioViewer();
    ~AudioViewer() override;
    bool set_audio(const QByteArray &bytes);

    QSlider *timeSlider;
    QLabel *timeLabel;
    QToolButton *playButton;

    long long duration = 0;
    QString durationText;
    bool playing = false;

private:
    QMediaPlayer *mediaPlayer;
    QBuffer *mediaBuffer;

public slots:
    void player_duration_changed(long long d);
    void position_changed(long long p) const;
    void playButton_clicked();
    void timeSlider_moved(int v);
};

class VideoViewer : public QWidget {
    Q_OBJECT
public:
    VideoViewer();
    ~VideoViewer() override;

    bool set_video(const QByteArray &byteArray);

    QSlider *timeSlider;
    QLabel *timeLabel;
    QToolButton *playButton;
    QVideoWidget *videoWidget;

    long long duration = 0;
    QString durationText;
    bool playing = false;

private:
    QMediaPlayer *mediaPlayer;
    QBuffer *mediaBuffer;

public slots:
    void player_duration_changed(long long d);
    void position_changed(long long p) const;
    void playButton_clicked();
    void timeSlider_moved(int v);
};


#endif //RACHEFILE_RESOURCEVIEWER_H
