// Copyright 2012 CrossControl

#ifndef CANWRAPPER_H
#define CANWRAPPER_H

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/select.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/can/error.h>

class CanWrapper
{
public:
    CanWrapper();

    bool Init(const char *interfaceName, int &errorCode);

    void Close();

    bool SendMsg(struct can_frame msg, bool extended, bool rtr, int &errorCode);

    bool GetMsg(struct can_frame &frame, bool &extended, bool &rtr, bool &error, int &errorCode, struct timeval timeout);

    bool SetRecvBufferSize(int size);

    void EnableErrorMessages();

    void DisableEcho();

private:
    bool m_initialized; // indicates if socket is initialized

    int m_socket;       // Socket
};

#endif // CANWRAPPER_H
