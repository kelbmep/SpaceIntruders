#include <GL/Sprite.hpp>

void Sprite::Transform(float scale, int rotation, glm::vec2 pos)//int pos_x, int pos_y)
{
	float angle = (rotation + 360) % 360 * M_PI / 180;
	for (Engine::VertexTriangle& tr : _triangles)
	{
		tr.arr[0].x = tr.arr[0].x * scale * cos(angle) - sin(angle) * tr.arr[0].y + pos.x;
		tr.arr[0].y = tr.arr[0].x * sin(angle) + scale * cos(angle) * tr.arr[0].y + pos.y;
		tr.arr[1].x = tr.arr[1].x * scale * cos(angle) - sin(angle) * tr.arr[1].y + pos.x;
		tr.arr[1].y = tr.arr[1].x * sin(angle) + scale * cos(angle) * tr.arr[1].y + pos.y;
		tr.arr[2].x = tr.arr[2].x * scale * cos(angle) - sin(angle) * tr.arr[2].y + pos.x;
		tr.arr[2].y = tr.arr[2].x * sin(angle) + scale * cos(angle) * tr.arr[2].y + pos.y;
	}
	this->_scale *= scale;
	this->_rotation += rotation;
	this->_position += pos;
}

/*glm::mat3 Sprite::getTransform()
{
	glm::mat3 trans(1.0f);

	trans = glm::translate(trans, _position);
	trans = glm::scale(trans, _scale);
	trans = glm::rotate(trans, glm::radians(_rotation));
	//trans = glm::translate(trans, -glm::vec2(_anchor.x * _contentSize.x, _anchor.y * _contentSize.y));

	return trans;
}*/

void Sprite::push(Engine::VertexTriangle a)
{
	_triangles.push_back(a);
}

const Engine::VertexTriangle* Sprite::get_data() const
{
	return _triangles.data();
}

//Engine::VertexTriangle* Sprite::get_triangles() const
//{
//	return _triangles.data();
//}

size_t Sprite::get_size() const
{
	return _triangles.size();
}