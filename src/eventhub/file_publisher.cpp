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

#include "file_publisher.hpp"
#include "utils/log.hpp"

FilePublisher::FilePublisher(const std::string path) {
    m_file.open(path, std::ios_base::out);
}

void FilePublisher::publishEvent(const std::string& type) {
    m_file << type << std::endl << std::flush;
}

void FilePublisher::publishEvent(const std::string& type, const std::string& info) {
    m_file << type << ":" << info << std::endl << std::flush;
}