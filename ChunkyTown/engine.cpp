#include "engine.h"
#include "transformation.h"

#include <iostream>
#include <algorithm>
#include <cmath>
#include<fstream>

Engine::Engine() : m_player(Vector3f(VIEW_DISTANCE, 81.f, VIEW_DISTANCE)), m_shader(), m_textureAtlas(BTYPE_LAST), m_blockInfo(), m_bt1(), m_bt2(), m_bt3(), m_playerOldPos(VIEW_DISTANCE, VIEW_DISTANCE)
{

}

Engine::~Engine()
{

}

void Engine::Init()
{
	GLenum glewErr = glewInit();
	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	if (glewErr != GLEW_OK)
	{
		std::cerr << "ERREUR GLEW: " << glewGetErrorString(glewErr) << std::endl;
		abort();
	}

	glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)Width() / (float)Height(), 0.0001f, 1000.0f);
	glEnable(GL_LINE_SMOOTH);

	// Light
	GLfloat light0Pos[4] = { 0.0f, CHUNK_SIZE_Y, 0.0f, 1.0f };
	GLfloat light0Amb[4] = { 0.9f, 0.9f, 0.9f, 1.0f };
	GLfloat light0Diff[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light0Spec[4] = { 0.2f, 0.2f, 0.2f, 1.0f };

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0Amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0Spec);

	CenterMouse();
	HideCursor();
}

void Engine::DeInit()
{

}

