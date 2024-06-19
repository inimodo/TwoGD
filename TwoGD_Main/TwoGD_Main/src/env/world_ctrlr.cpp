#include"../twogd.h"

o_world::o_world(){}
o_world::o_world(CODEC3D * o_Codec_)
{
	this->o_Layers = NULL;
	this->o_Codec = o_Codec_;
}

UINT32 o_world::AppendLayer(LPSTR c_StreamName, COLOR c_Color)
{
	if (this->o_Layers == NULL) 
	{
		this->o_Layers = (LAYER*)malloc(sizeof(LAYER));
		if (this->o_Layers == NULL) return NULL;

		this->o_Layers[0] = LAYER(c_StreamName,c_Color);
		this->i_Length = 1;
		return 0;
	}

	LAYER* o_pTemp = this->o_Layers;

	this->o_Layers = (LAYER*)realloc(this->o_Layers,(this->i_Length+1)* sizeof(LAYER));
	if (this->o_Layers == NULL) 
	{
		this->o_Layers = o_pTemp;
		return NULL;
	}

	this->o_Layers[this->i_Length] = LAYER(c_StreamName, c_Color);
	this->i_Length++;
	return this->i_Length-1;
}

UCHAR o_world::Render()
{
	for (UINT32 i_Index = 0; i_Index < this->i_Length; i_Index++)
	{
		this->o_Codec->DrawObject(&this->o_Layers[i_Index].o_Obj, &this->o_Layers[i_Index].c_Color, this->o_Layers[i_Index].i_PixelFlag, this->o_Layers[i_Index].i_PrioFlag);
	}

	return GD_TASK_OKAY;
}

UCHAR o_world::RayTrace(CAM3D * o_pCam, UINT32 * i_pLayer)
{
	V3 v_UnitZ = V3(0,0,1);
	v_UnitZ.RotateThis(o_pCam->i_Rotation);


	return GD_TASK_OKAY;
}

UCHAR o_world::Dispose()
{
	if (o_Layers != NULL)
	{
		for (UINT32 i_index = 0; i_index < this->i_Length; i_index++)
		{
			this->o_Layers[i_index].o_Obj.Dispose();
		}
		free(o_Layers);
	}
	return GD_TASK_OKAY;
}