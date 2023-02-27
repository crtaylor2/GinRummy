#ifndef GIN_RUMMY_REST_SERVER
#define GIN_RUMMY_REST_SERVER

#include "GinRummy.h"

#include <string>
#include <cpprest/http_listener.h>
#include <cpprest/http_msg.h>
#include <pplx/pplxtasks.h>

class GinRummyRestServer
{
    public:
        GinRummyRestServer(std::string url);

        pplx::task<void> open() { return m_listener.open(); }
        pplx::task<void> close() { return m_listener.close(); }

    private:
        void handle_get(web::http::http_request message);

        std::string createJsonResponse(const std::string& response) const;

        GinRummy ginRummy;
        web::http::experimental::listener::http_listener m_listener;
};

#endif
