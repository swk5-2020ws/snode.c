/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020  Volker Christian <me@vchrist.at>
 * Json Middleware 2020 Marlene Mayr, Anna Moser, Matteo Prock, Eric Thalhammer
 * Github <MarleneMayr><moseranna><MatteoMatteoMatteo><peregrin-tuk>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include "Logger.h"
#include "SNodeC.h"
#include "ServerResponse.h"
#include "config.h" // just for this example app
#include "legacy/Client.h"

#include <cstring>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

int main(int argc, char* argv[]) {
    net::SNodeC::init(argc, argv);

    http::legacy::Client jsonClient(
        [](http::legacy::Client::SocketConnection* socketConnection) -> void {
            VLOG(0) << "-- OnConnect";

            VLOG(0) << "\tServer: " + socketConnection->getRemoteAddress().toString();
            VLOG(0) << "\tClient: " + socketConnection->getLocalAddress().toString();
        },
        [](http::ServerRequest& serverRequest) -> void {
            serverRequest.method = "POST";
            serverRequest.url = "/index.html";
            serverRequest.type("application/json").send("{\"userId\":1,\"schnitzel\":\"good\",\"hungry\":false}");
        },
        [](const http::ServerResponse& clientResponse) -> void {
            VLOG(0) << "-- OnResponse";
            VLOG(0) << "     Status:";
            VLOG(0) << "       " << clientResponse.httpVersion;
            VLOG(0) << "       " << clientResponse.statusCode;
            VLOG(0) << "       " << clientResponse.reason;

            VLOG(0) << "     Headers:";
            for (auto [field, value] : *clientResponse.headers) {
                VLOG(0) << "       " << field + " = " + value;
            }

            VLOG(0) << "     Cookies:";
            for (auto [name, cookie] : *clientResponse.cookies) {
                VLOG(0) << "       " + name + " = " + cookie.getValue();
                for (auto [option, value] : cookie.getOptions()) {
                    VLOG(0) << "         " + option + " = " + value;
                }
            }

            char* body = new char[clientResponse.contentLength + 1];
            memcpy(body, clientResponse.body, clientResponse.contentLength);
            body[clientResponse.contentLength] = 0;

            VLOG(1) << "     Body:\n----------- start body -----------\n" << body << "------------ end body ------------";

            delete[] body;
        },
        [](int status, const std::string& reason) -> void {
            VLOG(0) << "-- OnResponseError";
            VLOG(0) << "     Status: " << status;
            VLOG(0) << "     Reason: " << reason;
        },
        [](http::legacy::Client::SocketConnection* socketConnection) -> void {
            VLOG(0) << "-- OnDisconnect";

            VLOG(0) << "\tServer: " + socketConnection->getRemoteAddress().toString();
            VLOG(0) << "\tClient: " + socketConnection->getLocalAddress().toString();
        });

    jsonClient.connect("localhost", 8080, [](int err) -> void {
        if (err != 0) {
            PLOG(ERROR) << "OnError: " << err;
        }
    });

    jsonClient.connect("localhost", 8080, [](int err) -> void {
        if (err != 0) {
            PLOG(ERROR) << "OnError: " << err;
        }
    });
    /*
        jsonClient.post("localhost", 8080, "/index.html", "{\"userId\":1,\"schnitzel\":\"good\",\"hungry\":false}", [](int err) -> void {
            if (err != 0) {
                PLOG(ERROR) << "OnError: " << err;
            }
        });
    */
    return net::SNodeC::start();
}
