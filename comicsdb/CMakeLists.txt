add_executable(comicService
  comicService.cpp
  comicsdb.cpp
  comic.h
  comic.cpp
)
target_link_libraries(comicService PRIVATE pistache::pistache rapidjson)
