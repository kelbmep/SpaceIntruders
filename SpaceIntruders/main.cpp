// SpaceIntruders.cpp: определяет точку входа для приложения.
//
#include "ImaginaryEngine/src/engine.hpp"
#include <tiny_obj_loader.cc>

using namespace std;

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

void read_file(std::string_view file_name, tinyobj::ObjReader& reader) {
	tinyobj::ObjReaderConfig reader_config;
	reader_config.mtl_search_path = "./"; // Path to material files

	if (!reader.ParseFromFile(file_name.data(), reader_config)) {
		if (!reader.Error().empty()) {
			std::cerr << "TinyObjReader: " << reader.Error();
		}
		exit(1);
	}

	if (!reader.Warning().empty()) {
		std::cout << "TinyObjReader: " << reader.Warning();
	}
}

std::vector<Engine::VertexTriangle> get_triangle_list(std::string file_name) {
	tinyobj::ObjReader reader;
	read_file(file_name, reader);

	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();
	//auto& materials = reader.GetMaterials();

	std::vector<Engine::VertexTriangle> res;
	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

			Engine::VertexTriangle trian;
			
			/*trian.setFirst({static_cast<int>(attrib.vertices[3 * size_t(shapes[s].mesh.indices[index_offset + 0].vertex_index) + 0]),
						static_cast<int>(attrib.vertices[3 * size_t(shapes[s].mesh.indices[index_offset + 0].vertex_index) + 1]) });

			trian.setSecond({ static_cast<int>(attrib.vertices[3 * size_t(shapes[s].mesh.indices[index_offset + 1].vertex_index) + 0]),
						static_cast<int>(attrib.vertices[3 * size_t(shapes[s].mesh.indices[index_offset + 1].vertex_index) + 1]) });

			trian.setThird({ static_cast<int>(attrib.vertices[3 * size_t(shapes[s].mesh.indices[index_offset + 2].vertex_index) + 0]),
						static_cast<int>(attrib.vertices[3 * size_t(shapes[s].mesh.indices[index_offset + 2].vertex_index) + 1]) });*/

			trian.arr[0].x = attrib.vertices[3 * size_t(shapes[s].mesh.indices[index_offset + 0].vertex_index) + 0];
			trian.arr[0].y = attrib.vertices[3 * size_t(shapes[s].mesh.indices[index_offset + 0].vertex_index) + 1];
			trian.arr[1].x = attrib.vertices[3 * size_t(shapes[s].mesh.indices[index_offset + 1].vertex_index) + 0];
			trian.arr[1].y = attrib.vertices[3 * size_t(shapes[s].mesh.indices[index_offset + 1].vertex_index) + 1];
			trian.arr[2].x = attrib.vertices[3 * size_t(shapes[s].mesh.indices[index_offset + 2].vertex_index) + 0];
			trian.arr[2].y = attrib.vertices[3 * size_t(shapes[s].mesh.indices[index_offset + 2].vertex_index) + 1];

			res.push_back(trian);
			index_offset += fv;
		}
	}
	return res;
}

int main(int argc, char* argv[])
{
	Engine engine{};
	int mode = 0;
	if (argc > 1)
	{
		mode = std::stoi(argv[1]);
	}

	engine.init("Mace Windows", SCREEN_WIDTH, SCREEN_HEIGHT, mode);
	engine.drawObj(get_triangle_list("../../../../SpaceIntruders/ImaginaryEngine/src/african_head.obj"));

	while (engine.isActive()) 
	{
		engine.update();
	}
	return 0;
}
