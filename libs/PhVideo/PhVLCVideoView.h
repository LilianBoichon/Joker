#ifndef PHVLCVIDEOVIEW_H
#define PHVLCVIDEOVIEW_H

#include <QWidget>
#include <vlc/vlc.h>

#include "PhTools/PhClock.h"

class PhVLCVideoView : public QWidget
{
	Q_OBJECT
public:
	/**
	 * @brief PhVideoView constructor
	 * @param parent widget for the view
	 */
	explicit PhVLCVideoView(QWidget *parent = 0);

	~PhVLCVideoView();

	/**
	 * @brief Open a video file
	 * @param fileName A video file path
	 * @return True if the file was opened successfully, false otherwise
	 */
	bool open(QString fileName);

	void setClock(PhClock *clock);

signals:

public slots:
	void onRateChanged(PhRate rate);
	void onFrameChanged();

private:
	libvlc_instance_t *vlcInstance;
    libvlc_media_player_t *vlcPlayer;

	PhClock *_clock;
};

#endif // PHVLCVIDEOVIEW_H
