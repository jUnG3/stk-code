/*
 * Copyright (C) 2021 Stjepan Soldo
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef STK_MQTT_CLIENT_H
#define STK_MQTT_CLIENT_H

#include <mosquitto.h>
#include <string>
#include <cerrno>
#include <iostream>

class Client {
private:
    char *id;
    struct mosquitto *mosquittoClient;
    int port;
    char *host;
    int keepAlive;
public:
    Client();

    ~Client();

    void
    sendMessage(const char *topic, const char *message,
                int qualityOfService = 0,
                bool retain = false) const;
};

#endif //STK_MQTT_CLIENT_H
