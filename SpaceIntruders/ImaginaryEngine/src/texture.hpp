#include <glm/vec2.hpp>

class Texture
{
public:
    explicit Texture(glm::vec2);
    virtual ~Texture() = default;

    glm::vec2 get_size() const;
protected:
    glm::vec2 _size;
};