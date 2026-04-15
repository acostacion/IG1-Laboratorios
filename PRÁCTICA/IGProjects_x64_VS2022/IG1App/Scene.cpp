#include "Scene.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void
Scene::init()
{
	setGL(); // OpenGL settings

	// allocate memory and load resources
	// Lights
	// Textures

	// Graphics objects (entities) of the scene
	gObjects.push_back(new RGBAxes(400.0));
}

Scene::~Scene()
{
	destroy();
	resetGL();
}

void
Scene::destroy()
{ // release memory and resources

	for (Abs_Entity* el : gObjects)
		delete el;
	gObjects.clear();

	for (Abs_Entity* el : gObjectsTrans)
		delete el;
	gObjectsTrans.clear();

	for (Texture* t : gTextures)
		delete t;
	gTextures.clear();
}

void
Scene::load() {
	for (Abs_Entity* obj : gObjects)
		obj->load();

	for (Abs_Entity* obj : gObjectsTrans)
		obj->load();
}

void
Scene::unload() {
	for (Abs_Entity* obj : gObjects)
		obj->unload();

	for (Abs_Entity* obj : gObjectsTrans)
		obj->unload();
}

void
Scene::setGL() {
	// OpenGL basic setting
	glClearColor(0.6, 0.7, 0.8, 1.0); // background color (alpha=1 -> opaque)
	glEnable(GL_DEPTH_TEST);          // enable Depth test
}
void
Scene::resetGL() {
	glClearColor(.0, .0, .0, .0); // background color (alpha=1 -> opaque)
	glDisable(GL_DEPTH_TEST);     // disable Depth test
}

void
Scene::render(Camera const& cam) const {
	cam.upload();

	// --- objetos opacos
	for (Abs_Entity* el : gObjects)
		el->render(cam.viewMat());

	// --- blending objetos translucidos
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // activa blend func antes de renderizar objetos translucidos
	glDepthMask(GL_FALSE);							   // 

	// translucidos -> despues objetos con transparencia
	for (Abs_Entity* el : gObjectsTrans)
		el->render(cam.viewMat());

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glClearColor(0.6, 0.7, 0.8, 1.0);
}

void Scene0::init() {
	Scene::init();
}

void Scene1::init() {
	Scene::init();

	// hexagono magenta.
	RegularPolygon* hex = new RegularPolygon(6, 200);
	hex->setColor({ 1.0f, 0.0f, 1.0f, 1.0f });
	gObjects.push_back(hex);

	// circunferencia amarilla.
	RegularPolygon* circ = new RegularPolygon(40, 200);
	circ->setColor({ 1.0f, 1.0f, 0.0f, 1.0f });
	gObjects.push_back(circ);
}

void Scene2::init() {
	Scene::init();
	RGBTriangle* tri = new RGBTriangle(50, 215);
	gObjects.push_back(tri);

	RGBRectangle* rect = new RGBRectangle(300, 300);
	gObjects.push_back(rect);

	RegularPolygon* circ = new RegularPolygon(40, 215);
	circ->setColor({ 1.0f, 0.0f, 1.0f, 1.0f });
	gObjects.push_back(circ);
}

void Scene3::init() {
	Scene::init();

	/*Cube* cube = new Cube(200);
	cube->setColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	gObjects.push_back(cube);*/

	RGBCube* rgbCube = new RGBCube(200);
	gObjects.push_back(rgbCube);

}

void Scene4::init() {
	Scene::init();

	//createBoxOutline(200);
	createPhoto(100, 100);
	createGlassParapet(600);
	createBox(200);
	createGrass(200);
	createGround(600, 600);
	createStar(100, 5, 65);
}

#pragma region P4 Aux
void Scene4::createGround(GLdouble width, GLdouble height) {
	Ground* ground = new Ground(width, height);
	gObjects.push_back(ground);
}

void Scene4::createStar(GLdouble re, GLuint np, GLdouble h) {
	Star3D* star = new Star3D(re, np, h);
	Texture* starTex = new Texture();
	starTex->load("..\\assets\\images\\rueda.png");
	gTextures.push_back(starTex);
	star->setTexture(starTex);
	star->setBaseTransform(
		glm::translate(glm::mat4(1), glm::vec3(150, 100, 150)) *
		glm::scale(glm::mat4(1), glm::vec3(0.5f, 0.5f, 0.5f))
	);
	gObjects.push_back(star);
}

void Scene4::createBoxOutline(GLdouble length) {
	// --- texturas
	// creamos y cargamos (con load()) las texturas de los objetos de la escena
	
	// ----> textura por fuera <----
	Texture* texC = new Texture();								// crea nueva textura
	const std::string con = "../assets/images/papelE.png";	// ruta de la textura
	texC->load(con, 255);									// carga la textura con su alfa
	gTextures.push_back(texC);									// lo metemos en el vector de texturas de la escena para poder eliminarla luego

	// ----> textura por dentro <---
	Texture* texP = new Texture();								// crea nueva textura
	const std::string pap = "../assets/images/container.jpg";		// ruta de la textura
	texP->load(pap, 255);									// carga la textura con su alfa
	gTextures.push_back(texP);									// lo metemos en el vector de texturas de la escena para poder eliminarla luego

	// --- entidad
	BoxOutline* bo = new BoxOutline(length);
	bo->setTexture(texC);	// establece la textura de esta entidad
	bo->setTextureInterior(texP); // textura para el interior

	// --- traslaciones, escalas y rotaciones
	//bo->setModelMat(translate(glm::dmat4(1), glm::dvec3(80, 30 / 2, 80))); TODO luego
	gObjects.push_back(bo); // mete la entidad en la escena
}

