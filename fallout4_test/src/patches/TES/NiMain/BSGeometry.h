#pragma once

#include "NiPointer.h"
#include "NiAVObject.h"
#include "NiAlphaProperty.h"
#include "NiSkinInstance.h"

class NiShadeProperty;
class BSShaderProperty;

enum
{
	GEOMETRY_TYPE_GEOMETRY = 0x0,
	GEOMETRY_TYPE_PARTICLES = 0x1,
	GEOMETRY_TYPE_STRIP_PARTICLES = 0x2,
	GEOMETRY_TYPE_TRISHAPE = 0x3,
	GEOMETRY_TYPE_DYNAMIC_TRISHAPE = 0x4,
	GEOMETRY_TYPE_MESHLOD_TRISHAPE = 0x5,
	GEOMETRY_TYPE_LOD_MULTIINDEX_TRISHAPE = 0x6,
	GEOMETRY_TYPE_MULTIINDEX_TRISHAPE = 0x7,
	GEOMETRY_TYPE_SUBINDEX_TRISHAPE = 0x8,
	GEOMETRY_TYPE_SUBINDEX_LAND_TRISHAPE = 0x9,
	GEOMETRY_TYPE_MULTISTREAMINSTANCE_TRISHAPE = 0xA,
	GEOMETRY_TYPE_PARTICLE_SHADER_DYNAMIC_TRISHAPE = 0xB,
	GEOMETRY_TYPE_LINES = 0xC,
	GEOMETRY_TYPE_DYNAMIC_LINES = 0xD,
	GEOMETRY_TYPE_INSTANCE_GROUP = 0xE,
};

class BSGeometry : public NiAVObject
{
public:
	virtual ~BSGeometry();
	virtual class BSMultiIndexTriShape *IsMultiIndexTriShape();
	virtual class BSSkinnedDecalTriShape *IsSkinnedDecalTriShape();
	virtual void *IsUnknown();

	char _pad[0x10];
	NiPointer<NiProperty> spProperties[2];
	NiPointer<NiSkinInstance> spSkinInstance;
	void *pRendererData;
	char _pad2[0x8];
	uint64_t uiVertexDesc;
	char ucType;
	// bool Registered?

	NiProperty *QProperty(uint32_t Index) const
	{
		AssertDebug(Index < ARRAYSIZE(spProperties));

		return spProperties[Index];
	}

	NiAlphaProperty *QAlphaProperty() const
	{
		return static_cast<NiAlphaProperty *>(QProperty(0));
	}

	NiShadeProperty *QNiShadeProperty() const
	{
		return reinterpret_cast<NiShadeProperty *>(QProperty(1));
	}

	BSShaderProperty *QShaderProperty() const
	{
		return reinterpret_cast<BSShaderProperty *>(QProperty(1));
	}

	NiSkinInstance *QSkinInstance() const
	{
		return spSkinInstance;
	}

	void *QRendererData() const
	{
		return pRendererData;
	}

	uint64_t GetVertexDesc() const
	{
		return uiVertexDesc;
	}

	uint32_t GetVertexAttributeOffset(uint32_t Index) const
	{
		return CalculateVertexAttributeOffset(uiVertexDesc, Index);
	}

	uint32_t GetVertexAttributeStream(uint32_t Index) const
	{
		return CalculateVertexAttributeStream(uiVertexDesc, Index);
	}

	uint32_t GetVertexSize() const
	{
		return CalculateVertexSize(uiVertexDesc);
	}

	uint32_t GetDynamicVertexSize() const
	{
		return CalculateDyanmicVertexSize(uiVertexDesc);
	}

	int QType() const
	{
		return ucType;
	}

	void GetViewerStrings(void(*Callback)(const char *, ...), bool Recursive) const
	{
		if (Recursive)
			__super::GetViewerStrings(Callback, Recursive);

		static const char *vertexDescTypes[] =
		{
			"POSITION",
			"TEXCOORD0",
			"TEXCOORD1",
			"NORMAL",
			"BINORMAL",
			"COLOR",
			"SKINNING_DATA",
			"LANDSCAPE_DATA",
			"EYE_DATA",
			"EYE_INSTANCE",
		};

		char buffer[1024] = {};
		for (int i = 0; i < ARRAYSIZE(vertexDescTypes); i++)
		{
			if (HasVertexAttribute(GetVertexDesc(), i))
			{
				strcat_s(buffer, vertexDescTypes[i]);

				if (GetVertexAttributeStream(i) == 1)
					strcat_s(buffer, "[D]");
				else
					strcat_s(buffer, "[S]");

				strcat_s(buffer, " ");
			}
		}

		XUtil::Trim(buffer, ' ');

		Callback("-- BSGeometry --\n");
		Callback("Type = %d\n", QType());
		Callback("Vertex Desc = 0x%llX (%s)\n", GetVertexDesc(), buffer);
		Callback("Vertex Stride = %u\n", GetVertexSize());
		Callback("Vertex Stride Dynamic = %u\n", GetDynamicVertexSize());
		Callback("Renderer Data = 0x%p\n", QRendererData());
		Callback("Skin Instance = 0x%p\n", QSkinInstance());
	}

	//
	// enum BSGraphics::VertexDescEntry
	// https://forum.niftools.org/topic/4514-bsvertexdata-bsvertexformat/
	// https://github.com/ousnius/BodySlide-and-Outfit-Studio/blob/4537c4fc66a79d8a54e72cc12a732dc00008d356/lib/NIF/VertexData.h#L45
	//
	static bool HasVertexAttribute(uint64_t VertexDesc, uint32_t AttributeIndex)
	{
		return (VertexDesc & (0x40100000000000i64 << AttributeIndex)) != 0;
	}

	static uint32_t CalculateVertexAttributeOffset(uint64_t VertexDesc, uint32_t Index)
	{
		return (VertexDesc >> (4 * Index + 2)) & 0x3C;
	}

	static uint32_t CalculateVertexAttributeStream(uint64_t VertexDesc, uint32_t Index)
	{
		if (VertexDesc & ((uint64_t)Index << 44))
			return 0;

		if (VertexDesc & ((uint64_t)Index << 54))
			return 1;

		return 0xFFFFFFFF;
	}

	static uint32_t CalculateVertexSize(uint64_t VertexDesc)
	{
		// Vertex buffer slot 0
		return (VertexDesc << 2) & 0x3C;
	}

	static uint32_t CalculateDyanmicVertexSize(uint64_t VertexDesc)
	{
		// Vertex buffer slot 1 (BSDynamicTriShape)
		return (VertexDesc >> 2) & 0x3C;
	}
};
static_assert(sizeof(BSGeometry) == 0x158);
static_assert_offset(BSGeometry, spProperties, 0x120);
static_assert_offset(BSGeometry, spSkinInstance, 0x130);
static_assert_offset(BSGeometry, pRendererData, 0x138);
static_assert_offset(BSGeometry, uiVertexDesc, 0x148);
static_assert_offset(BSGeometry, ucType, 0x150);