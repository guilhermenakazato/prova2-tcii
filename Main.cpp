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
auto decorate(const TriangleMesh& mesh) {
    using Color = Vec3f;
    using VA = ElementAttribute<Color>; // ElementAttribute<Color>;
    using TA = ElementAttribute<Color>;
    using MA = MeshAttribute<VA, TA>;

    auto nv = mesh.data().vertexCount();
    auto nt = mesh.data().triangleCount();
    auto ma = MA::New(mesh);

    // decltype pega o tipo de nv... acho que em tempo de compilação
    for(decltype(nv) i = 0; i < nv; ++i) {
      ma->setVertexAttributes(i, Color{0, 0, 1});
    }

    ma->setVertexAttribute<0>(0, Color{1, 1, 0});
    for(decltype(nv) i = 0; i < nv; ++i) {
      ma->setTriangleAttributes(i, Color{0, 1, 0});
    }
    ma->setTriangleAttribute<0>(0, Color{0, 1, 1});

    return ma;
  }

/**
 * pegar o mesh dps...
 */
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
