#include <UIManager.hpp>
#include <shaderProgram.hpp>
#include <render.hpp>
#include <bitmap.hpp>
#include <imgui.h>

UIManager::UIManager(const Engine& engine) : _engine{ engine }
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();
	io.BackendPlatformName = "custom_micro_engine";

	io.KeyMap[ImGuiKey_A] = static_cast<size_t>(EventManager::KeyCode::A);
	
	io.RenderDrawListsFn = nullptr;

	auto width = 0;
	auto height = 0;
	unsigned char* dataPtr;
	io.Fonts->GetTexDataAsRGBA32(&dataPtr, &width, &height);
	std::vector<unsigned char> image(dataPtr, dataPtr + (width * height * 4));

	Bitmap bitmap(4, std::move(image), glm::vec2{ width, height });

	_program = _engine.get_render().create_program("draw");

	_texture_uniform = _program->create_texture_uniform("uTexture");
	_texture_uniform->texture = _engine.get_render().create_texture(std::move(bitmap));

	_screen_size_uniform = _program->create_vec2_uniform("uScreenSize");
	_transform_uniform = _program->create_mat3_uniform("uTransform");

	_command.program = _program;

	_engine.get_event_manager().add_delegate(this);
}

void UIManager::handle_event(EventManager::MouseEvent e)
{
	if (e.button == EventManager::MouseButton::Left)
	{
		_isLeft = (e.type == EventManager::KeyType::KeyDown);
	}

	if (e.button == EventManager::MouseButton::Right)
	{
		_isRight = (e.type == EventManager::KeyType::KeyDown);
	}

	if (e.button == EventManager::MouseButton::Middle)
	{
		_isMiddle = (e.type == EventManager::KeyType::KeyDown);
	}

	_mousePos.x = (float)e.x;
	_mousePos.y = (float)e.y;
}

void UIManager::handle_event(EventManager::TextInputEvent e)
{
	ImGuiIO& io = ImGui::GetIO();
	io.AddInputCharactersUTF8(e.text.data());
}

void UIManager::handle_event(EventManager::MouseMoveEvent e)
{
	_mousePos.x = (float)e.x;
	_mousePos.y = (float)e.y;
}

void UIManager::visit()
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(float(_engine.get_window_width()), float(_engine.get_window_height()));

	io.MousePos = { _mousePos.x, _mousePos.y };

	io.MouseDown[0] = _isLeft;
	io.MouseDown[1] = _isRight;
	io.MouseDown[2] = _isMiddle;

	ImGui::NewFrame();

	static auto show_demo_window = false;
	ImGui::ShowDemoWindow(&show_demo_window);
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

		MeshData meshData;
		meshData.vertices = { vertex_data, vertex_data + vert_count };
		meshData.indexes = { indexes, indexes + index_count };

		auto vertexBuffer = _engine.get_render().create_vertex_buffer(std::move(meshData));

		_command.vertex_buffer = std::move(vertexBuffer);

		_screen_size_uniform->value.x = _engine.get_window_width() / 1.0;
		_screen_size_uniform->value.y = _engine.get_window_height() / 1.0;

		_transform_uniform->value = glm::mat3(1.0f);

		size_t offset = 0;
		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
		{
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];

			_command.sub.emplace();
			_command.sub->num = pcmd->ElemCount;
			_command.sub->offset = offset * sizeof(std::uint32_t);

			_command.scissor = glm::vec4(pcmd->ClipRect.x, pcmd->ClipRect.y, pcmd->ClipRect.z, pcmd->ClipRect.w);

			_engine.get_render().add_command(_command);

			offset += pcmd->ElemCount;
		}
	}
}