#include "SceneParser.h"
#include "../Objects/2D/Camera2D.h"
#include "../Objects/2D/VisualInstance2D/Sprite2D.h"
#include "../Assets/Image/FileImage.h"
#include "../Assets/Material/Material.h"
#include "../Assets/Mesh2D/BoxMesh2D.h"
#include "../Assets/Script/Script.h"
#include "../Core/Engine.h"
#include "../Assets/Mesh2D/CircleMesh2D.h"

#define CHECK(t) \
if (#t == type)
#define GORD(name, cls, defalt) cls name##def = defalt; auto name##it = values.find(#name); cls name = *(name##it != values.end() ? static_cast<cls*>(name##it->second) : &name##def);

SceneEntryInst SceneParser::BuildInstance(const std::string& type, LoadableValues& values) {
	// Objects
	CHECK(Sprite2D) {
		Sprite2D* sprite2d = new Sprite2D;
		sprite2d->Load(values);
		SceneEntryInst inst = {
			SceneEntryInst::Kind::Object,
			sprite2d
		};
		return inst;
	}
	CHECK(VisualInstance2D) {
		VisualInstance2D* visual_inst2D = new VisualInstance2D;
		visual_inst2D->Load(values);
		SceneEntryInst inst = {
			SceneEntryInst::Kind::Object,
			visual_inst2D
		};
		return inst;
	}
	CHECK(Camera2D) {
		Camera2D* cam2D = new Camera2D;
		cam2D->Load(values);
		SceneEntryInst inst = {
			SceneEntryInst::Kind::Object,
			cam2D
		};
		return inst;
	}
	CHECK(Object2D) {
		Object2D* obj2D = new Object2D;
		obj2D->Load(values);
		SceneEntryInst inst = {
			SceneEntryInst::Kind::Object,
			obj2D
		};
		return inst;
	}
	CHECK(Object) {
		Object* obj = new Object;
		obj->Load(values);
		SceneEntryInst inst = {
			SceneEntryInst::Kind::Object,
			obj
		};
		return inst;
	}
	// Assets
	CHECK(FileImage) {
		GORD(filepath, std::string, "");
		FileImage* image = new FileImage(filepath);
		SceneEntryInst inst = {
			SceneEntryInst::Kind::Asset,
			image
		};
		return inst;
	}
	CHECK(Script) {
		GORD(filepath, std::string, "");
		Script* script = new Script(filepath);
		SceneEntryInst inst = {
			SceneEntryInst::Kind::Asset,
			script
		};
		return inst;
	}
	CHECK(Material) {
		GORD(vert_filepath, std::string, "");
		GORD(frag_filepath, std::string, "");
		Material* mat = new Material(vert_filepath, frag_filepath);
		SceneEntryInst inst = {
			SceneEntryInst::Kind::Asset,
			mat
		};
		return inst;
	}
	CHECK(BoxMesh2D) {
		BoxMesh2D* boxmesh = new BoxMesh2D();
		SceneEntryInst inst = {
			SceneEntryInst::Kind::Asset,
			boxmesh
		};
		return inst;
	}
	CHECK(CircleMesh2D) {
		CircleMesh2D* circlemesh = new CircleMesh2D();
		SceneEntryInst inst = {
			SceneEntryInst::Kind::Asset,
			circlemesh
		};
		return inst;
	}

	Engine::GetInstance()->PushError("Unrecognized type '" + type + "' when loading scene");
}
