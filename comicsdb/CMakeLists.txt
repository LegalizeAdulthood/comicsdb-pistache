add_executable(comicService
  comicService.cpp
  comicsdb.cpp
  comic.h
  comic.cpp
)
target_link_libraries(comicService PRIVATE pistache::pistache rapidjson)
set_property(TARGET comicService PROPERTY CXX_STANDARD "17")
