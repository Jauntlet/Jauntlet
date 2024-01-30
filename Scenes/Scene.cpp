#include "Scene.h"

Scene::Scene(const char* path) {
	JSON::MutableDocument document;
	JSON::MutableArray root(document);
	document.setRoot(root);

	document.writeTo(path);
}