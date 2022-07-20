#pragma once

#include "comic.h"

#include <vector>

namespace comicsdb
{

using ComicDb = std::vector<Comic>;

ComicDb load();
Comic readComic(ComicDb &db, std::size_t id);
void deleteComic(ComicDb &db, std::size_t id);
void updateComic(ComicDb &db, std::size_t id, const Comic &comic);
void createComic(ComicDb &db, Comic &&comic);

} // namespace comicsdb
