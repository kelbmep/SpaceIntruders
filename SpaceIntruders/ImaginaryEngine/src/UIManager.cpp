#include <UIManager.hpp>
#include <shaderProgram.hpp>
#include <bitmap.hpp>
#include <imgui.h>

UIManager::UIManager(const Engine& engine) : _engine{ engine }
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();
	io.BackendPlatformName = "custom_micro_engine";

	io.KeyMap[ImGuiKey_Tab] = static_cast<size_t>(KeyCode::TAB);
	io.KeyMap[ImGuiKey_LeftArrow] = static_cast<size_t>(KeyCode::LEFT);
	io.KeyMap[ImGuiKey_RightArrow] = static_cast<size_t>(KeyCode::RIGHT);
	io.KeyMap[ImGuiKey_UpArrow] = static_cast<size_t>(KeyCode::UP);
	io.KeyMap[ImGuiKey_DownArrow] = static_cast<size_t>(KeyCode::DOWN);
	io.KeyMap[ImGuiKey_PageUp] = static_cast<size_t>(KeyCode::PAGEUP);
	io.KeyMap[ImGuiKey_PageDown] = static_cast<size_t>(KeyCode::PAGEDOWN);
	io.KeyMap[ImGuiKey_Home] = static_cast<size_t>(KeyCode::HOME);
	io.KeyMap[ImGuiKey_End] = static_cast<size_t>(KeyCode::END);
	io.KeyMap[ImGuiKey_Insert] = static_cast<size_t>(KeyCode::INSERT);
	io.KeyMap[ImGuiKey_Delete] = static_cast<size_t>(KeyCode::DELETE);
	io.KeyMap[ImGuiKey_Backspace] = static_cast<size_t>(KeyCode::BACKSPACE);
	io.KeyMap[ImGuiKey_Space] = static_cast<size_t>(KeyCode::SPACE);
	io.KeyMap[ImGuiKey_Enter] = static_cast<size_t>(KeyCode::RETURN);
	io.KeyMap[ImGuiKey_Escape] = static_cast<size_t>(KeyCode::ESCAPE);
	io.KeyMap[ImGuiKey_A] = static_cast<size_t>(KeyCode::A);
	io.KeyMap[ImGuiKey_C] = static_cast<size_t>(KeyCode::C);
	io.KeyMap[ImGuiKey_V] = static_cast<size_t>(KeyCode::V);
	io.KeyMap[ImGuiKey_X] = static_cast<size_t>(KeyCode::X);
	io.KeyMap[ImGuiKey_Y] = static_cast<size_t>(KeyCode::Y);
	io.KeyMap[ImGuiKey_Z] = static_cast<size_t>(KeyCode::Z); 

	io.RenderDrawListsFn = nullptr;

	auto width = 0, height = 0;
	unsigned char* dataPtr;
	io.Fonts->GetTexDataAsRGBA32(&dataPtr, &width, &height);
	std::vector<unsigned char> image(dataPtr, dataPtr + (width * height * 4));

	Bitmap bitmap(4, std::move(image), glm::vec2{ width, height });

	_command.program = _engine.get_render().create_program("draw");

	_texture_uniform = _command.program->create_texture_uniform("uTexture");
	_texture_uniform->texture = _engine.get_render().create_texture(std::move(bitmap));

	_screen_size_uniform = _command.program->create_vec2_uniform("uScreenSize");
	_transform_uniform = _command.program->create_mat3_uniform("uTransform");

	_command.uniforms.push_back(_transform_uniform);
	_command.uniforms.push_back(_screen_size_uniform);
	_command.uniforms.push_back(_texture_uniform);

	_engine.get_event_manager().add_delegate(this);
}

void UIManager::handle_event(MouseEvent e)
{
	if (e.button == MouseButton::Left)
	{
		_isLeft = (e.type == KeyType::KeyDown);
	}

	if (e.button == MouseButton::Right)
	{
		_isRight = (e.type == KeyType::KeyDown);
	}

	if (e.button == MouseButton::Middle)
	{
		_isMiddle = (e.type == KeyType::KeyDown);
	}

	_mouse_pos.x = (float)e.x;
	_mouse_pos.y = (float)e.y;
}

void UIManager::handle_event(TextInputEvent e)
{
	ImGuiIO& io = ImGui::GetIO();
	io.AddInputCharactersUTF8(e.text.data());
}

