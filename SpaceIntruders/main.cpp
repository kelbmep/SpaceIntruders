// SpaceIntruders.cpp: определяет точку входа для приложения.
//
#include <engine.hpp>
#include <sprite.hpp>
#include <render.hpp>
#include <window.hpp>
#include <tank.hpp>
#include <chrono>
#include <tiny_obj_loader.cc>

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

/*void read_file(std::string file_name, tinyobj::ObjReader& reader)
{
	tinyobj::ObjReaderConfig reader_config;
	reader_config.mtl_search_path = "./"; // Path to material files

	if (!reader.ParseFromFile(file_name.data(), reader_config)) 
	{
		if (!reader.Error().empty()) 
		{
			std::cerr << "TinyObjReader: " << reader.Error();
		}
		exit(1);
	}

	if (!reader.Warning().empty()) {
		std::cout << "TinyObjReader: " << reader.Warning();
	}
}*/

/*std::vector<Engine::VertexTriangle> get_triangle_list(std::string file_name)
{
	tinyobj::ObjReader reader;
	read_file(file_name, reader);

	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();
	//auto& materials = reader.GetMaterials();

	std::vector<Engine::VertexTriangle> res;
	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) 
	{
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) 
		{
			size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

			Engine::VertexTriangle trian;
			
			/*trian.setFirst({static_cast<int>(attrib.vertices[3 * size_t(shapes[s].mesh.indices[index_offset + 0].vertex_index) + 0]),
						static_cast<int>(attrib.vertices[3 * size_t(shapes[s].mesh.indices[index_offset + 0].vertex_index) + 1]) });

			trian.setSecond({ static_cast<int>(attrib.vertices[3 * size_t(shapes[s].mesh.indices[index_offset + 1].vertex_index) + 0]),
						static_cast<int>(attrib.vertices[3 * size_t(shapes[s].mesh.indices[index_offset + 1].vertex_index) + 1]) });

			trian.setThird({ static_cast<int>(attrib.vertices[3 * size_t(shapes[s].mesh.indices[index_offset + 2].vertex_index) + 0]),
						static_cast<int>(attrib.vertices[3 * size_t(shapes[s].mesh.indices[index_offset + 2].vertex_index) + 1]) })/

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
*/
int main(int argc, char* argv[])
{
	std::shared_ptr<EventManager> evm = std::make_shared<EventManager>();
	Engine engine{ evm };
	//Tank tank{ engine };
	
	int mode = 0;
	if (argc > 1)
	{
		mode = std::stoi(argv[1]);
	}

	engine.init("Mace Windows", SCREEN_WIDTH, SCREEN_HEIGHT, mode);
	
	std::shared_ptr<Tank> tank = std::make_shared<Tank>(engine);

	evm->add_delegate(tank.get());

	engine.get_scene()->addNode(tank);
	//engine.drawObj(get_triangle_list("../../../../SpaceIntruders/ImaginaryEngine/src/african_head.obj"));

	/*auto tank_body = std::make_shared<Sprite>(engine, "../../../../SpaceIntruders/ImaginaryEngine/src/img/tank_body.png");
	auto tank_tower = std::make_shared<Sprite>(engine, "../../../../SpaceIntruders/ImaginaryEngine/src/img/tank_tower.png");
	tank_body->set_position(glm::vec2(engine.get_window_width() * 0.5f,
		engine.get_window_height() * 0.5f));

	tank_body->set_scale(glm::vec2(1.5f, 1.5f));

	glm::vec2 tank_body_size = tank_body->get_size();
	glm::vec2 tank_tower_size = tank_tower->get_size();
	glm::vec2 tank_body_scale = tank_body->get_scale();

	tank_tower->set_position(glm::vec2(tank_body_size.x * 0.5f, tank_body_size.y * 0.539f));
	tank_tower->set_anchor(glm::vec2(0.5f, 0.694f));

	engine.get_scene()->addNode(tank_body);
	tank_body->addNode(tank_tower);*/

	while (engine.isActive()) 
	{
		engine.update();
	}
	return 0;
}
