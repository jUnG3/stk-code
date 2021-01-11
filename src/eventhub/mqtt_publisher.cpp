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

#include "mqtt_publisher.hpp"
#include <cstring>

void MQTTPublisher::publishEvent(const std::string &type) {
    mqttClient.sendMessage("stk", type.c_str());
}

void
MQTTPublisher::publishEvent(const std::string &type, const std::string &info) {
    char *buffer = new char[type.length() + info.length() + 3];
    std::strcpy(buffer, type.c_str());
    std::strcat(buffer, " ");
    std::strcat(buffer, info.c_str());
    mqttClient.sendMessage("stk", buffer);
    delete[] buffer;
}

MQTTPublisher::MQTTPublisher()
        : mqttClient(Client()) {}
