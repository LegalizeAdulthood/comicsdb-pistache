#include "comic.h"
#include "comicsdb.h"

#include <pistache/endpoint.h>

namespace comicService
{

#if 0
std::shared_ptr<restbed::Settings> getSettings()
{
    auto settings = std::make_shared<restbed::Settings>();
    settings->set_default_header("Connection", "close");
    return settings;
}
#endif

class ComicHandler : public Pistache::Http::Handler
{
public:
    HTTP_PROTOTYPE(ComicHandler)

    void onRequest(const Pistache::Http::Request &request, Pistache::Http::ResponseWriter response)
    {
        response.send(Pistache::Http::Code::Ok, "Hello world\n");
    }
};

void run()
{
    Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(8080));

    auto opts = Pistache::Http::Endpoint::options().threads(1);
    Pistache::Http::Endpoint server(addr);
    server.init(opts);
    server.setHandler(Pistache::Http::make_handler<ComicHandler>());
    server.serve();
}

} // namespace comicService

int main()
{
    try
    {
        comicService::run();
    }
    catch (const std::exception &bang)
    {
        std::cerr << bang.what() << '\n';
        return 1;
    }
    catch (...)
    {
        return 1;
    }

    return 0;
}