void UIManager::handle_event(MouseMoveEvent e)
{
	_mouse_pos.x = (float)e.x;
	_mouse_pos.y = (float)e.y;
}

void UIManager::handle_event(KeyEvent e)
{
	if (e.key == KeyCode::ESCAPE && e.type == KeyType::KeyUp)
	{
		_show_main_menu = !_show_main_menu;
	}
}

void UIManager::visit()
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(float(_engine.get_window_width()), float(_engine.get_window_height()));

	io.MousePos = { _mouse_pos.x, _mouse_pos.y };

	io.MouseDown[0] = _isLeft;
	io.MouseDown[1] = _isRight;
	io.MouseDown[2] = _isMiddle;

	ImGui::NewFrame();

	//ImGui::ShowDemoWindow(&_show_demo_window);
	
	if (_show_main_menu)
	{
		for (auto& menu_item : _menu_items)
		{
			menu_item->visit();
		}
	}

	ImGui::Render();

	auto drawData = ImGui::GetDrawData();

	static_assert(sizeof(MeshData::Vertex) == sizeof(ImDrawVert));
	static_assert(sizeof(MeshData::Vertex::pos) == sizeof(ImDrawVert::pos));
	static_assert(sizeof(MeshData::Vertex::texcoord) == sizeof(ImDrawVert::uv));
	static_assert(sizeof(MeshData::Vertex::color) == sizeof(ImDrawVert::col));
	static_assert(offsetof(MeshData::Vertex, pos) == offsetof(ImDrawVert, pos));
	static_assert(offsetof(MeshData::Vertex, texcoord) == offsetof(ImDrawVert, uv));
	static_assert(offsetof(MeshData::Vertex, color) == offsetof(ImDrawVert, col));

	for (auto n = 0; n < drawData->CmdListsCount; n++)
	{
		const auto* cmd_list = drawData->CmdLists[n];
		
		auto vertex_data = reinterpret_cast<MeshData::Vertex*>(cmd_list->VtxBuffer.Data);
		size_t vert_count = static_cast<size_t>(cmd_list->VtxBuffer.size());

		const auto* indexes = cmd_list->IdxBuffer.Data;
		size_t index_count = static_cast<size_t>(cmd_list->IdxBuffer.size());

		MeshData mesh_data;
		mesh_data.vertices = { vertex_data, vertex_data + vert_count };
		mesh_data.indexes = { indexes, indexes + index_count };

		auto vertexBuffer = _engine.get_render().create_vertex_buffer(std::move(mesh_data));

		_command.vertex_buffer = std::move(vertexBuffer);

		_screen_size_uniform->value.x = _engine.get_window_width();
		_screen_size_uniform->value.y = _engine.get_window_height();

		_transform_uniform->value = glm::mat3(1.0f);

		size_t offset = 0;
		for (auto cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
		{
			const auto* pcmd = &cmd_list->CmdBuffer[cmd_i];

			_command.sub.emplace();
			_command.sub->num = pcmd->ElemCount;
			_command.sub->offset = offset * sizeof(std::uint32_t);

			_command.scissor = glm::vec4(pcmd->ClipRect.x, pcmd->ClipRect.y, pcmd->ClipRect.z, pcmd->ClipRect.w);

			_engine.get_render().add_command(_command);

			offset += pcmd->ElemCount;
		}
	}
}

void UIManager::add_menu_item(std::shared_ptr<MenuItem> menu_item)
{
	_menu_items.push_back(std::move(menu_item));
}

void UIManager::remove_menu_item(const std::shared_ptr<MenuItem>& menu_item)
{
	auto it = std::find(std::begin(_menu_items), std::end(_menu_items), menu_item);
	_menu_items.erase(it);
}

bool UIManager::get_show_main_menu()
{
	return _show_main_menu;
}

void Menu::Button::visit()
{
	if (ImGui::Button(_text.c_str()))
	{
		_is_clicked = true;
	}
}

bool Menu::Button::get_is_clicked()
{
	return _is_clicked;
}

void Menu::Button::set_is_clicked(bool a)
{
	_is_clicked = a;
}

void Menu::Slider::visit()
{
	ImGui::SliderInt(_text.c_str(), _val, _min, _max);
}

void BeginItem::visit()
{
	ImGui::Begin(_text.c_str());
}

void EndItem::visit()
{
	ImGui::End();
}