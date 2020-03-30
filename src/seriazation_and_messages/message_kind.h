#ifndef _MESSAGEKIND_H
#define _MESSAGEKIND_H

#include "message.h"

static const MsgKind ACK = MsgKind::Ack;
static const MsgKind NACK = MsgKind::Nack;
static const MsgKind PUT = MsgKind::Put;
static const MsgKind REPLY = MsgKind::Reply;
static const MsgKind GET = MsgKind::Get;
static const MsgKind WAITANDGET = MsgKind::WaitAndGet;
static const MsgKind STATUS = MsgKind::Status;
static const MsgKind KILL = MsgKind::Kill;
static const MsgKind REGISTER = MsgKind::Register;
static const MsgKind DIRECTORY = MsgKind::Directory;

#endif
