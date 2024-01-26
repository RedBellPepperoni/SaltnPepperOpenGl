#include "Model.h"
namespace SaltnPepperEngine
{
    Model::Model(const FilePath& filepath)
    {
        Load(filepath);

    }

   

    Model::Model(PrimitiveType primitive)
        : m_filePath ("Primitive")
        , m_primitiveType(primitive)
    {
        m_meshes.push_back(GeneratePrimitive(primitive));
    }

    Model::Model(SharedPtr<Mesh>& mesh, PrimitiveType primitive)
        :m_filePath("Primitive")
        ,m_primitiveType(primitive)
    {
        m_meshes.push_back(mesh);
    }

    std::vector<SharedPtr<Mesh>>& Model::GetMeshes()
    {
        return m_meshes;
    }

    void Model::Load(const FilePath& filepath)
    {
        // fetching the model details from the given filepath
        ModelDetail loadedmodelDetail = ModelLoader::LoadModel(filepath);

        int meshCount = 0;

        // Incase there are sub meshes we generate a mesh for each of them
        for (MeshDetails meshDetail : loadedmodelDetail.meshes)
        {

            // Creating a new Mesh from the gathered data
            SharedPtr<Mesh> newMesh = MakeShared<Mesh>(meshDetail.vertices, meshDetail.indices);
            newMesh->SetName(FileSystem::GetFileName(filepath) + std::to_string(meshCount));


            // Adding the generated mesh to the Model List of meshes
            m_meshes.push_back(newMesh);

            meshCount++;
        }



    }
    const std::string& Model::GetFilePath() const
    {
        return m_filePath;
    }
    PrimitiveType Model::GetPrimitiveType()
    {
        return m_primitiveType;
    }

    void Model::SetPrimitiveType(PrimitiveType type)
    {
        m_primitiveType = type;
    }
}
