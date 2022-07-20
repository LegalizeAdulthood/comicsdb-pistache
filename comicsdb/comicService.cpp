#include "comic.h"
#include "comicsdb.h"

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/net.h>
#include <pistache/router.h>

#include <memory>
#include <thread>

namespace comicService
{

class Service
{
public:
    Service(uint16_t portNum = 8000, unsigned int numThreads = std::thread::hardware_concurrency())
        : m_numThreads(numThreads),
        m_address("localhost", portNum),
        m_endPoint(std::make_shared<Pistache::Http::Endpoint>(m_address)),
        m_db(comicsdb::load())
    {
    }

    void run();

private:
    void configureRoutes();

    void getComic(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);

    unsigned int m_numThreads;
    Pistache::Address m_address;
    std::shared_ptr<Pistache::Http::Endpoint> m_endPoint;
    Pistache::Rest::Router m_router;
    comicsdb::ComicDb m_db;
};

void Service::configureRoutes()
{
    Pistache::Rest::Routes::Get(m_router, "/comic/:id", Pistache::Rest::Routes::bind(&Service::getComic, this));
}

void Service::getComic(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response)
{
    std::size_t id = request.param(":id").as<std::size_t>();
    try
    {
        comicsdb::Comic comic = comicsdb::readComic(m_db, id);
        response.send(Pistache::Http::Code::Ok, comicsdb::toJson(comic));
    }
    catch (const std::runtime_error &bang)
    {
        response.send(Pistache::Http::Code::Not_Found, bang.what());
    }
    catch (...)
    {
        response.send(Pistache::Http::Code::Internal_Server_Error, "Internal error");
    }
}

void Service::run()
{
    m_endPoint->init(Pistache::Http::Endpoint::options().threads(m_numThreads));
    configureRoutes();
    m_endPoint->setHandler(m_router.handler());
    m_endPoint->serve();
}

} // namespace comicService

int main()
{
    try
    {
        comicService::Service service;
        service.run();
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
