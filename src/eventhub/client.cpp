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

#include "client.hpp"
#include "../utils/log.hpp"
#include <cstring>

Client::Client()
        : id{nullptr}, port{1883}, host{nullptr}, keepAlive{60 * 60} {

    mosquitto_lib_init();
    host = new char[10];
    strcpy(host, "localhost");

    mosquittoClient = mosquitto_new(id, id == nullptr, nullptr);

    if (mosquittoClient == nullptr) {
        switch (errno) {
            case ENOMEM:
                std::cerr << "Out of memory errno: " << errno << std::endl;
                throw std::exception();
            case EINVAL:
                throw std::invalid_argument("Invalid input parameters given");
            default:
                throw std::exception();
        }
    }

    int connectionStatus = mosquitto_connect(mosquittoClient, host,
                                             port, keepAlive);

    if (connectionStatus != MOSQ_ERR_SUCCESS) {
        switch (connectionStatus) {
            case MOSQ_ERR_INVAL:
                throw std::invalid_argument("Invalid input parameters given");
            case MOSQ_ERR_ERRNO:
                throw std::exception();
            default:
                throw std::exception();
        }
    }
}

Client::~Client() {
#ifdef DEBUG
    std::cout << "~Client() called" << std::endl;
#endif
    delete[] host;
    int result = mosquitto_disconnect(mosquittoClient);
    if (result != MOSQ_ERR_SUCCESS) {
        if (result == MOSQ_ERR_INVAL)
            std::cerr << "Invalid parameters on disconnect errno: "
                      << result << std::endl;
        if (result == MOSQ_ERR_NO_CONN)
            std::cerr
                    << "The client was not connected to a broker disconnect errno: "
                    << result << std::endl;
    }
    mosquitto_destroy(mosquittoClient);
    mosquitto_lib_cleanup();
}

void Client::sendMessage(const char *topic, const char *message,
                         int qualityOfService, bool retain) const {

    size_t maxLen = 455;
    size_t payloadLen = strlen(message) + 1;
    int responseCode;

    if (payloadLen > maxLen) {
        char *messageBuffer = new char[maxLen];
        std::strncpy(messageBuffer, message, maxLen);
        messageBuffer[maxLen - 1] = 0;

        responseCode = mosquitto_publish(mosquittoClient, nullptr, topic,
                                         maxLen, messageBuffer,
                                         qualityOfService,
                                         retain);
        delete[] messageBuffer;
    } else {
        responseCode = mosquitto_publish(mosquittoClient, nullptr, topic,
                                         payloadLen, message, qualityOfService,
                                         retain);
    }

    switch (responseCode) {
        case MOSQ_ERR_SUCCESS:
            break;
        case MOSQ_ERR_INVAL:
            Log::warn("Event", "mosquitto_publish fail: MOSQ_ERR_NOMEM %d",
                      MOSQ_ERR_NOMEM);
        case MOSQ_ERR_NOMEM:
            Log::warn("Event", "mosquitto_publish fail: MOSQ_ERR_NOMEM %d",
                      MOSQ_ERR_NOMEM);
        case MOSQ_ERR_NO_CONN:
            Log::warn("Event", "mosquitto_publish fail: MOSQ_ERR_NO_CONN %d",
                      MOSQ_ERR_NO_CONN);
        case MOSQ_ERR_PROTOCOL:
            Log::warn("Event", "mosquitto_publish fail: MOSQ_ERR_PROTOCOL %d",
                      MOSQ_ERR_PROTOCOL);
        case MOSQ_ERR_PAYLOAD_SIZE:
            Log::warn("Event", "mosquitto_publish fail: MOSQ_ERR_PAYLOAD_SIZE %d",
                      MOSQ_ERR_PAYLOAD_SIZE);
        case MOSQ_ERR_MALFORMED_UTF8:
            Log::warn("Event", "mosquitto_publish fail: MOSQ_ERR_MALFORMED_UTF8 %d",
                      MOSQ_ERR_MALFORMED_UTF8);
        case MOSQ_ERR_QOS_NOT_SUPPORTED:
            Log::warn("Event", "mosquitto_publish fail: MOSQ_ERR_QOS_NOT_SUPPORTED %d",
                      MOSQ_ERR_QOS_NOT_SUPPORTED);
        case MOSQ_ERR_OVERSIZE_PACKET:
            Log::warn("Event", "mosquitto_publish fail: MOSQ_ERR_OVERSIZE_PACKET %d",
                      MOSQ_ERR_OVERSIZE_PACKET);
    }
}
