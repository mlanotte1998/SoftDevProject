#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include "message.h"

class Register : public Message {
	sockaddr_in client;
	size_t port;


};