void Engine::LoadResource()
{
	LoadTexture(m_textureCrosshair, TEXTURE_PATH "cross.bmp");
	LoadTexture(m_textureFont, TEXTURE_PATH "font.bmp");

	LoadTexture(m_face1, TEXTURE_PATH "face1.jpg");
	LoadTexture(m_face2, TEXTURE_PATH "face2.jpg");
	LoadTexture(m_face3, TEXTURE_PATH "face3.jpg");
	LoadTexture(m_face4, TEXTURE_PATH "face4.jpg");
	LoadTexture(m_face5, TEXTURE_PATH "face5.jpg");
	LoadTexture(m_face6, TEXTURE_PATH "face6.jpg");

	std::cout << "Loading and compiling shaders..." << std::endl;
	if (!m_shader.Load(SHADER_PATH "shader01.vert", SHADER_PATH "shader01.frag", true))
	{
		std::cout << "Failed to load shader" << std::endl;
		exit(1);
	}

	float u, v, h, w;

	TextureAtlas::TextureIndex texBrickIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "brick.png");
	m_textureAtlas.TextureIndexToCoord(texBrickIndex, u, v, w, h);
	m_blockInfo[BTYPE_BRICKS] = new BlockInfo(BTYPE_BRICKS, "brick", 1);
	m_blockInfo[BTYPE_BRICKS]->SetUVWH(u, v, w, h);

	TextureAtlas::TextureIndex texCobbledIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "cobble.png");
	m_textureAtlas.TextureIndexToCoord(texCobbledIndex, u, v, w, h);
	m_blockInfo[BTYPE_COBBLE] = new BlockInfo(BTYPE_COBBLE, "cobble", 1);
	m_blockInfo[BTYPE_COBBLE]->SetUVWH(u, v, w, h);

	TextureAtlas::TextureIndex texConcreteIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "concrete.png");
	m_textureAtlas.TextureIndexToCoord(texConcreteIndex, u, v, w, h);
	m_blockInfo[BTYPE_CONCRETE] = new BlockInfo(BTYPE_CONCRETE, "concrete", 1);
	m_blockInfo[BTYPE_CONCRETE]->SetUVWH(u, v, w, h);

	TextureAtlas::TextureIndex texDirtIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "dirt.png");
	m_textureAtlas.TextureIndexToCoord(texDirtIndex, u, v, w, h);
	m_blockInfo[BTYPE_DIRT] = new BlockInfo(BTYPE_DIRT, "dirt", 1);
	m_blockInfo[BTYPE_DIRT]->SetUVWH(u, v, w, h);

	TextureAtlas::TextureIndex texGoldIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "gold.png");
	m_textureAtlas.TextureIndexToCoord(texGoldIndex, u, v, w, h);
	m_blockInfo[BTYPE_GOLD] = new BlockInfo(BTYPE_GOLD, "gold", 1);
	m_blockInfo[BTYPE_GOLD]->SetUVWH(u, v, w, h);

	TextureAtlas::TextureIndex texGrassIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "grass.png");
	m_textureAtlas.TextureIndexToCoord(texGrassIndex, u, v, w, h);
	m_blockInfo[BTYPE_GRASS] = new BlockInfo(BTYPE_GRASS, "grass", 1);
	m_blockInfo[BTYPE_GRASS]->SetUVWH(u, v, w, h);

	TextureAtlas::TextureIndex texIceIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "ice.png");
	m_textureAtlas.TextureIndexToCoord(texIceIndex, u, v, w, h);
	m_blockInfo[BTYPE_ICE] = new BlockInfo(BTYPE_ICE, "ice", 1);
	m_blockInfo[BTYPE_ICE]->SetUVWH(u, v, w, h);

	TextureAtlas::TextureIndex texIronIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "iron.png");
	m_textureAtlas.TextureIndexToCoord(texIronIndex, u, v, w, h);
	m_blockInfo[BTYPE_IRON] = new BlockInfo(BTYPE_IRON, "iron", 1);
	m_blockInfo[BTYPE_IRON]->SetUVWH(u, v, w, h);

	TextureAtlas::TextureIndex texLeavesIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "leaves.png");
	m_textureAtlas.TextureIndexToCoord(texLeavesIndex, u, v, w, h);
	m_blockInfo[BTYPE_LEAVES] = new BlockInfo(BTYPE_LEAVES, "leaves", 1);
	m_blockInfo[BTYPE_LEAVES]->SetUVWH(u, v, w, h);

	TextureAtlas::TextureIndex texMarbleBlackIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "marbleblack.png");
	m_textureAtlas.TextureIndexToCoord(texMarbleBlackIndex, u, v, w, h);
	m_blockInfo[BTYPE_MARBLEBLACK] = new BlockInfo(BTYPE_MARBLEBLACK, "black marble", 1);
	m_blockInfo[BTYPE_MARBLEBLACK]->SetUVWH(u, v, w, h);

	TextureAtlas::TextureIndex texMarbleWhiteIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "marblewhite.png");
	m_textureAtlas.TextureIndexToCoord(texMarbleWhiteIndex, u, v, w, h);
	m_blockInfo[BTYPE_MARBLEWHITE] = new BlockInfo(BTYPE_MARBLEWHITE, "white marble", 1);
	m_blockInfo[BTYPE_MARBLEWHITE]->SetUVWH(u, v, w, h);

	TextureAtlas::TextureIndex texMoonIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "moon.png");
	m_textureAtlas.TextureIndexToCoord(texMoonIndex, u, v, w, h);
	m_blockInfo[BTYPE_MOON] = new BlockInfo(BTYPE_MOON, "moon", 1);
	m_blockInfo[BTYPE_MOON]->SetUVWH(u, v, w, h);

	TextureAtlas::TextureIndex texSandIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "sand.png");
	m_textureAtlas.TextureIndexToCoord(texSandIndex, u, v, w, h);
	m_blockInfo[BTYPE_SAND] = new BlockInfo(BTYPE_SAND, "sand", 1);
	m_blockInfo[BTYPE_SAND]->SetUVWH(u, v, w, h);

	TextureAtlas::TextureIndex texSnowIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "snow.png");
	m_textureAtlas.TextureIndexToCoord(texSnowIndex, u, v, w, h);
	m_blockInfo[BTYPE_SNOW] = new BlockInfo(BTYPE_SNOW, "snow", 1);
	m_blockInfo[BTYPE_SNOW]->SetUVWH(u, v, w, h);

	TextureAtlas::TextureIndex texStoneIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "stone.png");
	m_textureAtlas.TextureIndexToCoord(texStoneIndex, u, v, w, h);
	m_blockInfo[BTYPE_STONE] = new BlockInfo(BTYPE_STONE, "stone", 1);
	m_blockInfo[BTYPE_STONE]->SetUVWH(u, v, w, h);

	TextureAtlas::TextureIndex texWoodIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "wood.png");
	m_textureAtlas.TextureIndexToCoord(texWoodIndex, u, v, w, h);
	m_blockInfo[BTYPE_WOOD] = new BlockInfo(BTYPE_WOOD, "wood", 1);
	m_blockInfo[BTYPE_WOOD]->SetUVWH(u, v, w, h);

	if (!m_textureAtlas.Generate(128, false))
	{
		std::cout << "Unable to generate texture atlas..." << std::endl;
		abort();
	}
}

