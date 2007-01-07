/* ============================================================
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published bythe Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef ZMLIVEPLAYER_H
#define ZMLIVEPLAYER_H


#include <mythtv/uitypes.h>
#include <mythtv/uilistbtntype.h>
#include <mythtv/xmlparse.h>
#include <mythtv/mythdialogs.h>

//gl stuff
#include <GL/glx.h>
#include <GL/glu.h>

// mythzoneminder
#include "zmdefines.h"

class Player
{
  public:
    Player(void);
    ~Player(void);

    bool startPlayer(Monitor *mon, Window winID);
    void stopPlaying(void);
    void updateScreen(const uchar* buffer);
    void setMonitor(int monID, Window winID);
    Monitor *getMonitor(void) { return &m_monitor; }

  private:
    void getMonitorList(void);

    Monitor     m_monitor;
    bool        m_initalized;
    GLXContext  m_cx;
    Display    *m_dis;
    Window      m_win;
};

class ZMLivePlayer : public MythThemedDialog
{
    Q_OBJECT

public:

    ZMLivePlayer(int monitorID, int eventID, MythMainWindow *parent,
             const QString &window_name, const QString &theme_filename,
             const char *name = 0);
    ~ZMLivePlayer();

    void setMonitorLayout(int layout);

  private slots:
    void updateFrame(void);
    void updateMonitorStatus(void);
    void initMonitorLayout(void);
    void receivedLiveFrame(int minitorID, QString& status, const uchar* buffer, int frameSize);
    void getMonitorList(void);

  private:
    void wireUpTheme(void);
    UITextType* getTextType(QString name);
    void keyPressEvent(QKeyEvent *e);
    void stopPlayers(void);
    void changePlayerMonitor(int playerNo);

    QTimer               *m_frameTimer;
    QTimer               *m_statusTimer;
    bool                  m_paused;
    int                   m_eventID;
    int                   m_monitorID;
    int                   m_monitorLayout;
    int                   m_monitorCount;

    vector<Player *>     *m_players;
    vector<Monitor *>    *m_monitors;

    fontProp             *m_idleFont;
    fontProp             *m_alarmFont;
    fontProp             *m_alertFont;
};

#endif
