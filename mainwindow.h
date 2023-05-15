#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <vector>


#include <QMainWindow>
#include <QStackedLayout>
#include <QMessageBox>
#include <QListWidget>
#include <QMediaPlayer>
#include <QAudioOutput>

#include <cstddef>
#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "structures.h"
#include "coverdownloader.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    std::vector<BookEntry> books;
    void showList();
    void mediaPlayerInit();
    void initKeyboardShortcuts();
    void sliderInit();

private slots:
    void on_playBtn_clicked();

    void on_pauseBtn_clicked();

    void on_forwardBtn_clicked();

    void on_backBtn_clicked();

    void displayCover();

    void loadImage();


private:
    Ui::MainWindow *ui;
    std::string pass;
    std::string email;
    std::string loginResponse;
    std::string token;
    QMediaPlayer *player;
    QAudioOutput* audioOutput;
    int idx;
    bool try_login();
    void sliderSeek(int millis);

    CoverDownloader *cd;
};
#endif // MAINWINDOW_H
