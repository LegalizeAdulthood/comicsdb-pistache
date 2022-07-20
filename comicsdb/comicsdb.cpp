#include "comicsdb.h"

#include <iostream>
#include <mutex>
#include <string>
#include <vector>

namespace comicsdb
{

std::mutex g_dbMutex;
using ComicDb = std::vector<Comic>;

ComicDb load()
{
    ComicDb db;
    db.emplace_back(fromJson(
        R"json({"title":"The Fantastic Four","issue":1,"writer":"Stan Lee","penciler":"Jack Kirby","inker":"George Klein","letterer":"Artie Simek","colorist":"Stan Goldberg"})json"));
    {
        Comic comic;
        comic.title = "The Fantastic Four";
        comic.issue = 3;
        comic.writer = "Stan Lee";
        comic.penciler = "Jack Kirby";
        comic.inker = "Sol Brodsky";
        comic.letterer = "Artie Simek";
        comic.colorist = "Stan Goldberg";
        db.push_back(comic);
    }
    return db;
}

bool validId(const ComicDb &db, std::size_t &id)
{
    return id < db.size() && db[id].issue != Comic::DELETED_ISSUE;
}

Comic readComic(ComicDb &db, std::size_t id)
{
    std::unique_lock<std::mutex> lock(g_dbMutex);

    if (!validId(db, id))
    {
        throw std::runtime_error("Invalid id " + std::to_string(id));
    }

    return db[id];
}

void deleteComic(ComicDb &db, std::size_t id)
{
    std::unique_lock<std::mutex> lock(g_dbMutex);

    if (!validId(db, id))
    {
        throw std::runtime_error("Invalid id " + std::to_string(id));
    }

    db[id] = Comic{};
}

void updateComic(ComicDb &db, std::size_t id, const Comic &comic)
{
    if (comic.title.empty() || comic.issue < 1 ||
        comic.writer.empty() || comic.penciler.empty() ||
        comic.inker.empty() || comic.letterer.empty() ||
        comic.colorist.empty())
    {
        throw std::runtime_error("Invalid comic");
    }

    std::unique_lock<std::mutex> lock(g_dbMutex);
    db[id] = comic;
}

std::size_t createComic(ComicDb &db, Comic &&comic)
{
    if (comic.title.empty() || comic.issue < 1 ||
        comic.writer.empty() || comic.penciler.empty() ||
        comic.inker.empty() || comic.letterer.empty() ||
        comic.colorist.empty())
    {
        throw std::runtime_error("Invalid comic");
    }

    std::unique_lock<std::mutex> lock(g_dbMutex);
    // ids are zero-based
    const std::size_t id = db.size();
    db.push_back(comic);
    return id;
}

} // namespace comicsdb
