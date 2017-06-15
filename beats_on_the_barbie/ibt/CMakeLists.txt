
set(APP
	ibt)
set(APP_SOURCES
	ibt_frontend.cpp
	ibt.cpp
)

marsyas_app(${APP} "${APP_SOURCES}")

target_link_libraries(${APP} sndfile)
target_link_libraries(${APP} wiringPi)
target_link_libraries(${APP} crypt)
