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

#include "web/http/server/RequestParser.h"

#include "web/http/http_utils.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <regex>
#include <tuple> // for tie, tuple
#include <utility>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace web::http::server {

    RequestParser::RequestParser(
        core::socket::SocketContext* socketContext,
        const std::function<void(void)>& onStart,
        const std::function<void(
            const std::string&, const std::string&, const std::string&, int, int, const std::map<std::string, std::string>&)>& onRequest,
        const std::function<void(const std::map<std::string, std::string>&, const std::map<std::string, std::string>&)>& onHeader,
        const std::function<void(std::vector<uint8_t>&)>& onContent,
        const std::function<void()>& onParsed,
        const std::function<void(int, const std::string&)>& onError)
        : Parser(socketContext)
        , onStart(onStart)
        , onRequest(onRequest)
        , onHeader(onHeader)
        , onContent(onContent)
        , onParsed(onParsed)
        , onError(onError) {
    }

    void RequestParser::reset() {
        Parser::reset();
        method.clear();
        url.clear();
        httpVersion.clear();
        queries.clear();
        cookies.clear();
        httpMajor = 0;
        httpMinor = 0;
    }

    bool RequestParser::methodSupported(const std::string& method) const {
        return supportedMethods.contains(method);
    }

    void RequestParser::begin() {
        onStart();
    }

    enum Parser::ParserState RequestParser::parseStartLine(const std::string& line) {
        enum Parser::ParserState parserState = Parser::ParserState::HEADER;

        if (!line.empty()) {
            std::string remaining;

            std::tie(method, remaining) = httputils::str_split(line, ' ');
            std::tie(url, httpVersion) = httputils::str_split(remaining, ' ');

            std::string queriesLine;
            std::tie(std::ignore, queriesLine) = httputils::str_split(url, '?');

            if (!methodSupported(method)) {
                parserState = parsingError(400, "Bad request method");
            } else if (url.empty() || url.front() != '/') {
                parserState = parsingError(400, "Malformed request");
            } else {
                std::smatch httpVersionMatch;
                if (!std::regex_match(httpVersion, httpVersionMatch, httpVersionRegex)) {
                    parserState = parsingError(400, "Wrong protocol-version");
                } else {
                    httpMajor = std::stoi(httpVersionMatch.str(1));
                    httpMinor = std::stoi(httpVersionMatch.str(2));

                    while (!queriesLine.empty()) {
                        std::string query;

                        std::tie(query, queriesLine) = httputils::str_split(queriesLine, '&');
                        queries.insert(httputils::str_split(query, '='));
                    }

                    onRequest(method, url, httpVersion, httpMajor, httpMinor, queries);
                }
            }
        } else {
            parserState = parsingError(400, "Request-line empty");
        }

        return parserState;
    }

    enum Parser::ParserState RequestParser::parseHeader() {
        for (auto& [headerFieldName, headerFieldValue] : Parser::headers) { // cppcheck-suppress unassignedVariable
            if (headerFieldName != "cookie") {
                if (headerFieldName == "content-length") {
                    Parser::contentLength = std::stoul(headerFieldValue);
                }
            } else {
                std::string cookiesLine = headerFieldValue;

                while (!cookiesLine.empty()) {
                    std::string cookieLine;
                    std::tie(cookieLine, cookiesLine) = httputils::str_split(cookiesLine, ',');

                    while (!cookieLine.empty()) {
                        std::string cookie;
                        std::tie(cookie, cookieLine) = httputils::str_split(cookieLine, ';');

                        std::string cookieName;
                        std::string cookieValue;
                        std::tie(cookieName, cookieValue) = httputils::str_split(cookie, '=');

                        httputils::str_trimm(cookieName);
                        httputils::str_trimm(cookieValue);

                        cookies.insert({cookieName, cookieValue});
                    }
                }
            }
        }

        Parser::headers.erase("cookie");

        onHeader(Parser::headers, cookies);

        enum Parser::ParserState parserState = Parser::ParserState::BODY;
        if (contentLength == 0 && httpMinor == 1) {
            parsingFinished();
            parserState = ParserState::BEGIN;
        }

        return parserState;
    }

    Parser::ParserState RequestParser::parseContent(std::vector<uint8_t>& content) {
        onContent(content);
        parsingFinished();

        return ParserState::BEGIN;
    }

    void RequestParser::parsingFinished() {
        onParsed();
    }

    enum Parser::ParserState RequestParser::parsingError(int code, const std::string& reason) {
        onError(code, reason);

        return ParserState::ERROR;
    }

} // namespace web::http::server
