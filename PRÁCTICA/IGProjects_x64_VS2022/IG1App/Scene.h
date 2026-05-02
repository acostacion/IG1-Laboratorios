#ifndef _H_Scene_H_
#define _H_Scene_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Camera.h"
#include "Entity.h"
#include "Light.h"

#include <vector>

class Scene
{
public:
	Scene() = default;
	virtual ~Scene();

	Scene(const Scene& s) = delete;            // no copy constructor
	Scene& operator=(const Scene& s) = delete; // no copy assignment

	virtual void init();

	virtual void render(Camera const& cam) const;

	virtual void update() {
		for (Abs_Entity* obj : gObjects)
			obj->update();

		for (Abs_Entity* obj : gObjectsTrans)
			obj->update();
	}

	// load or unload scene data into the GPU
	void load();
	void unload();

	void toggleDirLight() {
		if (mDirLight)
			mDirLight->setEnabled(!mDirLight->enabled());
	}

protected:
	void uploadLights(Camera const& cam) const;

	void destroy();
	void setGL();
	void resetGL();

	std::vector<Abs_Entity*> gObjects;		// Entities (graphic objects) of the scene
	std::vector<Texture*> gTextures;		// texturas de la escena
	std::vector<Abs_Entity*> gObjectsTrans; // Entidades con transparencias
	std::vector<Light*> gLights;			// Luces

	DirLight* mDirLight = nullptr;
};

class Scene0 : public Scene {
public:
	void init() override;
};

class Scene1 : public Scene {
public:
	void init() override;
};

class Scene2 : public Scene {
public:
	void init() override;
};

class Scene3 : public Scene {
public:
	void init() override;
};

class Scene4 : public Scene {
public:
	void init() override;

private:
	void createGround(GLdouble width, GLdouble height);
	void createStar(GLdouble re, GLuint np, GLdouble h);
	void createBoxOutline(GLdouble length);
	void createPhoto(GLdouble w, GLdouble h);
	void createGlassParapet(GLdouble length);
	void createBox(GLdouble length);
	void createGrass(GLdouble length);

	GlassParapet* parapet;
	Box* box;
	Star3D* star;
};

class Scene5 : public Scene {
public: void init() override;
};

class Scene6 : public Scene {
public: void init() override;
};

class Scene7 : public Scene {
public: void init() override;
};

class Scene8 : public Scene {
public: 
	void init() override;
	void render(Camera const& cam) const override;
	void rotate();
	void orbit();

private:
	Sphere* mPlaneta = nullptr;
	Robot* mRobot = nullptr;
	CompoundEntity* mNodoFicticio = nullptr;
	CompoundEntity* mNodoRotacion = nullptr;

};

class Scene9 : public Scene {
public: void init() override;
};
#endif //_H_Scene_H_
