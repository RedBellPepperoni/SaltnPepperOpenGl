#ifndef SKINNEDMODEL_H
#define SKINNEDMODEL_H

#include "Engine/Utils/Loading/FileSystem.h"
#include "Engine/Core/Rendering/Geometry/SkinnedMesh.h"
#include "Engine/Core/Rendering/Material/Material.h"
#include "Engine/Utils/Loading/ModelLoader.h"

namespace SaltnPepperEngine
{
	class SkinnedModel
	{
    private:
        std::vector<SharedPtr<SkinnedMesh>> m_meshes;
        //std::vector<Transform> m_MeshTransform;

        std::string m_filePath;


    public:

        SkinnedModel() = default;
        SkinnedModel(const FilePath& filepath);
        

        std::vector<SharedPtr<SkinnedMesh>>& GetMeshes();

        void Load(const FilePath& filepath);

        const std::string& GetFilePath() const;
     

      

	};


}

#endif // !SKINNEDMODEL_H