void Engine::UnloadResource()
{
	for (auto it = m_map.begin(); it != m_map.end(); it++)
	{
		delete it->second;
	}
}

void Engine::Render(float elapsedTime)
{
	if (elapsedTime >= 0.2f)
		return;

	static float gameTime = elapsedTime; // Nbr de secondes depuis le debut du jeu
	gameTime += elapsedTime;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear screen

	// Transformations initiales
	glMatrixMode(GL_MODELVIEW); // Transformation s'appliquent ici
	glLoadIdentity();

	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glColor4f(1, 1, 1, 1);

	UpdateChunkVisible(false);

	Transformation t;
	Vector3f pos = m_player.GetPosition();
	Vector3f delta = m_player.SimulateMove(m_keyW, m_keyS, m_keyA, m_keyD, m_jump, m_fly, elapsedTime);

	delta.y -= 10 * elapsedTime;
	delta.y += m_player.m_velocite * elapsedTime;
	m_player.m_velocite *= 0.9f;

	// Collision par rapport au déplacement en x:
	m_bt1 = BlockAt(pos.x + delta.x, pos.y, pos.z, BTYPE_AIR);
	m_bt2 = BlockAt(pos.x + delta.x, pos.y - 0.9f, pos.z, BTYPE_AIR);
	m_bt3 = BlockAt(pos.x + delta.x, pos.y - 1.7f, pos.z, BTYPE_AIR);
	if (m_bt1 != BTYPE_AIR || m_bt2 != BTYPE_AIR || m_bt3 != BTYPE_AIR)
		delta.x = 0;

	// Collision par rapport au déplacement en y:
	m_bt1 = BlockAt(pos.x, pos.y + delta.y, pos.z, BTYPE_AIR);
	m_bt2 = BlockAt(pos.x, pos.y + delta.y - 0.9f, pos.z, BTYPE_AIR);
	m_bt3 = BlockAt(pos.x, pos.y + delta.y - 1.7f, pos.z, BTYPE_AIR);
	if (m_bt1 != BTYPE_AIR || m_bt2 != BTYPE_AIR || m_bt3 != BTYPE_AIR)
	{
		delta.y = 0;
		m_player.bOnGround = true;
	}

	// Collision par rapport au déplacement en z:
	m_bt1 = BlockAt(pos.x, pos.y, pos.z + delta.z, BTYPE_AIR);
	m_bt2 = BlockAt(pos.x, pos.y - 0.9f, pos.z + delta.z, BTYPE_AIR);
	m_bt3 = BlockAt(pos.x, pos.y - 1.7f, pos.z + delta.z, BTYPE_AIR);
	if (m_bt1 != BTYPE_AIR || m_bt2 != BTYPE_AIR || m_bt3 != BTYPE_AIR)
		delta.z = 0;

	pos += delta;
	m_player.SetPosition(pos);
	m_player.ApplyTransformation(t);
	t.ApplyTranslation(0.5f, 0.5f, 0.5f);
	t.Use();

	glNormal3f(0, 1, 0);
	m_textureAtlas.Bind();

	m_shader.Use();

	UpdateChunkVisible(true);

	Shader::Disable();

	t.Push();
	t.ApplyTranslation(m_player.GetPosition().x, m_player.GetPosition().y, m_player.GetPosition().z);
	t.Use();

	// Render the front quad
	m_face1.Bind();
	glBegin(GL_QUADS);
	glTranslated(m_player.GetPosition().x, m_player.GetPosition().y, m_player.GetPosition().z);
	glTexCoord2f(0, 0); glVertex3f(VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
	glTexCoord2f(1, 0); glVertex3f(-VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
	glTexCoord2f(1, 1); glVertex3f(-VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
	glTexCoord2f(0, 1); glVertex3f(VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
	glEnd();

	// Render the left quad
	m_face2.Bind();
	glBegin(GL_QUADS);
	glTranslated(m_player.GetPosition().x, m_player.GetPosition().y, m_player.GetPosition().z);
	glTexCoord2f(0, 0); glVertex3f(-VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
	glTexCoord2f(1, 0); glVertex3f(-VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
	glTexCoord2f(1, 1); glVertex3f(-VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
	glTexCoord2f(0, 1); glVertex3f(-VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
	glEnd();

	// Render the back quad
	m_face3.Bind();
	glBegin(GL_QUADS);
	glTranslated(m_player.GetPosition().x, m_player.GetPosition().y, m_player.GetPosition().z);
	glTexCoord2f(0, 0); glVertex3f(-VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
	glTexCoord2f(1, 0); glVertex3f(VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
	glTexCoord2f(1, 1); glVertex3f(VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
	glTexCoord2f(0, 1); glVertex3f(-VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
	glEnd();

	// Render the right quad
	m_face4.Bind();
	glBegin(GL_QUADS);
	glTranslated(m_player.GetPosition().x, m_player.GetPosition().y, m_player.GetPosition().z);
	glTexCoord2f(0, 0); glVertex3f(VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
	glTexCoord2f(1, 0); glVertex3f(VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
	glTexCoord2f(1, 1); glVertex3f(VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
	glTexCoord2f(0, 1); glVertex3f(VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
	glEnd();

	// Render the top quad
	m_face5.Bind();
	glBegin(GL_QUADS);
	glTranslated(m_player.GetPosition().x, m_player.GetPosition().y, m_player.GetPosition().z);
	glTexCoord2f(0, 0); glVertex3f(-VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
	glTexCoord2f(1, 0); glVertex3f(-VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
	glTexCoord2f(1, 1); glVertex3f(VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
	glTexCoord2f(0, 1); glVertex3f(VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
	glEnd();

	// Render the bottom quad
	m_face6.Bind();
	glBegin(GL_QUADS);
	glTranslated(m_player.GetPosition().x, m_player.GetPosition().y, m_player.GetPosition().z);
	glTexCoord2f(0, 0); glVertex3f(VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
	glTexCoord2f(1, 0); glVertex3f(VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
	glTexCoord2f(1, 1); glVertex3f(-VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
	glTexCoord2f(0, 1); glVertex3f(-VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
	glEnd();

	t.Pop();
	t.Use();

	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	DrawHud(elapsedTime);
	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GetBlockAtCursor();
}

void Engine::KeyPressEvent(unsigned char key)
{
	switch (key)
	{
	case 0: // Gauche
		m_keyD = true;
		m_keyS = false;
		break;
	case 3: // Droite
		m_keyA = true;
		m_keyS = false;
		break;
	case 22: // Devant
		m_keyW = true;
		m_keyA = false;
		break;
	case 18: // Derri�re
		m_keyS = true;
		m_keyW = false;
		break;
	case 5: // Voler
		m_fly = true;
		break;
	case 57: // Sauter
		m_jump = true;
		break;
	case 36: // ESC
		Stop();
		break;
	case 94: // F10
		SetFullscreen(!IsFullscreen());
		break;
	default:
		std::cout << "Unhandled key: " << (int)key << std::endl;
	}
}

void Engine::KeyReleaseEvent(unsigned char key)
{
	switch (key)
	{
	case 0:
		m_keyD = false;
		break;
	case 3:
		m_keyA = false;
		break;
	case 22:
		m_keyW = false;
		break;
	case 18:
		m_keyS = false;
		break;
	case 5: //FLY = touche f
		m_fly = false;
		break;
	case 57:
		m_jump = false;
		break;
	case 24: // Y
		m_wireframe = !m_wireframe;
		if (m_wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}
}

void Engine::MouseMoveEvent(int x, int y)
{
	// Centrer la souris seulement si elle n'est pas d�j� centr�e
	// Il est n�cessaire de faire la v�rification pour �viter de tomber
	// dans une boucle infinie o� l'appel � CenterMouse g�n�re un
	// MouseMoveEvent, qui rapelle CenterMouse qui rapelle un autre
	// MouseMoveEvent, etc
	if (x == (Width() / 2) && y == (Height() / 2))
		return;

	MakeRelativeToCenter(x, y);

	m_player.TurnLeftRight((float)x);
	m_player.TurnTopBottom((float)y);

	CenterMouse();
}

void Engine::MousePressEvent(const MOUSE_BUTTON& button, int x, int y)
{
	Chunk* currentChunk = ChunkAt(m_currentBlock);
	BlockType selectedBlock = BlockAt(((int)m_currentBlock.x % CHUNK_SIZE_X) + m_currentFaceNormal.x,
		((int)m_currentBlock.y % CHUNK_SIZE_Y) + m_currentFaceNormal.y,
		((int)m_currentBlock.z % CHUNK_SIZE_Z) + m_currentFaceNormal.z, BTYPE_AIR);


	if (m_currentBlock.x != -1 && m_currentBlock.y > 1)
	{
		if (button == MOUSE_BUTTON_LEFT)
		{
			currentChunk->RemoveBlock((int)m_currentBlock.x % CHUNK_SIZE_X, (int)m_currentBlock.y % CHUNK_SIZE_Y,
				(int)m_currentBlock.z % CHUNK_SIZE_Z);

			currentChunk->Modifier();
		}
		if (button == MOUSE_BUTTON_RIGHT)
		{
			if (selectedBlock != BTYPE_AIR && m_player.GetPosition().x < VIEW_DISTANCE * 2)
			{
				currentChunk->SetBlock(((int)m_currentBlock.x % CHUNK_SIZE_X) + m_currentFaceNormal.x,
					((int)m_currentBlock.y % CHUNK_SIZE_Y) + m_currentFaceNormal.y,
					((int)m_currentBlock.z % CHUNK_SIZE_Z) + m_currentFaceNormal.z, BTYPE_MARBLEBLACK);

				currentChunk->Modifier();

			}
			else if (m_player.GetPosition().x > VIEW_DISTANCE * 2) {
				currentChunk->SetBlock(((int)m_currentBlock.x % CHUNK_SIZE_X) + m_currentFaceNormal.x,
					((int)m_currentBlock.y % CHUNK_SIZE_Y) + m_currentFaceNormal.y,
					((int)m_currentBlock.z % CHUNK_SIZE_Z) + m_currentFaceNormal.z, BTYPE_MARBLEBLACK);

				currentChunk->Modifier();

			}

		}
	}

}

void Engine::MouseReleaseEvent(const MOUSE_BUTTON& button, int x, int y)
{

}

bool Engine::LoadTexture(Texture& texture, const std::string& filename, bool stopOnError)
{
	texture.Load(filename);
	if (!texture.IsValid())
	{
		std::cerr << "Unable to load texture (" << filename << ")" << std::endl;
		if (stopOnError)
			Stop();

		return false;
	}
	return true;
}

void Engine::PrintText(unsigned int x, unsigned int y, const std::string& t)
{
	glLoadIdentity();
	glTranslated(x, y, 0);

	for (unsigned int i = 0; i < t.length(); ++i)
	{
		float left = (float)((t[i] - 32) % 16) / 16.0f;
		float top = (float)((t[i] - 32) / 16) / 16.0f;
		top += 0.5f;

		glBegin(GL_QUADS);
		glTexCoord2f(left, 1.0f - top - 0.0625f);
		glVertex2f(0, 0);
		glTexCoord2f(left + 0.0625f, 1.0f - top - 0.0625f);
		glVertex2f(12, 0);
		glTexCoord2f(left + 0.0625f, 1.0f - top);
		glVertex2f(12, 12);
		glTexCoord2f(left, 1.0f - top);
		glVertex2f(0, 12);
		glEnd();
		glTranslated(8, 0, 0);
	}
}

void Engine::GetBlockAtCursor()
{
	int x = Width() / 2;
	int y = Height() / 2;

	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;

	glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	posX += .5f;
	posY += .5f;
	posZ += .5f;

	// Le cast vers int marche juste pour les valeurs entiere, utiliser une fonction de la libc si besoin
	// de valeurs negatives
	int px = (int)floor(posX);
	int py = (int)floor(posY);
	int pz = (int)floor(posZ);

	bool found = false;



	if ((m_player.GetPosition() - Vector3f((float)posX, (float)posY, (float)posZ)).Length() < MAX_SELECTION_DISTANCE)
	{


		// Apres avoir determine la position du bloc en utilisant la partie entiere du hit
		// point retourne par opengl, on doit verifier de chaque cote du bloc trouve pour trouver
		// le vrai bloc. Le vrai bloc peut etre different a cause d'erreurs de precision de nos
		// nombres flottants (si z = 14.999 par exemple, et qu'il n'y a pas de blocs a la position
		// 14 (apres arrondi vers l'entier) on doit trouver et retourner le bloc en position 15 s'il existe
		// A cause des erreurs de precisions, ils arrive que le cote d'un bloc qui doit pourtant etre a la
		// position 15 par exemple nous retourne plutot la position 15.0001
		for (int x = px - 1; !found && x <= px + 1; ++x)
		{
			for (int y = py - 1; !found && x >= 0 && y <= py + 1; ++y)
			{
				for (int z = pz - 1; !found && y >= 0 && z <= pz + 1; ++z)
				{
					if (z >= 0)
					{
					
						
						BlockType bt = BlockAt((float)x, (float)y, (float)z, BTYPE_AIR);

						if (bt == BTYPE_AIR) 
							continue;
				
						

						m_currentBlock.x = (float)x;
						m_currentBlock.y = (float)y;
						m_currentBlock.z = (float)z;

						if (InRangeWithEpsilon((float)posX, (float)x, (float)x + 1.f, 0.1f) && InRangeWithEpsilon((float)posY, (float)y, (float)y + 1.f, 0.1f) && InRangeWithEpsilon((float)posZ, (float)z, (float)z + 1.f, 0.1f))
						{
							found = true;
						}
						
					}

				}
			}
		}
	}


	

		if (!found)
		{
			m_currentBlock.x = -1;
		}
		else
		{
			// Find on which face of the bloc we got an hit
			m_currentFaceNormal.Zero();

			const float epsilon = 0.1f;

			// Front et back:
			if (EqualWithEpsilon((float)posZ, (float)m_currentBlock.z, epsilon))
				m_currentFaceNormal.z = -1;
			else if (EqualWithEpsilon((float)posZ, (float)m_currentBlock.z + 1.f, epsilon))
				m_currentFaceNormal.z = 1;
			else if (EqualWithEpsilon((float)posX, (float)m_currentBlock.x, epsilon))
				m_currentFaceNormal.x = -1;
			else if (EqualWithEpsilon((float)posX, (float)m_currentBlock.x + 1.f, epsilon))
				m_currentFaceNormal.x = 1;
			else if (EqualWithEpsilon((float)posY, (float)m_currentBlock.y, epsilon))
				m_currentFaceNormal.y = -1;
			else if (EqualWithEpsilon((float)posY, (float)m_currentBlock.y + 1.f, epsilon))
				m_currentFaceNormal.y = 1;
		}

}

void Engine::DrawHud(float elapsedTime)
{
	// Setter le blend function , tout ce qui sera noir sera transparent
	glDisable(GL_LIGHTING);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, Width(), 0, Height(), -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// Bind de la texture pour le font
	m_textureFont.Bind();
	std::ostringstream ss;
	ss << "Fps : " << (int)(1 / elapsedTime); //ElapsedTime
	PrintText(10, Height() - 25, ss.str());
	ss.str("");


	ss << "Position : " << "[X = " << ceil(m_player.GetPosition().x * 100) / 100 << "] / [Y = " << ceil(m_player.GetPosition().y * 100) / 100 << "] / [Z = " << ceil(m_player.GetPosition().z * 100) / 100 << "]";
	PrintText(10, 20, ss.str());
	ss.str("");
	ss << "ChunkCoord : " << "[Chunk_X = " << (int)(m_player.GetPosition().x / CHUNK_SIZE_X) << "] / [Chunk_Z = " << (int)(m_player.GetPosition().z / CHUNK_SIZE_Z) << "]";
	PrintText(10, 10, ss.str());

	// Affichage du crosshair
	m_textureCrosshair.Bind();
	static const int crossSize = 32;
	glLoadIdentity();
	glTranslated(Width() / 2 - crossSize / 2, Height() / 2 - crossSize / 2, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2i(0, 0);
	glTexCoord2f(1, 0);
	glVertex2i(crossSize, 0);
	glTexCoord2f(1, 1);
	glVertex2i(crossSize, crossSize);
	glTexCoord2f(0, 1);
	glVertex2i(0, crossSize);
	glEnd();
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

Chunk* Engine::ChunkAt(float x, float y, float z) const
{
	int cx = (int)x / CHUNK_SIZE_X;
	int cz = (int)z / CHUNK_SIZE_Z;

	auto it = m_map.find(Vector2f(cx, cz));
	if (it == m_map.end())
		return nullptr;
	return it->second;
}

Chunk* Engine::ChunkAt(const Vector3f& pos) const
{
	return ChunkAt(pos.x, pos.y, pos.z);
}

BlockType Engine::BlockAt(float x, float y, float z, BlockType defaultBlockType) const
{
	Chunk* c = ChunkAt(x, y, z);

	if (!c)
		return defaultBlockType;

	int bx = (int)x % CHUNK_SIZE_X;
	int by = (int)y % CHUNK_SIZE_Y;
	int bz = (int)z % CHUNK_SIZE_Z;

	return c->GetBlock(bx, by, bz);
}

bool Engine::EqualWithEpsilon(const float& v1, const float& v2, float epsilon)
{
	return (fabs(v2 - v1) < epsilon);
}

bool Engine::InRangeWithEpsilon(const float& v, const float& vinf, const float& vsup, float epsilon)
{
	return (v >= vinf - epsilon && v <= vsup + epsilon);
}
void Engine::UpdateChunkVisible(bool render) {

	m_chunkVisibleParJoueur = VIEW_DISTANCE / CHUNK_SIZE_X;

	int ChunkCoordoX = (int)(m_player.GetPosition().x / CHUNK_SIZE_X);
	int ChunkCoordoZ = (int)(m_player.GetPosition().z / CHUNK_SIZE_Z);

	m_playerOldPos.x = m_playerOldPos.x - ChunkCoordoX;
	m_playerOldPos.z = m_playerOldPos.z - ChunkCoordoZ;


	for (int xOffSet = -m_chunkVisibleParJoueur; xOffSet < m_chunkVisibleParJoueur; xOffSet++)
	{
		for (int zOffSet = -m_chunkVisibleParJoueur; zOffSet < m_chunkVisibleParJoueur; zOffSet++) {

			Vector2f CoordChunkVu(ChunkCoordoX + xOffSet, ChunkCoordoZ + zOffSet);

			auto it = m_map.find(CoordChunkVu);
			if (it == m_map.end())
			{
				Chunk* chunk = new Chunk((float)CoordChunkVu.x, (float)CoordChunkVu.z);
				m_map.insert(std::pair <Vector2f, Chunk*>(CoordChunkVu, chunk));

				if (m_map.size() > VIEW_DISTANCE)
				{
					int allo = 0;

					if (m_playerOldPos.x == -1)
					{
						int tempz = 0;
						if (m_playerOldPos.z == 1)
						{
							int tempz = CoordChunkVu.z - 1;
							int tempx = CoordChunkVu.x + (m_chunkVisibleParJoueur * 2);
							Vector2f temp(tempx, tempz);
							it = m_map.find(temp);
							if (it != m_map.end())
							{
								delete it->second;
								m_map.erase(temp);
							}
						}
						else
						{
							int tempx = CoordChunkVu.x - (m_chunkVisibleParJoueur * 2);
							Vector2f temp(tempx, CoordChunkVu.z);
							it = m_map.find(temp);
							if (it != m_map.end())
							{
								delete it->second;
								m_map.erase(temp);
							}
						}
					}
					if (m_playerOldPos.x == 1)
					{
						int tempz = 0;
						if (m_playerOldPos.z == 1)
						{
							int tempz = CoordChunkVu.z + 1;
							int tempx = CoordChunkVu.x + (m_chunkVisibleParJoueur * 2);
							Vector2f temp(tempx, tempz);
							it = m_map.find(temp);
							if (it != m_map.end())
							{

								delete it->second;
								m_map.erase(temp);
							}
						}
						else
						{
							int tempx = CoordChunkVu.x + (m_chunkVisibleParJoueur * 2);
							Vector2f temp(tempx, CoordChunkVu.z);
							it = m_map.find(temp);
							if (it != m_map.end())
							{
								delete it->second;
								m_map.erase(temp);
							}
						}
					}
					if (m_playerOldPos.z == -1)
					{

						if (m_playerOldPos.x == 1)
						{
							int tempz = CoordChunkVu.z - (m_chunkVisibleParJoueur * 2);
							int tempx = CoordChunkVu.x - 1;
							Vector2f temp(tempx, tempz);
							it = m_map.find(temp);
							if (it != m_map.end())
							{
								delete it->second;
								m_map.erase(temp);
							}
						}
						else {

							int tempz = CoordChunkVu.z - (m_chunkVisibleParJoueur * 2);
							Vector2f temp(CoordChunkVu.x, tempz);
							it = m_map.find(temp);
							if (it != m_map.end())
							{
								delete it->second;
								m_map.erase(temp);

							}
						}

					}
					if (m_playerOldPos.z == 1)
					{

						if (m_playerOldPos.x == 1)
						{
							int tempz = CoordChunkVu.z - (m_chunkVisibleParJoueur * 2);
							int tempx = CoordChunkVu.x + 1;
							Vector2f temp(tempx, tempz);
							it = m_map.find(temp);
							if (it != m_map.end())
							{
								delete it->second;
								m_map.erase(temp);
							}
						}
						else {

							int tempz = CoordChunkVu.z + (m_chunkVisibleParJoueur * 2);
							Vector2f temp(CoordChunkVu.x, tempz);
							it = m_map.find(temp);
							if (it != m_map.end())
							{
								delete it->second;
								m_map.erase(temp);

							}
						}
					}
				}


			}

			if (render)
			{
				it = m_map.find(CoordChunkVu);
				if (it->second->IsDirty())
					it->second->Update(m_blockInfo);

				it->second->Render();
			}
		}

	}
	m_playerOldPos.x = ChunkCoordoX;
	m_playerOldPos.z = ChunkCoordoZ;
};
