#include "Scene.h"

Scene::Scene(const char* path) {
	JSON::MutableDocument document;
	JSON::MutableArray root(document);
	root.setDocRoot(document);

	document.writeTo(path);
}