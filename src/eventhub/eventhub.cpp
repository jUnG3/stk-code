/*
 * Copyright (C) 2019 Fabian Gajek
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

#include "eventhub.hpp"

#include <cstdarg>

EventHub* EventHub::m_event_hub_singleton = nullptr;

void EventHub::publishEvent(const std::string& type) {
    m_mutex.lock();
    for (auto sink: m_sinks) {
        sink->publishEvent(type);
    }
    m_mutex.unlock();
}


void EventHub::publishEvent(const std::string& type, const char* format...) {
    va_list args;
    va_start(args, format);

    m_mutex.lock();

    vsnprintf(m_buffer, sizeof(m_buffer), format, args);

    va_end(args);

    for (auto sink: m_sinks) {
        sink->publishEvent(type, std::string(m_buffer));
    }

    m_mutex.unlock();
}
