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

    // VA = VerticeAttribute, TA = TriangleAttribute
    template <typename VA, typename TA> 
    class MeshAttribute : public SharedObject {
        public:
            // factory
            static ObjectPtr<MeshAttribute> New(const TriangleMesh& mesh) {
                return new MeshAttribute{mesh};
            }

            /**
             * auto&&... é referência universal
             */
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

        private:
            ObjectPtr<TriangleMesh> _mesh;
            VA _va;
            TA _ta;
            
            MeshAttribute(const TriangleMesh& mesh):
            _mesh{&mesh},
            _va{mesh.data().vertexCount()},
            _ta{mesh.data().vertexCount()} 
            {

            }
    };
}

#endif