void Scene4::createPhoto(GLdouble w, GLdouble h) {
	Texture* texF = new Texture();								// crea nueva textura
	texF->loadColorBuffer(800.0, 600.0);
	gTextures.push_back(texF);									// lo metemos en el vector de texturas
	Photo* foto = new Photo(w, h);								// entidad
	foto->setTexture(texF);									    // establece la textura de esta entidad
	gObjects.push_back(foto);									// mete la entidad en la escena
}

void Scene4::createGlassParapet(GLdouble length) {
	Texture* texG = new Texture();										// crea nueva textura
	const std::string win = "../assets/images/windowC.png";				// ruta de la textura
	texG->load(win, 100);											// carga la textura con su alfa 255 opaco
	gTextures.push_back(texG);											// lo metemos en el vector de texturas 
	GlassParapet* gla = new GlassParapet(length);		// entidad
	gla->setTexture(texG);												// establece la textura de esta entidad
	gla->setModelMat(translate(glm::dmat4(1), glm::dvec3(0, 100, 0)));
    gla->setModelMat(glm::scale(glm::mat4(1.0), glm::vec3(1.0f, 0.2f, 1.0f)) * gla->modelMat());
	gObjectsTrans.push_back(gla);										// mete la entidad en la escena
}

void Scene4::createBox(GLdouble length) {
	// --- texturas
	// creamos y cargamos (con load()) las texturas de los objetos de la escena
	// ----> textura por fuera <----
	Texture* texCT = new Texture();									// crea nueva textura
	const std::string conT = "../assets/images/papelE.png";	// ruta de la textura
	texCT->load(conT, 255);								// carga la textura con su alfa
	gTextures.push_back(texCT);										// al vector de texturas de la escena
	// ----> textura por dentro <---
	Texture* texPT = new Texture();									// crea nueva textura
	const std::string papT = "../assets/images/container.jpg";		// ruta de la textura
	texPT->load(papT, 255);								// carga la textura con su alfa
	gTextures.push_back(texPT);										// sl vector de texturas de la escena

	// --- entidad
	Box* boT = new Box(length);
	boT->setTexture(texCT);	// establece la textura de esta entidad
	boT->setTextureInterior(texPT); // textura para el interior
	boT->setModelMat(translate(glm::dmat4(1), glm::dvec3(150, 101, 150)));
	boT->setBaseTranslation(translate(glm::mat4(1), glm::vec3(150, 101, 150)));
	boT->setModelMatAbj(translate(glm::dmat4(1), glm::dvec3(150, 101, 150)));

	gObjects.push_back(boT); // mete la entidad en la escena
}

void Scene4::createGrass(GLdouble length) {
	Texture* texH = new Texture();								// crea nueva textura
	const std::string h = "../assets/images/grass.png";	// ruta de la textura
	texH->load(h, 255);									// carga la textura con su alfa 255 opaco
	gTextures.push_back(texH);									// lo metemos en el vector de texturas 
	Grass* hier = new Grass(length);				// entidad
	hier->setTexture(texH);										// establece la textura de esta entidad
	hier->setModelMat(translate(glm::dmat4(1), glm::dvec3(-150, 100, 150)));
	gObjects.push_back(hier);									// mete la entidad en la escena
}
#pragma endregion

void Scene5::init() {
	Scene::init();

	Torus* torus = new Torus(200, 50);
	torus->setModelMat(glm::rotate(glm::dmat4(1), radians(-180.0), dvec3(0.0, 1.0, 0.0)));
	gObjects.push_back(torus);

}

void Scene6::init() {
	Scene::init();

	IndexedBox* indexedBox = new IndexedBox(200);
	gObjects.push_back(indexedBox);
}

void Scene7::init() {
	Scene::init();

	Robot* r = new Robot(100);
	gObjects.push_back(r);
}

void Scene8::init() {
	Scene::init();
	//Sphere* planeta = new Sphere(100, 20, 20);
	//gObjects.push_back(planeta);

	//Robot* r = new Robot(100);
	//r->setModelMat(glm::translate(glm::dmat4(1), glm::dvec3(0, 100, 0)));
	//gObjects.push_back(r);

	GLdouble radioPlaneta = 200.0;
	GLdouble radioRobot = 30.0;

	// --- planeta
	mPlaneta = new Sphere(radioPlaneta, 40, 40);
	mPlaneta->setColor({ 171.0f / 255.0f, 33.0f / 255.0f, 72.0f / 255.0f, 1.0f });
	gObjects.push_back(mPlaneta);

	// --- nodo ficticio: se encarga de posicionar el droide en el polo norte
	// El polo norte es (0, radioPlaneta, 0)
	mNodoFicticio = new CompoundEntity();
	mNodoFicticio->setModelMat(
		glm::translate(glm::dmat4(1), glm::dvec3(0, radioPlaneta + 30, 0))
	);

	// --- droide encima del polo norte
	mRobot = new Robot(radioRobot);
	static_cast<CompoundEntity*>(mNodoFicticio)->addEntity(mRobot);

	gObjects.push_back(mNodoFicticio);

}


void Scene8::render(Camera const& cam) const {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Scene::render(cam);
}

void Scene9::init() {
	Scene::init();

	SnowMan* sm = new SnowMan(100);
	gObjects.push_back(sm);
}
