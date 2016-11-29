cmake_minimum_required(VERSION 2.8)
project( vlc_recorder )

find_package( Boost )
include_directories( ${Boost_INCLUDE_DIR} )

add_executable( vlc_recorder 
				main.cpp 	
				source/includes.hpp	
				source/parameters.hpp		
				source/connection_manager.hpp
				source/connection_manager.cpp	
				)


target_link_libraries( 	vlc_recorder 
						boost_regex 
						boost_system 
						boost_serialization 
						boost_thread 
						boost_filesystem
						)
