#ifndef TLS_SOCKETREADER_H
#define TLS_SOCKETREADER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <cstddef>     // for size_t
#include <sys/types.h> // for ssize_t

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "socket/SocketReader.h"
#include "socket/tls/Socket.h"

// IWYU pragma: no_forward_declare tls::Socket

namespace tls {

    class SocketReader : public ::SocketReader<tls::Socket> {
    protected:
        using ::SocketReader<tls::Socket>::SocketReader;

    private:
        ssize_t recv(char* junk, size_t junkSize) override;
    };

}; // namespace tls

#endif // TLS_SOCKETREADER_H
