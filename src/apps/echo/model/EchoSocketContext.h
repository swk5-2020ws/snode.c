/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020, 2021 Volker Christian <me@vchrist.at>
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

#ifndef APPS_MODEL_ECHOSOCKETCONTEXT_H
#define APPS_MODEL_ECHOSOCKETCONTEXT_H

#include "core/socket/stream/SocketContext.h"        // for SocketP...
#include "core/socket/stream/SocketContextFactory.h" // for SocketP...

namespace core::socket::stream {
    class SocketConnection;
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace apps::echo::model {

    class EchoServerSocketContext : public core::socket::stream::SocketContext {
    public:
        explicit EchoServerSocketContext(core::socket::stream::SocketConnection* socketConnection);

        void onConnected() override;
        void onDisconnected() override;

        void onReceiveFromPeer() override;

        void onWriteError(int errnum) override;
        void onReadError(int errnum) override;
    };

    class EchoServerSocketContextFactory : public core::socket::stream::SocketContextFactory {
    private:
        core::socket::stream::SocketContext* create(core::socket::stream::SocketConnection* socketConnection) override;
    };

    class EchoClientSocketContext : public core::socket::stream::SocketContext {
    public:
        explicit EchoClientSocketContext(core::socket::stream::SocketConnection* socketConnection);

        void onConnected() override;
        void onDisconnected() override;

        void onReceiveFromPeer() override;

        void onWriteError(int errnum) override;
        void onReadError(int errnum) override;
    };

    class EchoClientSocketContextFactory : public core::socket::stream::SocketContextFactory {
    private:
        core::socket::stream::SocketContext* create(core::socket::stream::SocketConnection* socketConnection) override;
    };

} // namespace apps::echo::model

#endif // APPS_MODEL_ECHOSOCKETCONTEXT_H
