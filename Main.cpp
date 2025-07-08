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

    ma->setVertexAttributes(0, Color{1, 1, 0}, Position{1, 1, 1}, 1.2f);
    ma->setTriangleAttributes(0, Color{0, 1, 1}, Position{0, 0, 0}, 0.3f);

    // pra mostrar que os atributos dos que não foram alterados continuam os mesmos...
    // std::cout << ma->vertexAttribute<0>(1) << std::endl;
    // std::cout << ma->vertexAttribute<1>(1) << std::endl;
    // std::cout << ma->vertexAttribute<2>(1) << std::endl;
    // std::cout << "---" << std::endl;
    // std::cout << ma->triangleAttribute<0>(1) << std::endl;
    // std::cout << ma->triangleAttribute<1>(1) << std::endl;
    // std::cout << ma->triangleAttribute<2>(1) << std::endl;
    // std::cout << "---" << std::endl;

    return ma;
  }

int
main()
{
  auto filename = "./f-16.obj";
  auto mesh = readOBJ(filename);

  if (!mesh)
    printf("Could not read '%s'\n", filename);
  //else
  //  mesh->print(filename);

  auto ma = decorateAttributes(*mesh);
  // mostrando os atributos pós alterações
  std::cout << "------------------------------- multiple attributes\n";
  for (auto i = 0; i < 2; i++) {
      std::cout << "at pos " << i << ":\n\t";
      std::cout << "vertex[ma](color): " << ma->vertexAttribute<0>(i) << '\n';
      std::cout << "\tvertex[ma](position): " << ma->vertexAttribute<1>(i) << '\n';
      std::cout << "\tvertex[ma](intensity): " << ma->vertexAttribute<2>(i) << '\n';
      std::cout << "\ttriangle[ma](color): " << ma->triangleAttribute<0>(i) << '\n';
      std::cout << "\ttriangle[ma](position): " << ma->triangleAttribute<1>(i) << '\n';
      std::cout << "\ttriangle[ma](intensity): " << ma->triangleAttribute<2>(i) << '\n';
  }
  std::cout << "------------------------------- end of multiple attributes\n";
  ///////////////////////////////////////////////////multiplos

  ///////////////////////////////////////////////////movimento
  using VA = ElementAttribute<Color, Position, Intensity>;
  using TA = ElementAttribute<Color>;
  using MA = MeshAttribute<VA, TA>;
   
  auto ma2 = MA::New(std::move(ma->vertexAttributes()), *mesh);

  auto n = mesh->data().triangleCount();
  for (decltype(n) i = 0; i < n; ++i) {
      ma2->setTriangleAttributes(i, Color{ 1.0, 1.0f, 1.0f });
  }

  std::cout << "------------------------------- movement\n";
  std::cout << "movement from ma to ma2: \n";
  for (auto i = 0; i < 2; i++) {
      std::cout << "at pos " << i << ":\n\t";
      std::cout << "vertex[ma2](color): " << ma2->vertexAttribute<0>(i) << '\n';
      std::cout << "\tvertex[ma2](position): " << ma2->vertexAttribute<1>(i) << '\n';
      std::cout << "\tvertex[ma2](intensity): " << ma2->vertexAttribute<2>(i) << '\n';
      std::cout << "\ttriangle[ma2](color): " << ma2->triangleAttribute<0>(i) << '\n';
  }
  std::cout << "------------------------------- end of movement\n";
  ///////////////////////////////////////////////////movimento

  ///////////////////////////////////////////////////void
  using Vertex = MeshAttribute<VA, void>; 
  using Triangle = MeshAttribute<void, TA>;
  
  auto vertex = Vertex::New(*mesh);
  auto triangle = Triangle::New(*mesh);

  vertex->setVertexAttributes(0, Color{0.01, 0.02, 0.03}, Position{0.1, 0.2, 0.3}, Intensity{0.123f});
  triangle->setTriangleAttribute<0>(0, Color{0.23, 0.09, 0.05});
    
  std::cout << "------------------------------- void\n";
  std::cout << "===vertex\n";
  std::cout << "Atributos do vertice: " << vertex->vertexAttribute<0>(0) << "," << vertex->vertexAttribute<1>(0) << "\n";
  std::cout << "===triangle\n";
  std::cout << "Atributos do triangulo: " << triangle->triangleAttribute<0>(0) << "\n";
  std::cout << "------------------------------- void\n";
///////////////////////////////////////////////////void

  puts("Press any key to exit...");
  (void)getchar();
  return 0;
}
