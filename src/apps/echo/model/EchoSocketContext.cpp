/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020, 2021, 2022 Volker Christian <me@vchrist.at>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "EchoSocketContext.h"

#include "log/Logger.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <cstddef>     // for size_t
#include <string>      // for string
#include <sys/types.h> // for ssize_t

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace apps::echo::model {

    EchoSocketContext::EchoSocketContext(core::socket::SocketConnection* socketConnection, Role role)
        : core::socket::SocketContext(socketConnection)
        , role(role) {
    }

    void EchoSocketContext::onConnected() {
        VLOG(0) << "Echo connected";

        if (role == Role::CLIENT) {
            sendToPeer("Hello peer! Nice to see you!!!");
        }
    }

    void EchoSocketContext::onDisconnected() {
        VLOG(0) << "Echo disconnected";
    }

    void EchoSocketContext::onReceiveFromPeer() {
        char junk[4096];

        ssize_t ret = readFromPeer(junk, 4096);

        if (ret > 0) {
            std::size_t junklen = static_cast<std::size_t>(ret);
            VLOG(0) << "Data to reflect: " << std::string(junk, junklen);
            sendToPeer(junk, junklen);
        }
    }

    core::socket::SocketContext* EchoServerSocketContextFactory::create(core::socket::SocketConnection* socketConnection) {
        return new EchoSocketContext(socketConnection, EchoSocketContext::Role::SERVER);
    }

    core::socket::SocketContext* EchoClientSocketContextFactory::create(core::socket::SocketConnection* socketConnection) {
        return new EchoSocketContext(socketConnection, EchoSocketContext::Role::CLIENT);
    }

} // namespace apps::echo::model
