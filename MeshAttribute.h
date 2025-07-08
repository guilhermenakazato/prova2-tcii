#ifndef __MeshAttribute_h
#define __MeshAttribute_h

#include "TriangleMesh.h"
#include "SoA.h"

namespace tcii::cg {
    struct Allocator {
        template <typename T> 
        static T* allocate(size_t size) {
            return new T[size];
        }

        template <typename T>
        static void free(T* ptr) {
            delete[] ptr;
        }
    };

    using MeshIndex = typename TriangleMesh::index_t;
    
    template <typename ...Fields>
    using ElementAttribute = SoA<Allocator, MeshIndex, Fields...>;

    // VA = VertexAttribute, TA = TriangleAttribute
    template <typename VA, typename TA> 
    class MeshAttribute : public SharedObject {
        public:
            
            /////////////////////////////////////////////////decoracao
            static ObjectPtr<MeshAttribute> New(const TriangleMesh& mesh) {
                return new MeshAttribute{mesh};
            }

            static ObjectPtr<MeshAttribute> New(VA&& va, const TriangleMesh& mesh) {
                return new MeshAttribute{ std::move(va), mesh };
            }

            static ObjectPtr<MeshAttribute> New(const TriangleMesh& mesh, TA&& ta) {
                return new MeshAttribute{ mesh, std::move(ta) };
            }

            static ObjectPtr<MeshAttribute> New(const TriangleMesh& mesh, TA&& ta, VA&& va) {
                return new MeshAttribute{ mesh, std::move(va), std::move(ta)};
            }
            /////////////////////////////////////////////////decoracao

            /**
             * auto&&... é referência universal
             */
            /////////////////////////////////////////////////vertices
            template <size_t I, typename Field>
            void setVertexAttribute(MeshIndex i, Field&& field) {
                _va.template get<I>(i) = std::forward<Field>(field);
            }   

            template <size_t I>
            auto& vertexAttribute(MeshIndex i) {
                return _va.template get<I>(i);
            } 

            template <typename... Fields>
            void setVertexAttributes(MeshIndex i, Fields&&... fields) {
                _va.set(i, std::forward<Fields>(fields)...);
            } 
            /////////////////////////////////////////////////vertices

            /////////////////////////////////////////////////triangulo
            template <size_t I>
            auto& triangleAttribute(MeshIndex i) {
                return _ta.template get<I>(i);
            } 

            template <size_t I, typename Field>
            void setTriangleAttribute(MeshIndex i, Field&& field) {
                _ta.template get<I>(i) = std::forward<Field>(field);
            }  

            template <typename... Fields>
            void setTriangleAttributes(MeshIndex i, Fields&&... fields) {
                _ta.set(i, std::forward<Fields>(fields)...);
            }  
            /////////////////////////////////////////////////triangulo

            /////////////////////////////////////////////////getter
            VA& vertexAttributes() { return _va; }
            TA& triangleAttributes() { return _ta; }
            /////////////////////////////////////////////////getter

        private:
            ObjectPtr<TriangleMesh> _mesh;
            VA _va;
            TA _ta;
            
            MeshAttribute(const TriangleMesh& mesh):
            _mesh{&mesh},
            _va{mesh.data().vertexCount()},
            _ta{mesh.data().triangleCount()} 
            {

            }

            MeshAttribute(VA&& va, const TriangleMesh& mesh) :
                _mesh{ &mesh },
                _va{ std::move(va) },
                _ta{ mesh.data().triangleCount() }
            {

            }

            MeshAttribute(const TriangleMesh& mesh, TA&& ta) :
                _mesh{ &mesh },
                _va{ mesh.data().vertexCount() },
                _ta{ std::move(ta) }
            {

            }

            MeshAttribute(const TriangleMesh& mesh, TA&& ta, VA&& va): 
                _mesh{&mesh},
                _va{std::move(va)},
                _ta{std::move(ta)}
            {

            }
    };

    // não tem TA
    template <typename VA>
    class MeshAttribute<VA, void> : public SharedObject {
        public:
            
            /////////////////////////////////////////////////decoracao
            static ObjectPtr<MeshAttribute> New(const TriangleMesh& mesh) {
                return new MeshAttribute{mesh};
            }

            static ObjectPtr<MeshAttribute> New(VA&& va, const TriangleMesh& mesh) {
                return new MeshAttribute{ std::move(va), mesh };
            }
            /////////////////////////////////////////////////decoracao

            /**
             * auto&&... é referência universal
             */
            /////////////////////////////////////////////////vertices
            template <size_t I, typename Field>
            void setVertexAttribute(MeshIndex i, Field&& field) {
                _va.template get<I>(i) = std::forward<Field>(field);
            }   

            template <size_t I>
            auto& vertexAttribute(MeshIndex i) {
                return _va.template get<I>(i);
            } 

            template <typename... Fields>
            void setVertexAttributes(MeshIndex i, Fields&&... fields) {
                _va.set(i, std::forward<Fields>(fields)...);
            } 
            /////////////////////////////////////////////////vertices

            /////////////////////////////////////////////////getter
            VA& vertexAttributes() { return _va; }
            /////////////////////////////////////////////////getter

        private:
            ObjectPtr<TriangleMesh> _mesh;
            VA _va;
            
            MeshAttribute(const TriangleMesh& mesh):
            _mesh{&mesh},
            _va{mesh.data().vertexCount()}
            {

            }

            MeshAttribute(VA&& va, const TriangleMesh& mesh) :
                _mesh{ &mesh },
                _va{ std::move(va) }
            {

            }
    };

    // não tem VA
    template <typename TA> 
    class MeshAttribute<void, TA> : public SharedObject {
        public:
            
            /////////////////////////////////////////////////decoracao
            static ObjectPtr<MeshAttribute> New(const TriangleMesh& mesh) {
                return new MeshAttribute{mesh};
            }

            static ObjectPtr<MeshAttribute> New(const TriangleMesh& mesh, TA&& ta) {
                return new MeshAttribute{ mesh, std::move(ta) };
            }
            /////////////////////////////////////////////////decoracao

            /**
             * auto&&... é referência universal
             */

            /////////////////////////////////////////////////triangulo
            template <size_t I>
            auto& triangleAttribute(MeshIndex i) {
                return _ta.template get<I>(i);
            } 

            template <size_t I, typename Field>
            void setTriangleAttribute(MeshIndex i, Field&& field) {
                _ta.template get<I>(i) = std::forward<Field>(field);
            }  

            template <typename... Fields>
            void setTriangleAttributes(MeshIndex i, Fields&&... fields) {
                _ta.set(i, std::forward<Fields>(fields)...);
            }  
            /////////////////////////////////////////////////triangulo

            /////////////////////////////////////////////////getter
            TA& triangleAttributes() { return _ta; }
            /////////////////////////////////////////////////getter

        private:
            ObjectPtr<TriangleMesh> _mesh;
            TA _ta;
            
            MeshAttribute(const TriangleMesh& mesh):
            _mesh{&mesh},
            _ta{mesh.data().triangleCount()} 
            {

            }

            MeshAttribute(const TriangleMesh& mesh, TA&& ta) :
                _mesh{ &mesh },
                _ta{ std::move(ta) }
            {

            }
    };
}

#endif
