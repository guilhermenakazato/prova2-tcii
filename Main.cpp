#include "MeshAttribute.h"

/**
 * Descrição do trabalho:
 * primeiro, fazer um teste mais robusto em que esses atributos são múltiplos.
 * Ao invés de uma cor, pode ser uma cor e mais alguma coisa
 * 
 * o outro teste é criar uma decoração a partir de pedaços de outra decoração
 * ou seja, fazer um New que pega uma outra decoração
 * 
 * suponha que VA seja void. Assim, não terá atributo de vértice e os métodos relacionados
 * a VertexAttribute não existirão.
 * hint: especialização
 */

using namespace tcii::cg;
using Color = Vec3f;
using Position = Vec3f;
using Intensity = float;

auto decorate(const TriangleMesh& mesh) {
    using VA = ElementAttribute<Color, Intensity>; 
    using TA = ElementAttribute<Color>;
    using MA = MeshAttribute<VA, TA>;

    auto nv = mesh.data().vertexCount();
    auto nt = mesh.data().triangleCount();
    auto ma = MA::New(mesh);

    // decltype pega o tipo de nv em tempo de compilação
    for(decltype(nv) i = 0; i < nv; ++i) {
      ma->setVertexAttributes(i, Color{0, 0, 1}, Intensity{0.01f});
    }
    
    for(decltype(nt) i = 0; i < nt; i++) {
      ma->setTriangleAttributes(i, Color{0, 1, 0});
    }

    ma->setVertexAttributes(0, Color{1, 1, 0}, Intensity{0.2f});
    ma->setTriangleAttribute<0>(0, Color{0, 1, 1});

    return ma;
  }

  auto decorateAttributes(const TriangleMesh& mesh) {
    using VA = ElementAttribute<Color, Position, Intensity>; 
    using TA = ElementAttribute<Color, Position, Intensity>;
    using MA = MeshAttribute<VA, TA>;

    auto nv = mesh.data().vertexCount();
    auto nt = mesh.data().triangleCount();
    auto ma = MA::New(mesh);

    // decltype pega o tipo de nv em tempo de compilação
    for(decltype(nv) i = 0; i < nv; ++i) {
      ma->setVertexAttributes(i, Color{0, 0, 1}, Position{1, 0, 0}, Intensity{2.5f});
    }

    for(decltype(nt) i = 0; i < nt; ++i) {
      ma->setTriangleAttributes(i, Color{0, 1, 0}, Position{0, 0.5, 0}, Intensity{0.5f});
    }

    ma->setVertexAttribute<0>(0, Color{1, 1, 0});
    ma->setVertexAttribute<1>(0, Position{1, 2, 3});
    ma->setVertexAttribute<2>(0, Intensity{4.5f});

    // ma->setVertexAttributes(0, Color{1, 1, 0}, Position{1, 1, 1}, 1.2f);
    ma->setTriangleAttributes(0, Color{0, 1, 1}, Position{0, 0, 0}, 0.3f);

    std::cout << ma->vertexAttribute<0>(1) << std::endl;
    std::cout << ma->vertexAttribute<1>(1) << std::endl;
    std::cout << ma->vertexAttribute<2>(1) << std::endl;
    std::cout << "---" << std::endl;
    std::cout << ma->triangleAttribute<0>(1) << std::endl;
    std::cout << ma->triangleAttribute<1>(1) << std::endl;
    std::cout << ma->triangleAttribute<2>(1) << std::endl;
    std::cout << "---" << std::endl;


    return ma;
  }

int
main()
{
  auto filename = "./f-16.obj";
  auto mesh = readOBJ(filename);

  if (!mesh)
    printf("Could not read '%s'\n", filename);
  else
    mesh->print(filename);

  auto ma = decorate(*mesh);
  std::cout << ma->vertexAttribute<0>(0) << '\n';
  std::cout << ma->triangleAttribute<0>(0) << '\n';

  puts("Press any key to exit...");
  (void)getchar();
  return 0;
}
