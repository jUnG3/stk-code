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

#ifndef HEADER_EVENT_HUB_HPP
#define HEADER_EVENT_HUB_HPP

#include <string>
#include <set>
#include <mutex>

/**
 * interface to process events from the hub
 */
class EventSink {
public:
   virtual void publishEvent(const std::string& type) = 0;
   virtual void publishEvent(const std::string& type, const std::string& info) = 0;
};

class EventHub {
private:
    static EventHub* m_event_hub_singleton;

    std::set<EventSink*> m_sinks;
    char m_buffer[1024];
    std::mutex m_mutex;

    EventHub() {};
public:
    /**
     * returns the singleton instance of EventHub
     */
    static EventHub* get()
    {
        if (!m_event_hub_singleton) {
            m_event_hub_singleton = new EventHub();
        }
        return m_event_hub_singleton;
    }

    void registerSink(EventSink* sink) {
        m_sinks.insert(sink);
    }

    void unregisterSink(EventSink* sink) {
        m_sinks.erase(sink);
    }

    void publishEvent(const std::string& type);
    void publishEvent(const std::string& type, const char* format...);
};

#endif
