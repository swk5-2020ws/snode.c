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

#ifndef WEB_WEBSOCKET_SUBSPROTOCOL_H
#define WEB_WEBSOCKET_SUBSPROTOCOL_H

namespace web::websocket {
    template <typename SubProtocolT, typename RequestT, typename ResponseT>
    class SocketContextUpgrade;
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include "log/Logger.h"

#include <cstddef>
#include <cstdint>
#include <string>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace web::websocket {

    template <typename SocketContextUpgradeT>
    class SubProtocol {
    public:
        using SocketContextUpgrade = SocketContextUpgradeT;

    protected:
        SubProtocol() = delete;
        SubProtocol(const std::string& name);

        SubProtocol(const SubProtocol&) = delete;
        SubProtocol& operator=(const SubProtocol&) = delete;

        virtual ~SubProtocol() = default;

    public:
        /* Facade (API) to WSServerContext -> WSTransmitter to be used from SubProtocol-Subclasses */
        void sendMessage(const char* message, std::size_t messageLength);
        void sendMessage(const std::string& message);
        void sendMessageStart(const char* message, std::size_t messageLength);
        void sendMessageStart(const std::string& message);
        void sendMessageFrame(const char* message, std::size_t messageLength);
        void sendMessageFrame(const std::string& message);
        void sendMessageEnd(const char* message, std::size_t messageLength);
        void sendMessageEnd(const std::string& message);
        void sendPing(char* reason = nullptr, std::size_t reasonLength = 0);
        void sendClose(uint16_t statusCode = 1000, const char* reason = nullptr, std::size_t reasonLength = 0);

        const std::string& getName();

    private:
        /* Callbacks (API) WSReceiver -> SubProtocol-Subclasses */
        virtual void onMessageStart(int opCode) = 0;
        virtual void onMessageData(const char* junk, std::size_t junkLen) = 0;
        virtual void onMessageEnd() = 0;
        virtual void onPongReceived() = 0;
        virtual void onMessageError(uint16_t errnum) = 0;

        /* Callbacks (API) socketConnection -> SubProtocol-Subclasses */
        virtual void onConnected() = 0;
        virtual void onDisconnected() = 0;

        void setSocketContextUpgrade(SocketContextUpgrade* socketContextUpgrade);

        void setName(const std::string& name);

        SocketContextUpgrade* socketContextUpgrade;

        std::string name;

        template <typename SubProtocolT, typename RequestT, typename ResponseT>
        friend class web::websocket::SocketContextUpgrade;

        friend SocketContextUpgrade;
    };

} // namespace web::websocket

#endif // WEB_WEBSOCKET_SUBSPROTOCOL_H
