/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020 Volker Christian <me@vchrist.at>
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

#ifndef READSTREAM_H
#define READSTREAM_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <list>
#include <stddef.h> // for size_t

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace net::stream {

    class WriteStream;

    class ReadStream {
    public:
        ReadStream();

        virtual ~ReadStream();

        void pipe(WriteStream& writeStream);
        void unPipe(WriteStream& writeStream);

        void dispatch(const char* junk, size_t junkLen);
        void dispatchEOF();
        void dispatchError(int errnum);

    protected:
        std::list<WriteStream*> writeStreams;

    private:
        std::list<WriteStream*> unPipedStreams;

        bool dispatching = false;
    };

} // namespace net::stream

#endif // READSTREAM_H
