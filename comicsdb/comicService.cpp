#include "comic.h"
#include "comicsdb.h"

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/mime.h>
#include <pistache/net.h>
#include <pistache/router.h>

#include <memory>
#include <string>
#include <thread>

namespace comicService
{

class Service
{
public:
    Service(uint16_t portNum = 8000, unsigned int numThreads = std::thread::hardware_concurrency())
        : m_portNum(portNum),
        m_numThreads(numThreads),
        m_address("localhost", portNum),
        m_endPoint(std::make_shared<Pistache::Http::Endpoint>(m_address)),
        m_db(comicsdb::load())
    {
    }

    void run();

private:
    void configureRoutes();

    using Request = Pistache::Rest::Request;
    using Response = Pistache::Http::ResponseWriter;
    void getComic(const Request &request, Response response);
    void deleteComic(const Request &request, Response response);
    void updateComic(const Request &request, Response response);

    uint16_t m_portNum;
    unsigned int m_numThreads;
    Pistache::Address m_address;
    std::shared_ptr<Pistache::Http::Endpoint> m_endPoint;
    Pistache::Rest::Router m_router;
    comicsdb::ComicDb m_db;
};

void Service::configureRoutes()
{
    Pistache::Rest::Routes::Get(m_router, "/comic/:id", Pistache::Rest::Routes::bind(&Service::getComic, this));
    Pistache::Rest::Routes::Delete(m_router, "/comic/:id", Pistache::Rest::Routes::bind(&Service::deleteComic, this));
    Pistache::Rest::Routes::Put(m_router, "/comic/:id", Pistache::Rest::Routes::bind(&Service::updateComic, this));
}

void Service::getComic(const Request &request, Response response)
{
    std::size_t id = request.param(":id").as<std::size_t>();
    try
    {
        comicsdb::Comic comic = comicsdb::readComic(m_db, id);
        response.send(Pistache::Http::Code::Ok, comicsdb::toJson(comic), MIME(Application, Json));
    }
    catch (const std::runtime_error &bang)
    {
        response.send(Pistache::Http::Code::Not_Found, bang.what(), MIME(Text, Plain));
    }
    catch (...)
    {
        response.send(Pistache::Http::Code::Internal_Server_Error, "Internal error", MIME(Text, Plain));
    }
}

void Service::deleteComic(const Request &request, Response response)
{
    std::size_t id = request.param(":id").as<std::size_t>();
    try
    {
        comicsdb::deleteComic(m_db, id);
        response.send(Pistache::Http::Code::Ok, "Comic " + std::to_string(id) + " deleted.", MIME(Text, Plain));
    }
    catch (const std::runtime_error &bang)
    {
        response.send(Pistache::Http::Code::Not_Found, bang.what(), MIME(Text, Plain));
    }
    catch (...)
    {
        response.send(Pistache::Http::Code::Internal_Server_Error, "Internal error", MIME(Text, Plain));
    }
}

void Service::updateComic(const Request &request, Response response)
{
    std::size_t id = request.param(":id").as<std::size_t>();
    try
    {
        std::string json = request.body();
        comicsdb::Comic comic(comicsdb::fromJson(json));
        comicsdb::updateComic(m_db, id, comic);
        response.send(Pistache::Http::Code::Ok, "Comic " + std::to_string(id) + " updated.", MIME(Text, Plain));
    }
    catch (const std::runtime_error &bang)
    {
        response.send(Pistache::Http::Code::Not_Found, bang.what(), MIME(Text, Plain));
    }
    catch (...)
    {
        response.send(Pistache::Http::Code::Internal_Server_Error, "Internal error", MIME(Text, Plain));
    }
}

void Service::run()
{
    std::cout << "Starting on port " << m_portNum << " with " << m_numThreads << " threads.\n";
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
