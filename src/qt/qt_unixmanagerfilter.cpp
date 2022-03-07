/*
 * 86Box A hypervisor and IBM PC system emulator that specializes in
 *      running old operating systems and software designed for IBM
 *      PC systems and compatibles from 1981 through fairly recent
 *      system designs based on the PCI bus.
 *
 *      This file is part of the 86Box distribution.
 *
 *      Source file for Unix VM-managers (client-side)
 *
 * Authors:
 *      Teemu Korhonen
    Cacodemon345
 *
 *      Copyright 2022 Teemu Korhonen
 *      Copyright 2022 Cacodemon345
 */

#include "qt_unixmanagerfilter.hpp"
#include <stdio.h>

UnixManagerSocket::UnixManagerSocket(QObject* obj)
    : QLocalSocket(obj)
{
    connect(this, &QLocalSocket::readyRead, this, &UnixManagerSocket::readyToRead);
}

void UnixManagerSocket::readyToRead()
{
    if (canReadLine())
    {
        printf("%s \n", "Reading line");
        QByteArray line = readLine();
        line.resize(line.size() -1);
        printf("%lli \n", line.size());
        printf("%s \n", line.data());
        if (line.size())
        {
            if (line.data() == "showsettings")
            {
                emit showsettings();
            }
            else if (line.data() == "pause")
            {
                emit pause();
                printf("%s \n", "Matched Pause")
            }
            else if (line.data() == "cad")
            {
                emit ctrlaltdel();
            }
            else if (line.data() == "reset")
            {
                emit resetVM();
            }
            else if (line.data() == "shutdownnoprompt")
            {
                emit force_shutdown();
            }
            else if (line.data() == "shutdown")
            {
                emit request_shutdown();
            }
        }
    }
}

bool UnixManagerSocket::eventFilter(QObject *obj, QEvent *event)
{
    if (state() == QLocalSocket::ConnectedState)
    {
        if (event->type() == QEvent::WindowBlocked)
        {
            write(QByteArray{"1"});
        }
        else if (event->type() == QEvent::WindowUnblocked)
        {
            write(QByteArray{"0"});
        }
    }

    return QObject::eventFilter(obj, event);
}
