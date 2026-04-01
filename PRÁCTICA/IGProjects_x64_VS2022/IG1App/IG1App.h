#ifndef _H_IG1App_H_
#define _H_IG1App_H_

#include <GL/glew.h>     // OpenGL Extension Wrangler
#include <GLFW/glfw3.h>  // OpenGL Library
#include <glm/glm.hpp>   // OpenGL Mathematics

#include "Camera.h"
#include "Scene.h"
#include "Viewport.h"


class IG1App
{
public:

	static constexpr double FRAME_DURATION = 1.0 / 60.0; // 60 FPS

	bool mUpdateEnabled = true;
	double mNextUpdate = 0.0;

	// static single instance (singleton pattern)
	static IG1App s_ig1app;
	IG1App(IG1App const& J) = delete;         // no copy constructor
	void operator=(IG1App const& J) = delete; // no copy assignment

	// Viewport position and size
	Viewport const& viewPort();
	// Camera position, view volume and projection
	Camera const& camera();
	// Graphics objects of the scene
	Scene const& scene();

	// Change to the given scene
	bool changeScene(size_t sceneNr);

	void run();   // the main event processing loop
	void close(); // the application

	// 2 vistas
	bool m2Vistas = false;
	void display2V() const; 

	// 2 escenas.
	bool m2Scenes = false;
	void display2S(Scene* s1, Scene* s2) const; 

protected:
	IG1App() = default;
	~IG1App() { close(); };

	void init();
	void iniWinOpenGL();
	void destroy();

	void display() const;                      // the scene
	void resize(int newWidth, int newHeight);  // the viewport (without changing the scale)
	void key(unsigned int codepoint);          // keypress event
	void specialkey(int key, int scancode, int action, int mods); // special keypress event

	// static callbacks
	static void s_display(GLFWwindow*) { s_ig1app.display(); };
	static void s_resize(GLFWwindow*, int newWidth, int newHeight) { s_ig1app.resize(newWidth, newHeight); };
	static void s_key(GLFWwindow* win, unsigned int codepoint) { s_ig1app.key(codepoint); };
	static void s_specialkey(GLFWwindow* win, int key, int scancode, int action, int mods) { s_ig1app.specialkey(key, scancode, action, mods); };
	static void s_mouse(GLFWwindow* win, int button, int action, int mods) { s_ig1app.mouse(button, action, mods); };
	static void s_motion(GLFWwindow* win, double x, double y) { s_ig1app.motion(x, y); };
	static void s_mouseWheel(GLFWwindow* win, double dx, double dy) { s_ig1app.mouseWheel(dx, dy); };

	void mouse(int button, int state, int mods);
	void motion(double x, double y);
	void mouseWheel(double dx, double dy);
	void fixAxisY(double& y); // arregla el eje y para lo del mouse

	void captura();


	// Viewport position and size
	Viewport* mViewPort = nullptr;
	// Camera position, view volume and projection
	Camera* mCamera = nullptr;
	Camera* mCamera2 = nullptr; // para el 2escenas
	// Graphics objects are in the scenes
	std::vector<Scene*> mScenes;
	size_t mCurrentScene = 0;

	bool mNeedsRedisplay = true;   // main event processing loop
	GLFWwindow* mWindow = nullptr; // window's handle
	int mWinW = 800;               // window's width
	int mWinH = 600;               // window's height

	glm::dvec2 mMouseCoord; // para guardar las coordenadas del raton.
	int mMouseButt = -1; // para guardar el boton pulsado 
	int mMouseMods = 0; // para guardar el control
};

inline Viewport const&
IG1App::viewPort()
{
	return *mViewPort;
}

inline Camera const&
IG1App::camera()
{
	return *mCamera;
}

inline Scene const&
IG1App::scene()
{
	return *mScenes[mCurrentScene];
}

#endif //_H_IG1App_H_
