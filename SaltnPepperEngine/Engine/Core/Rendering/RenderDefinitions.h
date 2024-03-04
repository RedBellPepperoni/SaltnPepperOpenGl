#ifndef RENDERDEFINITIONS_H
#define RENDERDEFINITIONS_H

#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Utils/GLUtils.h"
#include <string>
#include <vector>

#define MAX_NUM_OF_BONES_PER_VERTEX 4
#define MAX_BONES 200


namespace SaltnPepperEngine
{	

	namespace Rendering
	{
		enum class UsageType
		{
			STREAM_DRAW,
			STREAM_READ,
			STREAM_COPY,
			STATIC_DRAW,
			STATIC_READ,
			STATIC_COPY,
			DYNAMIC_DRAW,
			DYNAMIC_READ,
			DYNAMIC_COPY,
		};

		enum class BufferType
		{
			UNKNOWN = 0,
			ARRAY,
			ELEMENT_ARRAY,
			SHADER_STORAGE,
		};

		// Returns the correct GL value for the usage
		static uint32_t GetGLBufferUsage(UsageType usage)
		{
			switch (usage)
			{
			case UsageType::STREAM_DRAW: return GL_STREAM_DRAW;
				break;
			case UsageType::STREAM_READ: return GL_STREAM_READ;
				break;
			case UsageType::STREAM_COPY: return GL_STREAM_COPY;
				break;
			case UsageType::STATIC_DRAW: return GL_STATIC_DRAW;
				break;
			case UsageType::STATIC_READ: return GL_STATIC_READ;
				break;
			case UsageType::STATIC_COPY: return GL_STATIC_COPY;
				break;
			case UsageType::DYNAMIC_DRAW: return GL_DYNAMIC_DRAW;
				break;
			case UsageType::DYNAMIC_READ: return GL_DYNAMIC_READ;
				break;
			case UsageType::DYNAMIC_COPY: return GL_DYNAMIC_COPY;
				break;
			default: return GL_FALSE;
				break;
			}
		}

		//struct BoneInfluence
		//{
		//	int BoneInfoIndices[MAX_NUM_OF_BONES_PER_VERTEX] = { 0, 0, 0, 0 };
		//	float Weights[MAX_NUM_OF_BONES_PER_VERTEX] = { 0.0f, 0.0f, 0.0f, 0.0f };


		//	void AddBoneData(int boneInfoIndex, float weight)
		//	{
		//		if (weight < 0.0f || weight > 1.0f)
		//		{
		//			weight = Clamp(weight, 0.0f, 1.0f);
		//		}
		//		if (weight > 0.0f)
		//		{
		//			for (size_t i = 0; i < MAX_NUM_OF_BONES_PER_VERTEX; i++)
		//			{
		//				if (Weights[i] == 0.0f)
		//				{
		//					BoneInfoIndices[i] = boneInfoIndex;
		//					Weights[i] = weight;
		//					return;
		//				}

		//				
		//			}


		//			
		//		}
		//	}

		//	void NormalizeWeights()
		//	{
		//		float sumWeights = 0.0f;
		//		for (size_t i = 0; i < 4; i++)
		//		{
		//			sumWeights += Weights[i];
		//		}
		//		if (sumWeights > 0.0f)
		//		{
		//			for (size_t i = 0; i < 4; i++)
		//			{
		//				Weights[i] /= sumWeights;
		//			}
		//		}
		//	}

		//	//static constexpr size_t Size =  
		//};


		//struct BoneInfo
		//{
		//	Matrix4 offsetMatrix;
		//	Matrix4 finalTransform;
		//	//Matrix4 globalTransform;
		//	
		//	BoneInfo(Matrix4& offset)
		//	{
		//		offsetMatrix = offset;
		//		finalTransform = Matrix4(1.0f);
		//	}

		//};

		//struct Node
		//{
		//	Node(const std::string& name) : Name(name) { }
		//	std::string Name;
		//	Matrix4 Transformation;
		//	std::vector<Node*> Children;
		//};

		//// Animation Node
		//struct NodeAnim
		//{
		//	NodeAnim(const std::string& name) : Name(name) { }
		//	std::string Name;
		//	/*std::vector<PositionKeyFrame> m_PositionKeyFrames;
		//	std::vector<ScaleKeyFrame> m_ScaleKeyFrames;
		//	std::vector<RotationKeyFrame> m_RotationKeyFrames;*/
		//};



		struct Vertex
		{
			Vector3 position{ 0.0f };
			Vector2 texCoord{ 0.0f };
			Vector3 normal{ 0.0f };
			Vector3 tangent{ 0.0f };
			Vector3 bitangent{ 0.0f };

			static const size_t Size = 3 + 2 + 3 + 3 + 3;
		};

		struct SkinnedVertex
		{
			Vector3 position{ 0.0f };
			Vector2 texCoord{ 0.0f };
			Vector3 normal{ 0.0f };
			Vector3 tangent{ 0.0f };
			Vector3 bitangent{ 0.0f };
			//BoneInfluence boneData{};
			Vector4Int BoneIds {0};
			Vector4 BoneWeights {0.0f};

			//static const size_t Size = 3 + 2 + 3 + 3 + 3;
		};

		struct Triangle
		{
			Vertex v0;
			Vertex v1;
			Vertex v2;
		};

		


	}

}

#endif // !RENDERDEFINITIONS_H

