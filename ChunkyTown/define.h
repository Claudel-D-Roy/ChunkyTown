#ifndef DEFINE_H__
#define DEFINE_H__

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#ifdef _WIN32
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#else
#endif

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 256
#define CHUNK_SIZE_Z 16

typedef uint8_t BlockType;

enum T {BTYPE_AIR,
		BTYPE_BRICKS,
		BTYPE_COBBLE,
		BTYPE_CONCRETE,
		BTYPE_DIRT,	
		BTYPE_GOLD,
		BTYPE_GRASS,
		BTYPE_ICE,
		BTYPE_IRON,
		BTYPE_LEAVES,
		BTYPE_MARBLEBLACK,
		BTYPE_MARBLEWHITE,
		BTYPE_MOON,
		BTYPE_SAND,
		BTYPE_SNOW,
		BTYPE_STONE,
		BTYPE_WOOD,
		BTYPE_LAST
};

#define SHADER_PATH "../ChunkyTown/media/shaders/"
#define TEXTURE_PATH "../ChunkyTown/media/textures/"
#define VIEW_DISTANCE 64
#define MAX_SELECTION_DISTANCE 6

#endif // DEFINE_H__
