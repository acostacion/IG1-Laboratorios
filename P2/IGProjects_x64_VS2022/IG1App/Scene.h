#ifndef _H_Scene_H_
#define _H_Scene_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Camera.h"
#include "Entity.h"

#include <vector>

class Scene
{
public:
	Scene() = default;
	virtual ~Scene();

	Scene(const Scene& s) = delete;            // no copy constructor
	Scene& operator=(const Scene& s) = delete; // no copy assignment

	virtual void init();

	void render(Camera const& cam) const;

	virtual void update() {
		for (Abs_Entity* obj : gObjects)
			obj->update();
	}

	// load or unload scene data into the GPU
	void load();
	void unload();

protected:
	void destroy();
	void setGL();
	void resetGL();

	std::vector<Abs_Entity*> gObjects; // Entities (graphic objects) of the scene
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
};
#endif //_H_Scene_H_
